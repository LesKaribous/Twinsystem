"""
services.py — Simulation stubs for Motion, Vision, Safety and Chrono services.

API mirrors the C++ services so strategy.py code can be written identically
to what runs on the Teensy, minus the hardware-specific parts.

Threading model
───────────────
  Strategy thread  → calls motion.go(), vision.query_color_sync(), etc.
                     These calls BLOCK the strategy thread.
  Physics thread   → calls motion.update(dt), safety.update(dt) at 60 Hz.
                     These mutate robot state and signal the events.

  A threading.Event per motion command signals completion from physics → strategy.
"""

import math
import time
import threading
from dataclasses import dataclass, field
from typing import Optional, Callable

from config import (
    Vec2, angle_diff,
    FIELD_W, FIELD_H, ROBOT_RADIUS,
    MAX_SPEED, MAX_ACCEL, MAX_ROT_SPEED,
    MIN_DISTANCE, MIN_ANGLE, WAYPOINT_RADIUS,
    OBS_RADIUS, SAFETY_CHECK_S, SAFETY_RESUME_S,
    MATCH_DURATION_S,
    ObjectColor, RobotCompass, BlockResult,
    robot_compass_offset_deg, polar_vec, compass_deg,
    POI, Team,
)
from sim_core import RobotPhysics, OccupancyGrid, Pathfinder, GameObjects


# ─────────────────────────────────────────────────────────────────────────────
#  Internal structures
# ─────────────────────────────────────────────────────────────────────────────

@dataclass
class MoveOptions:
    collision_enabled: bool  = True
    cancel_on_collide: bool  = False
    optimize_rotation: bool  = True
    feedrate:          float = -1.0   # -1 → use global feedrate


@dataclass
class _Waypoint:
    pos:          Vec2
    theta:        Optional[float]  # None → keep current heading
    pass_through: bool
    opts:         MoveOptions = field(default_factory=MoveOptions)


class _MotionState:
    IDLE      = "IDLE"
    RUNNING   = "RUNNING"
    PAUSED    = "PAUSED"
    COMPLETED = "COMPLETED"
    CANCELED  = "CANCELED"


# ─────────────────────────────────────────────────────────────────────────────
#  Motion service
# ─────────────────────────────────────────────────────────────────────────────

