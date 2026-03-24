"""
config.py — TwinSystem Simulator constants, enums and POIs
Mirrors src/config/settings.h + src/config/poi.h
"""

import math
from enum import Enum
from dataclasses import dataclass

# ── Field ─────────────────────────────────────────────────────────────────────
FIELD_W          = 3000     # mm
FIELD_H          = 2000     # mm
MATCH_DURATION_S = 100.0    # seconds

# ── Robot ─────────────────────────────────────────────────────────────────────
ROBOT_RADIUS     = 125.98   # mm  (circumradius)
WHEEL_RADIUS     = 30.0     # mm

# ── Motion parameters ─────────────────────────────────────────────────────────
MAX_SPEED        = 1800.0   # mm/s
MAX_ACCEL        = 2000.0   # mm/s²
MAX_ROT_SPEED    = 10.0     # rad/s
MAX_ROT_ACCEL    = 30.0     # rad/s²
MIN_DISTANCE     = 20.0     # mm   (position tolerance)
MIN_ANGLE        = math.radians(2.0)  # rad

# ── Pathfinding / waypoints ───────────────────────────────────────────────────
WAYPOINT_RADIUS  = 80.0     # mm  (pass-through acceptance)

# ── Occupancy grid ────────────────────────────────────────────────────────────
GRID_W           = 20
GRID_H           = 13
GRID_CELL        = 150.0    # mm/cell

# ── Safety ────────────────────────────────────────────────────────────────────
SAFETY_CHECK_S      = 0.10   # s
SAFETY_RESUME_S     = 1.00   # s after last detection before resume
OBS_RADIUS          = 150.0  # mm  radius used when placing dynamic obstacles

# ── Enums ─────────────────────────────────────────────────────────────────────

class Team(Enum):
    YELLOW = "yellow"
    BLUE   = "blue"


class ObjectColor(Enum):
    UNKNOWN = 0
    NONE    = 1
    RED     = 2
    GREEN   = 3
    BLUE    = 4
    YELLOW  = 5
    WHITE   = 6
    BLACK   = 7


COLOR_HEX = {
    ObjectColor.UNKNOWN: "#888888",
    ObjectColor.NONE:    "#CCCCCC",
    ObjectColor.RED:     "#E03232",
    ObjectColor.GREEN:   "#28C228",
    ObjectColor.BLUE:    "#2828E0",
    ObjectColor.YELLOW:  "#E0C000",
    ObjectColor.WHITE:   "#F0F0F0",
    ObjectColor.BLACK:   "#222222",
}

COLOR_BY_NAME = {c.name: c for c in ObjectColor}


class RobotCompass(Enum):
    A  = 0   # BAU
    AB = 1   # Hugger / front
    B  = 2   # Screen
    BC = 3   # Empty
    C  = 4   # Pull tab / tirette
    CA = 5   # Manipulator


class TableCompass(Enum):
    NORTH = 90
    SOUTH = 270
    EAST  = 0
    WEST  = 180


class BlockResult(Enum):
    SUCCESS = 0
    FAILED  = 1


# ── 2D vector ─────────────────────────────────────────────────────────────────

@dataclass
class Vec2:
    x: float = 0.0
    y: float = 0.0

    def __add__(self, o):
        return Vec2(self.x + o.x, self.y + o.y) if isinstance(o, Vec2) else Vec2(self.x + o, self.y + o)
    def __sub__(self, o):
        return Vec2(self.x - o.x, self.y - o.y) if isinstance(o, Vec2) else Vec2(self.x - o, self.y - o)
    def __mul__(self, s): return Vec2(self.x * s, self.y * s)
    def __rmul__(self, s): return self.__mul__(s)
    def __neg__(self): return Vec2(-self.x, -self.y)
    def __iter__(self): yield self.x; yield self.y
    def __repr__(self): return f"Vec2({self.x:.1f}, {self.y:.1f})"
    def __eq__(self, o):
        if not isinstance(o, Vec2): return False
        return abs(self.x - o.x) < 0.01 and abs(self.y - o.y) < 0.01
    def __hash__(self): return hash((round(self.x, 1), round(self.y, 1)))

    def mag(self):      return math.sqrt(self.x**2 + self.y**2)
    def mag_sq(self):   return self.x**2 + self.y**2
    def dot(self, o):   return self.x * o.x + self.y * o.y
    def heading(self):  return math.atan2(self.y, self.x)
    def dist(self, o):  return (self - o).mag()

    def normalized(self):
        m = self.mag()
        return Vec2(self.x / m, self.y / m) if m > 1e-9 else Vec2(0, 0)

    def rotated(self, angle_rad: float):
        c, s = math.cos(angle_rad), math.sin(angle_rad)
        return Vec2(c * self.x - s * self.y, s * self.x + c * self.y)

    def tuple(self): return (self.x, self.y)


