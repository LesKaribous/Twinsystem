"""
sim_core.py — Robot physics, occupancy grid, A* pathfinder, game objects
"""

import math
import heapq
from config import (
    FIELD_W, FIELD_H, ROBOT_RADIUS,
    MAX_SPEED, MAX_ACCEL, MAX_ROT_SPEED, MAX_ROT_ACCEL,
    GRID_W, GRID_H, GRID_CELL, OBS_RADIUS,
    Vec2, angle_diff, Team, TEAM_START, POI, ObjectColor,
)


# ─────────────────────────────────────────────────────────────────────────────
#  Robot physics
# ─────────────────────────────────────────────────────────────────────────────

class RobotPhysics:
    """
    Simplified holonomic robot model.
    PD velocity control toward target position/heading.
    """

    # PD gains
    KP_POS  = 3.5    # position proportional (1/s)
    KP_ROT  = 5.0    # rotation proportional (1/s)

    TRAIL_DIST  = 15.0   # mm — min distance between trail points
    TRAIL_MAX   = 300    # max trail history

    def __init__(self):
        self.pos     = Vec2(375, 225)  # mm
        self.theta   = 0.0            # rad
        self.vel     = Vec2(0, 0)     # mm/s world frame
        self.vtheta  = 0.0            # rad/s

        self.target_pos:   Vec2  | None = None
        self.target_theta: float | None = None

        self.feedrate  = 1.0
        self.collided  = False
        self.trail: list[tuple[float,float]] = []

        # Stall detection
        self.stall_counter = 0

    # ── Reset ───────────────────────────────────────────────────────────────

    def reset(self):
        self.pos     = Vec2(375, 225)
        self.theta   = 0.0
        self.vel     = Vec2(0, 0)
        self.vtheta  = 0.0
        self.target_pos   = None
        self.target_theta = None
        self.feedrate  = 1.0
        self.collided  = False
        self.stall_counter = 0
        self.trail.clear()

    def reset_to_start(self, team: str):
        team_enum = Team(team)
        start_pos, start_theta = TEAM_START[team_enum]
        self.pos    = Vec2(start_pos.x, start_pos.y)
        self.theta  = start_theta
        self.vel    = Vec2(0, 0)
        self.vtheta = 0.0
        self.target_pos   = None
        self.target_theta = None
        self.collided  = False
        self.stall_counter = 0
        self.trail.clear()

    # ── Physics update ───────────────────────────────────────────────────────

    def update(self, dt: float, occupancy: 'OccupancyGrid'):
        v_max  = MAX_SPEED    * self.feedrate
        a_max  = MAX_ACCEL    * dt
        vr_max = MAX_ROT_SPEED * self.feedrate
        ar_max = MAX_ROT_ACCEL * dt

        # ---- Translational PD ----
        if self.target_pos is not None:
            err = self.target_pos - self.pos
            v_desired = err * self.KP_POS
            if v_desired.mag() > v_max:
                v_desired = v_desired.normalized() * v_max
        else:
            v_desired = Vec2(0, 0)

        # Acceleration ramp
        dv = v_desired - self.vel
        if dv.mag() > a_max:
            dv = dv.normalized() * a_max
        new_vel = self.vel + dv

        # Boundary & occupancy collision
        new_pos = self.pos + new_vel * dt
        new_pos.x = max(ROBOT_RADIUS, min(FIELD_W - ROBOT_RADIUS, new_pos.x))
        new_pos.y = max(ROBOT_RADIUS, min(FIELD_H - ROBOT_RADIUS, new_pos.y))

        if occupancy.is_occupied_circle(new_pos, ROBOT_RADIUS):
            new_vel = Vec2(0, 0)
            new_pos = self.pos
            self.collided = True
            self.stall_counter += 1
        else:
            self.collided = False
            self.stall_counter = max(0, self.stall_counter - 1)

        self.vel = new_vel
        self.pos = new_pos

        # ---- Rotational PD ----
        if self.target_theta is not None:
            err_r     = angle_diff(self.target_theta, self.theta)
            vr_des    = err_r * self.KP_ROT
            vr_des    = max(-vr_max, min(vr_max, vr_des))
            dvr       = vr_des - self.vtheta
            dvr       = max(-ar_max, min(ar_max, dvr))
            self.vtheta = self.vtheta + dvr
        else:
            # Decelerate
            dvr = max(-ar_max, min(ar_max, -self.vtheta))
            self.vtheta += dvr
            if abs(self.vtheta) < 0.01:
                self.vtheta = 0.0

        self.theta = (self.theta + self.vtheta * dt) % (2 * math.pi)

        # ---- Trail ----
        if not self.trail or Vec2(*self.trail[-1]).dist(self.pos) > self.TRAIL_DIST:
            self.trail.append((self.pos.x, self.pos.y))
            if len(self.trail) > self.TRAIL_MAX:
                self.trail.pop(0)

    # ── Serialization for WebSocket ──────────────────────────────────────────

    def to_dict(self) -> dict:
        return {
            'x':      self.pos.x,
            'y':      self.pos.y,
            'theta':  self.theta,
            'vx':     self.vel.x,
            'vy':     self.vel.y,
            'vtheta': self.vtheta,
            'radius': ROBOT_RADIUS,
            'collided': self.collided,
            'trail':  self.trail[-80:],  # last 80 points
        }