class MotionService:
    """
    Mirrors the C++ Motion class API.

    Usage in strategy.py (all blocking):
        motion.go(500, 300)
        motion.go_align(POI.stock_1, RobotCompass.AB, 270)
        motion.cancel_on_collide().feedrate(0.8).go(500, 300)
        motion.via(200, 0).via(200, 300).go(500, 300)
        motion.turn(90)
        motion.was_successful()
    """

    MOVE_TIMEOUT_S = 30.0

    def __init__(self, robot: RobotPhysics,
                 occupancy: OccupancyGrid,
                 pathfinder: Pathfinder):
        self._robot    = robot
        self._occ      = occupancy
        self._pf       = pathfinder

        self._state    = _MotionState.IDLE
        self._done_evt = threading.Event()
        self._lock     = threading.Lock()

        self._waypoints: list[_Waypoint] = []
        self._wp_idx    = 0

        self._pending_opts     = MoveOptions()
        self._pending_via: list[Vec2] = []
        self._active_opts      = MoveOptions()

        self._feedrate         = 1.0
        self._pathfinding_on   = True
        self._last_successful  = False

        # Pause state
        self._paused_target_pos   = None
        self._paused_target_theta = None
        self._paused_wp_list      = []
        self._paused_wp_idx       = 0

        # For UI — expose current planned path
        self._current_path: list[Vec2] = []

    # ── Fluent option setters ────────────────────────────────────────────────

    def no_collide(self):
        self._pending_opts.collision_enabled = False
        return self

    def with_collision(self, on: bool = True):
        self._pending_opts.collision_enabled = on
        return self

    def cancel_on_collide(self, on: bool = True):
        self._pending_opts.cancel_on_collide = on
        return self

    def with_optimization(self, on: bool = True):
        self._pending_opts.optimize_rotation = on
        return self

    def feedrate(self, f: float):
        self._pending_opts.feedrate = max(0.05, min(1.0, f))
        return self

    # ── Via / waypoints ──────────────────────────────────────────────────────

    def via(self, x_or_vec, y: float = None):
        if isinstance(x_or_vec, Vec2):
            self._pending_via.append(x_or_vec)
        else:
            self._pending_via.append(Vec2(x_or_vec, y))
        return self

    # ── Movement commands (blocking) ─────────────────────────────────────────

    def go(self, x_or_vec, y: float = None):
        if isinstance(x_or_vec, Vec2):
            target = x_or_vec
        else:
            target = Vec2(x_or_vec, y)
        self._execute_move(target, None)
        return self

    def go_align(self, target: Vec2, rc: RobotCompass, orientation_deg: float):
        theta = math.radians(orientation_deg - robot_compass_offset_deg(rc))
        self._execute_move(target, theta)
        return self

    def go_polar(self, heading_deg: float, dist: float):
        target = self._robot.pos + polar_vec(heading_deg, dist)
        self._execute_move(target, None)
        return self

    def go_polar_align(self, heading_deg: float, dist: float,
                       rc: RobotCompass, orientation_deg: float):
        target = self._robot.pos + polar_vec(heading_deg, dist)
        theta  = math.radians(orientation_deg - robot_compass_offset_deg(rc))
        self._execute_move(target, theta)
        return self

    def turn(self, angle_deg: float):
        """Rotate to absolute heading (degrees)."""
        self._execute_move(self._robot.pos, math.radians(angle_deg),
                           pure_rotation=True)
        return self

    def align(self, rc: RobotCompass, orientation_deg: float):
        theta = math.radians(orientation_deg - robot_compass_offset_deg(rc))
        self._execute_move(self._robot.pos, theta, pure_rotation=True)
        return self

    # ── Internal move execution ──────────────────────────────────────────────

    def _execute_move(self, target: Vec2, theta: Optional[float],
                      pure_rotation: bool = False):
        opts = self._pending_opts
        via  = list(self._pending_via)
        self._pending_opts = MoveOptions()
        self._pending_via  = []

        effective_feedrate = opts.feedrate if opts.feedrate > 0 else self._feedrate

        # Build waypoint list
        wps: list[_Waypoint] = []

        # via points: use A* from each to next only if pathfinding on
        prev = self._robot.pos
        for v in via:
            if self._pathfinding_on and not pure_rotation:
                path_seg = self._pf.find_path(prev, v)
                for p in path_seg[1:]:
                    wps.append(_Waypoint(p, None, True, opts))
            else:
                wps.append(_Waypoint(v, None, True, opts))
            prev = v

        # Final target
        if self._pathfinding_on and not pure_rotation:
            path_seg = self._pf.find_path(prev, target)
            for i, p in enumerate(path_seg[1:]):
                is_last = (i == len(path_seg) - 2)
                wps.append(_Waypoint(p, theta if is_last else None, not is_last, opts))
        else:
            wps.append(_Waypoint(target, theta, False, opts))

        if not wps:
            self._last_successful = True
            return

        with self._lock:
            self._waypoints   = wps
            self._wp_idx      = 0
            self._active_opts = opts
            self._current_path = [self._robot.pos] + [w.pos for w in wps]

            self._robot.feedrate      = effective_feedrate
            self._robot.target_pos    = wps[0].pos
            self._robot.target_theta  = wps[0].theta if len(wps) == 1 else None
            self._state    = _MotionState.RUNNING
            self._done_evt = threading.Event()

        # Block strategy thread until done (with timeout)
        finished = self._done_evt.wait(timeout=self.MOVE_TIMEOUT_S)
        if not finished:
            self._cancel_internal()

        self._last_successful = (self._state == _MotionState.COMPLETED)

    # ── Physics-thread update (60 Hz) ────────────────────────────────────────

    def update(self, dt: float):
        with self._lock:
            if self._state != _MotionState.RUNNING:
                return

            # Collision cancel
            if self._active_opts.cancel_on_collide and self._robot.collided:
                self._cancel_internal(locked=True)
                return

            wp = self._waypoints[self._wp_idx]
            dist = self._robot.pos.dist(wp.pos)

            # Advance waypoint queue
            if wp.pass_through and dist < WAYPOINT_RADIUS:
                self._advance_waypoint(locked=True)
                return

            # Last waypoint: check full completion
            if not wp.pass_through:
                pos_ok   = dist < MIN_DISTANCE
                theta_ok = True
                if wp.theta is not None:
                    theta_ok = abs(angle_diff(wp.theta, self._robot.theta)) < MIN_ANGLE
                if pos_ok and theta_ok:
                    self._complete_internal(locked=True)

    def _advance_waypoint(self, locked: bool = False):
        """Move to next waypoint. Must be called with lock held if locked=True."""
        self._wp_idx += 1
        if self._wp_idx >= len(self._waypoints):
            self._complete_internal(locked=locked)
            return
        wp = self._waypoints[self._wp_idx]
        self._robot.target_pos   = wp.pos
        self._robot.target_theta = wp.theta

    def _complete_internal(self, locked: bool = False):
        self._state = _MotionState.COMPLETED
        self._robot.target_pos   = None
        self._robot.target_theta = None
        self._robot.vel          = Vec2(0, 0)
        self._robot.vtheta       = 0.0
        self._done_evt.set()

    def _cancel_internal(self, locked: bool = False):
        self._state = _MotionState.CANCELED
        self._robot.target_pos   = None
        self._robot.target_theta = None
        self._robot.vel          = Vec2(0, 0)
        self._robot.vtheta       = 0.0
        self._done_evt.set()

    # ── Lifecycle ────────────────────────────────────────────────────────────

    def cancel(self):
        with self._lock:
            if self._state == _MotionState.RUNNING:
                self._cancel_internal(locked=True)

    def pause(self):
        with self._lock:
            if self._state == _MotionState.RUNNING:
                self._state = _MotionState.PAUSED
                self._paused_target_pos   = self._robot.target_pos
                self._paused_target_theta = self._robot.target_theta
                self._robot.target_pos    = None
                self._robot.target_theta  = None

    def resume(self):
        with self._lock:
            if self._state == _MotionState.PAUSED:
                self._state = _MotionState.RUNNING
                self._robot.target_pos   = self._paused_target_pos
                self._robot.target_theta = self._paused_target_theta

    # ── Queries ──────────────────────────────────────────────────────────────

    def was_successful(self) -> bool:
        return self._last_successful

    def has_finished(self) -> bool:
        return self._state in (_MotionState.COMPLETED,
                               _MotionState.CANCELED,
                               _MotionState.IDLE)

    @property
    def state(self) -> str:
        return self._state

    def is_moving(self) -> bool:
        return self._state == _MotionState.RUNNING

    def is_paused(self) -> bool:
        return self._state == _MotionState.PAUSED

    def current_path(self) -> list[Vec2]:
        return self._current_path

    # ── Global settings ──────────────────────────────────────────────────────

    def set_feedrate(self, f: float):
        self._feedrate = max(0.05, min(1.0, f))

    def get_feedrate(self) -> float:
        return self._feedrate

    def set_pathfinding_enabled(self, on: bool):
        self._pathfinding_on = on

    def set_manual_target(self, pos: Vec2):
        """Set a manual target from the UI (outside strategy execution)."""
        if self._state in (_MotionState.IDLE,
                           _MotionState.COMPLETED,
                           _MotionState.CANCELED):
            with self._lock:
                self._robot.target_pos   = pos
                self._robot.target_theta = None
                self._current_path       = [self._robot.pos, pos]
                self._state = _MotionState.RUNNING
                self._waypoints = [_Waypoint(pos, None, False)]
                self._wp_idx    = 0
                self._done_evt  = threading.Event()
                # This move is fire-and-forget (not awaited by a strategy thread)


