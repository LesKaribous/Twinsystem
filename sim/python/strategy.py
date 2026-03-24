"""
strategy.py — TwinSystem match strategy (simulation)
═══════════════════════════════════════════════════════════════════════════════
This file is HOT-RELOADED automatically when saved.
Edit and save → the simulator picks up changes immediately.

Available globals (injected by the simulator):
    motion    — MotionService   (go, turn, via, goAlign, feedrate, …)
    vision    — VisionService   (query_color_sync, get_color)
    safety    — SafetyService   (enable, disable)
    chrono    — ChronoService   (time_left_s, is_running)
    log(msg)  — print to simulator console

API mirrors the C++ motion service:
    motion.go(x, y)
    motion.go(Vec2(x, y))
    motion.go_align(pos, RobotCompass.AB, 270)
    motion.go_polar(heading_deg, dist_mm)
    motion.turn(angle_deg)
    motion.via(x, y).via(x2, y2).go(x3, y3)   # path through waypoints
    motion.cancel_on_collide().feedrate(0.8).go(x, y)
    motion.no_collide().turn(90)
    motion.was_successful()  → bool

Vision:
    color = vision.query_color_sync(POI.stock_1, timeout_ms=400)
    # returns ObjectColor.YELLOW / RED / GREEN / BLUE / UNKNOWN / NONE

Mission blocks return BlockResult.SUCCESS or BlockResult.FAILED
═══════════════════════════════════════════════════════════════════════════════
"""

# ── These are injected by server.py — do NOT import them manually ────────────
# motion, vision, safety, chrono, log

from config import (
    POI, Vec2, polar_vec, compass_deg,
    ObjectColor, RobotCompass, TableCompass, BlockResult, Team,
)
from mission import Mission, Block


# ─────────────────────────────────────────────────────────────────────────────
#  Helpers
# ─────────────────────────────────────────────────────────────────────────────

def wait_ms(ms: int):
    import time
    time.sleep(ms / 1000.0)


def is_useful_color(color: ObjectColor) -> bool:
    return color in (ObjectColor.YELLOW, ObjectColor.RED,
                     ObjectColor.GREEN, ObjectColor.BLUE)


# ─────────────────────────────────────────────────────────────────────────────
#  Mission blocks
# ─────────────────────────────────────────────────────────────────────────────

def block_collect_stock_1() -> BlockResult:
    """Collect stock at position 1 (bottom-left)."""
    log("→ Collecting stock_1")

    # Query color from vision hub
    color = vision.query_color_sync(POI.stock_1, 400)
    log(f"  Color at stock_1: {color.name}")
    if not is_useful_color(color):
        return BlockResult.FAILED

    # Approach from south (robot side AB facing south wall)
    approach = POI.stock_1 + Vec2(0, 300)
    motion.cancel_on_collide().go(approach.x, approach.y)
    if not motion.was_successful():
        return BlockResult.FAILED

    # Final approach
    motion.cancel_on_collide().feedrate(0.6).go(POI.stock_1.x, POI.stock_1.y + 150)
    if not motion.was_successful():
        return BlockResult.FAILED

    log("  Collected stock_1 ✓")
    wait_ms(500)
    return BlockResult.SUCCESS


def block_collect_stock_4() -> BlockResult:
    """Collect stock at center position 4."""
    log("→ Collecting stock_4")

    color = vision.query_color_sync(POI.stock_4, 400)
    log(f"  Color at stock_4: {color.name}")
    if not is_useful_color(color):
        return BlockResult.FAILED

    # Navigate via waypoint to avoid center obstacles
    motion.via(POI.yellowWaypoint_1.x, POI.yellowWaypoint_1.y)\
          .cancel_on_collide()\
          .go(POI.stock_4.x, POI.stock_4.y + 200)
    if not motion.was_successful():
        return BlockResult.FAILED

    motion.cancel_on_collide().feedrate(0.5).go(POI.stock_4.x, POI.stock_4.y)
    if not motion.was_successful():
        return BlockResult.FAILED

    log("  Collected stock_4 ✓")
    wait_ms(500)
    return BlockResult.SUCCESS


def block_banner_yellow() -> BlockResult:
    """Place banner at yellow banner point."""
    log("→ Banner yellow")

    motion.cancel_on_collide().feedrate(0.7)\
          .go(POI.bannerYellow.x, POI.bannerYellow.y - 200)
    if not motion.was_successful():
        return BlockResult.FAILED

    motion.cancel_on_collide().feedrate(0.4)\
          .go(POI.bannerYellow.x, POI.bannerYellow.y - 50)
    if not motion.was_successful():
        return BlockResult.FAILED

    log("  Banner placed ✓")
    wait_ms(300)
    return BlockResult.SUCCESS


def block_return_to_base() -> BlockResult:
    """Return to waiting zone."""
    log("→ Return to base")
    motion.feedrate(1.0).go(POI.waitPointYellow.x, POI.waitPointYellow.y)
    return BlockResult.SUCCESS if motion.was_successful() else BlockResult.FAILED


# ─────────────────────────────────────────────────────────────────────────────
#  Feasibility checks
# ─────────────────────────────────────────────────────────────────────────────

def stock_1_reachable() -> bool:
    # Could check occupancy here — for now always try
    return True

def stock_4_reachable() -> bool:
    return True


# ─────────────────────────────────────────────────────────────────────────────
#  Entry point — called by server.py when "Run" is pressed
# ─────────────────────────────────────────────────────────────────────────────

def run_mission():
    log("═══ Mission start ═══")

    safety.enable()
    motion.set_feedrate(1.0)

    m = Mission(chrono, log)
    m.set_mode(Mission.PRIORITY)
    m.set_safety_margin_ms(5000)

    m.add(Block(
        name      = "collect_stock_1",
        priority  = 10,
        score     = 80,
        time_ms   = 7000,
        action    = block_collect_stock_1,
        feasible  = stock_1_reachable,
    ))
    m.add(Block(
        name      = "collect_stock_4",
        priority  = 8,
        score     = 60,
        time_ms   = 8000,
        action    = block_collect_stock_4,
        feasible  = stock_4_reachable,
    ))
    m.add(Block(
        name      = "banner_yellow",
        priority  = 6,
        score     = 20,
        time_ms   = 5000,
        action    = block_banner_yellow,
    ))
    m.add(Block(
        name      = "return_to_base",
        priority  = 1,
        score     = 10,
        time_ms   = 5000,
        action    = block_return_to_base,
    ))

    m.run()
    log(f"═══ Mission end — {m.total_score} pts ═══")