# ─────────────────────────────────────────────────────────────────────────────
#  Occupancy grid
# ─────────────────────────────────────────────────────────────────────────────

class OccupancyGrid:
    """
    20×13 binary grid, 150mm/cell.
    Also stores dynamic circular obstacles (adversary robot, placed manually).
    """

    def __init__(self):
        self._grid: list[list[bool]] = [[False] * GRID_H for _ in range(GRID_W)]
        self.dynamic_obstacles: list[Vec2] = []   # circle centers, radius=OBS_RADIUS

    def reset(self):
        for col in self._grid:
            for i in range(len(col)):
                col[i] = False
        self.dynamic_obstacles.clear()

    # ── Cell operations ──────────────────────────────────────────────────────

    def toggle_cell(self, gx: int, gy: int):
        if 0 <= gx < GRID_W and 0 <= gy < GRID_H:
            self._grid[gx][gy] = not self._grid[gx][gy]

    def set_cell(self, gx: int, gy: int, value: bool):
        if 0 <= gx < GRID_W and 0 <= gy < GRID_H:
            self._grid[gx][gy] = value

    def is_cell_occupied(self, gx: int, gy: int) -> bool:
        if gx < 0 or gx >= GRID_W or gy < 0 or gy >= GRID_H:
            return True   # out of bounds = wall
        return self._grid[gx][gy]

    # ── World coordinate queries ─────────────────────────────────────────────

    def world_to_grid(self, pos: Vec2) -> tuple[int, int]:
        return (int(pos.x // GRID_CELL), int(pos.y // GRID_CELL))

    def grid_to_world(self, gx: int, gy: int) -> Vec2:
        return Vec2((gx + 0.5) * GRID_CELL, (gy + 0.5) * GRID_CELL)

    def is_occupied_world(self, pos: Vec2) -> bool:
        gx, gy = self.world_to_grid(pos)
        return self.is_cell_occupied(gx, gy)

    def is_occupied_circle(self, center: Vec2, radius: float) -> bool:
        """True if a circle intersects any obstacle or boundary."""
        if (center.x - radius < 0 or center.x + radius > FIELD_W or
                center.y - radius < 0 or center.y + radius > FIELD_H):
            return True

        # Grid cells in bounding box
        x0 = max(0, int((center.x - radius) // GRID_CELL))
        x1 = min(GRID_W - 1, int((center.x + radius) // GRID_CELL))
        y0 = max(0, int((center.y - radius) // GRID_CELL))
        y1 = min(GRID_H - 1, int((center.y + radius) // GRID_CELL))

        for gx in range(x0, x1 + 1):
            for gy in range(y0, y1 + 1):
                if self._grid[gx][gy]:
                    cx = (gx + 0.5) * GRID_CELL
                    cy = (gy + 0.5) * GRID_CELL
                    # AABB→circle shortest distance
                    dx = max(abs(center.x - cx) - GRID_CELL / 2, 0.0)
                    dy = max(abs(center.y - cy) - GRID_CELL / 2, 0.0)
                    if dx * dx + dy * dy < radius * radius:
                        return True

        # Dynamic obstacles
        for obs in self.dynamic_obstacles:
            if center.dist(obs) < radius + OBS_RADIUS:
                return True

        return False

    def add_dynamic_obstacle(self, pos: Vec2):
        self.dynamic_obstacles.append(Vec2(pos.x, pos.y))

    def remove_nearest_dynamic(self, pos: Vec2, threshold: float = 200.0):
        if not self.dynamic_obstacles:
            return
        closest = min(self.dynamic_obstacles, key=lambda o: o.dist(pos))
        if closest.dist(pos) < threshold:
            self.dynamic_obstacles.remove(closest)

    # ── Serialization ────────────────────────────────────────────────────────

    def to_list(self) -> list:
        """Flat list of {gx, gy, occupied} for every cell."""
        cells = []
        for gx in range(GRID_W):
            for gy in range(GRID_H):
                if self._grid[gx][gy]:
                    cells.append({'gx': gx, 'gy': gy})
        return cells


# ─────────────────────────────────────────────────────────────────────────────
#  A* Pathfinder
# ─────────────────────────────────────────────────────────────────────────────

class Pathfinder:
    """
    8-directional A* on the 150mm occupancy grid.
    Returns simplified world-frame path as list[Vec2].
    """

    DIRS = [
        (1, 0, 1.0), (-1, 0, 1.0), (0, 1, 1.0), (0, -1, 1.0),
        (1, 1, 1.414), (-1, 1, 1.414), (1, -1, 1.414), (-1, -1, 1.414),
    ]

    def __init__(self, occupancy: OccupancyGrid):
        self.occ = occupancy

    def find_path(self, start: Vec2, goal: Vec2) -> list[Vec2]:
        """Return simplified world-frame path [start … goal]."""
        gs = self.occ.world_to_grid(start)
        gg = self.occ.world_to_grid(goal)

        # Trivial / unreachable
        if gs == gg:
            return [start, goal]
        if self.occ.is_cell_occupied(*gg):
            return [start, goal]   # destination blocked — direct line fallback

        # A* with (f, g, node) heap
        g_score = {gs: 0.0}
        came_from: dict = {}
        heap = [(self._h(gs, gg), 0.0, gs)]

        while heap:
            _, g, current = heapq.heappop(heap)

            if current == gg:
                return self._reconstruct(came_from, current, start, goal)

            if g > g_score.get(current, float('inf')):
                continue   # stale entry

            for dx, dy, cost in self.DIRS:
                nx, ny = current[0] + dx, current[1] + dy
                neighbor = (nx, ny)
                if self.occ.is_cell_occupied(nx, ny):
                    continue
                ng = g + cost
                if ng < g_score.get(neighbor, float('inf')):
                    g_score[neighbor] = ng
                    came_from[neighbor] = current
                    heapq.heappush(heap, (ng + self._h(neighbor, gg), ng, neighbor))

        return [start, goal]   # no path found

    def _h(self, a: tuple, b: tuple) -> float:
        return max(abs(a[0] - b[0]), abs(a[1] - b[1]))   # Chebyshev

    def _reconstruct(self, came_from: dict, current: tuple,
                     start: Vec2, goal: Vec2) -> list[Vec2]:
        cells = []
        while current in came_from:
            cells.append(current)
            current = came_from[current]
        cells.reverse()

        path = [start] + [self.occ.grid_to_world(gx, gy) for gx, gy in cells] + [goal]
        return self._simplify(path)

    def _simplify(self, path: list[Vec2]) -> list[Vec2]:
        """Remove collinear intermediate points (Douglas-Peucker lite)."""
        if len(path) <= 2:
            return path
        result = [path[0]]
        for i in range(1, len(path) - 1):
            v1 = path[i]     - path[i - 1]
            v2 = path[i + 1] - path[i]
            cross = v1.x * v2.y - v1.y * v2.x
            if abs(cross) > 500.0:   # mm² threshold
                result.append(path[i])
        result.append(path[-1])
        return result


# ─────────────────────────────────────────────────────────────────────────────
#  Game objects (colored items on field)
# ─────────────────────────────────────────────────────────────────────────────

class GameObjects:
    """
    Stores the colors assigned to each named POI (e.g. stock_1 → YELLOW).
    Simulates what the TwinVision camera hub would return.
    """

    MATCH_RADIUS = 80.0   # mm — how close a query position must be

    def __init__(self):
        # {poi_name: ObjectColor}
        self._colors: dict[str, ObjectColor] = {}
        # Initialize all stocks to UNKNOWN
        for name, _ in POI.all_named():
            self._colors[name] = ObjectColor.UNKNOWN

    def set_color(self, poi_name: str, color: ObjectColor):
        self._colors[poi_name] = color

    def get_color(self, poi_name: str) -> ObjectColor:
        return self._colors.get(poi_name, ObjectColor.UNKNOWN)

    def query_color_at(self, pos: Vec2) -> tuple[str | None, ObjectColor]:
        """Return (poi_name, color) of the nearest POI within MATCH_RADIUS."""
        best_name = None
        best_dist = float('inf')
        for name, poi_pos in POI.all_named():
            d = pos.dist(poi_pos)
            if d < self.MATCH_RADIUS and d < best_dist:
                best_dist = d
                best_name = name
        if best_name:
            return best_name, self._colors.get(best_name, ObjectColor.UNKNOWN)
        return None, ObjectColor.UNKNOWN

    def to_list(self) -> list:
        result = []
        for name, pos in POI.all_named():
            result.append({
                'name':  name,
                'x':     pos.x,
                'y':     pos.y,
                'color': self._colors.get(name, ObjectColor.UNKNOWN).name,
            })
        return result