# ─────────────────────────────────────────────────────────────────────────────
#  Vision service
# ─────────────────────────────────────────────────────────────────────────────

class VisionService:
    """
    Stubs TwinVision (camera hub over serial).
    query_color_sync → look up color in GameObjects, simulate latency.
    """

    SIMULATED_LATENCY_S = 0.05   # 50ms simulated query time

    def __init__(self, game_objects: GameObjects):
        self._go = game_objects

    def query_color_sync(self, pos: Vec2, timeout_ms: int = 500) -> ObjectColor:
        """
        Blocking color query. Returns ObjectColor.UNKNOWN on timeout.
        Simulates 50ms camera latency.
        """
        time.sleep(self.SIMULATED_LATENCY_S)
        _, color = self._go.query_color_at(pos)
        return color

    def get_color(self, pos: Vec2) -> ObjectColor:
        """Cache lookup (instant)."""
        _, color = self._go.query_color_at(pos)
        return color

    def set_color(self, poi_name: str, color: ObjectColor):
        """Set color for a named POI (called from UI)."""
        self._go.set_color(poi_name, color)

    def request_map_update(self):
        """No-op in sim."""
        time.sleep(0.1)


# ─────────────────────────────────────────────────────────────────────────────
#  Safety service
# ─────────────────────────────────────────────────────────────────────────────