def polar_vec(heading_deg: float, dist: float) -> Vec2:
    """Polar → Cartesian. heading_deg=0 → East, 90 → North."""
    a = math.radians(heading_deg)
    return Vec2(math.cos(a) * dist, math.sin(a) * dist)


def angle_diff(target: float, current: float) -> float:
    """Shortest signed angle difference, in [-π, π]."""
    d = (target - current) % (2 * math.pi)
    if d > math.pi: d -= 2 * math.pi
    return d


def compass_deg(direction) -> float:
    """Get angle in degrees for a TableCompass or RobotCompass."""
    if isinstance(direction, TableCompass): return float(direction.value)
    if isinstance(direction, RobotCompass): return direction.value * 60.0
    return float(direction)


def robot_compass_offset_deg(rc: RobotCompass) -> float:
    """Offset in degrees of a robot compass side from heading=0."""
    return rc.value * 60.0


# ── Points of Interest ────────────────────────────────────────────────────────

class POI:
    # Departure zones - Yellow
    y1 = Vec2(375,  225)
    y2 = Vec2(1225, 1825)
    y3 = Vec2(2775, 1125)

    # Departure zones - Blue
    b1 = Vec2(2625, 225)
    b2 = Vec2(1775, 1775)
    b3 = Vec2(225,  1125)

    # Construction areas - Yellow
    constAreaYellow_1      = Vec2(775,  2000)
    constAreaYellow_2      = Vec2(1300, 1850)
    constAreaYellowBelow_2 = Vec2(1300, 1650)
    constAreaYellow_3      = Vec2(2800, 1125)
    constAreaYellow_4      = Vec2(2775, 1800)

    # Construction areas - Blue
    constAreaBlue_1        = Vec2(2225, 2000)
    constAreaBlue_2        = Vec2(1700, 1850)
    constAreaBlueBelow_2   = Vec2(1700, 1650)
    constAreaBlue_3        = Vec2(200,  1125)
    constAreaBlue_4        = Vec2(225,  1800)

    # Stock locations (raw material)
    stock_1 = Vec2(75,   675)
    stock_2 = Vec2(75,   1600)
    stock_3 = Vec2(765,  1710)
    stock_4 = Vec2(1100, 1050)
    stock_5 = Vec2(1900, 1050)
    stock_6 = Vec2(2225, 1750)
    stock_7 = Vec2(2925, 1600)
    stock_8 = Vec2(2925, 675)

    # Reserved material stock
    reservedYellow = Vec2(825,  275)
    reservedBlue   = Vec2(2175, 275)

    # Waiting points
    waitPointYellowTemp = Vec2(800,  900)
    waitPointYellow     = Vec2(375,  675)
    waitPointBlueTemp   = Vec2(2200, 900)
    waitPointBlue       = Vec2(2625, 675)

    # Banner promotion points
    bannerYellow = Vec2(1225, 1894)
    bannerBlue   = Vec2(1775, 1894)

    # Path waypoints
    yellowWaypoint_1 = Vec2(1100, 1300)
    yellowWaypoint_2 = Vec2(775,  1300)
    blueWaypoint_1   = Vec2(1900, 1300)
    blueWaypoint_2   = Vec2(2225, 1300)

    @classmethod
    def all_named(cls) -> list:
        return [(k, v) for k, v in vars(cls).items()
                if isinstance(v, Vec2) and not k.startswith('_')]

    @classmethod
    def all_stocks(cls) -> list:
        return [cls.stock_1, cls.stock_2, cls.stock_3, cls.stock_4,
                cls.stock_5, cls.stock_6, cls.stock_7, cls.stock_8]


# ── Starting positions per team ───────────────────────────────────────────────

TEAM_START = {
    Team.YELLOW: (POI.y1, 0.0),              # (position, theta_rad)
    Team.BLUE:   (POI.b1, math.pi),
}