class SafetyService:
    """
    Pauses motion if an obstacle appears within the robot's path.
    Queries occupancy grid + dynamic obstacles.
    """

    LOOKAHEAD_MM  = 350.0
    SIDE_MARGIN   = ROBOT_RADIUS + 30.0

    def __init__(self, robot: RobotPhysics,
                 occupancy: OccupancyGrid,
                 motion: MotionService):
        self._robot    = robot
        self._occ      = occupancy
        self._motion   = motion
        self._enabled  = True
        self._detected = False
        self._last_seen_t = 0.0
        self._timer    = 0.0

    def enable(self):  self._enabled = True
    def disable(self): self._enabled = False

    @property
    def obstacle_detected(self) -> bool:
        return self._detected

    def update(self, dt: float):
        if not self._enabled:
            return

        self._timer += dt
        if self._timer < SAFETY_CHECK_S:
            return
        self._timer = 0.0

        # Only check when moving translationally
        if not self._motion.is_moving() or self._robot.vel.mag() < 50:
            self._detected = False
            if self._motion.is_paused():
                self._motion.resume()
            return

        # Look ahead in velocity direction
        direction = self._robot.vel.normalized()
        obstacles = self._check_lookahead(direction)

        now = time.time()
        if obstacles:
            self._detected    = True
            self._last_seen_t = now
            if not self._motion.is_paused():
                self._motion.pause()
        else:
            # Clear after SAFETY_RESUME_S since last detection
            if now - self._last_seen_t > SAFETY_RESUME_S:
                self._detected = False
                if self._motion.is_paused():
                    self._motion.resume()

    def _check_lookahead(self, direction: Vec2) -> bool:
        """Check discrete points along velocity vector for obstacles."""
        for step in range(1, 5):
            probe = self._robot.pos + direction * (self.LOOKAHEAD_MM * step / 4)
            if self._occ.is_occupied_circle(probe, self.SIDE_MARGIN):
                return True
        return False


# ─────────────────────────────────────────────────────────────────────────────
#  Chrono service
# ─────────────────────────────────────────────────────────────────────────────

class ChronoService:
    """Match timer (100 s)."""

    def __init__(self):
        self._elapsed = 0.0
        self._running = False

    def start(self):
        self._running = True

    def stop(self):
        self._running = False

    def reset(self):
        self._elapsed = 0.0
        self._running = False

    def update(self, dt: float):
        if self._running:
            self._elapsed = min(self._elapsed + dt, MATCH_DURATION_S)
            if self._elapsed >= MATCH_DURATION_S:
                self._running = False

    def time_left_s(self) -> float:
        return max(0.0, MATCH_DURATION_S - self._elapsed)

    def time_elapsed_s(self) -> float:
        return self._elapsed

    def is_running(self) -> bool:
        return self._running

    def is_finished(self) -> bool:
        return self._elapsed >= MATCH_DURATION_S
