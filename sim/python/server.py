"""
server.py — TwinSystem Simulator
Flask + SocketIO backend.  Open http://localhost:5000 to use.

Launch:
    pip install -r requirements.txt
    python server.py
"""

import os, sys, time, math, threading, traceback, importlib.util

from flask import Flask, render_template, jsonify
from flask_socketio import SocketIO, emit

from config import (
    FIELD_W, FIELD_H, GRID_W, GRID_H, GRID_CELL,
    Vec2, Team, ObjectColor, COLOR_HEX, COLOR_BY_NAME,
    ROBOT_RADIUS, POI,
)
from sim_core  import RobotPhysics, OccupancyGrid, Pathfinder, GameObjects
from services  import MotionService, VisionService, SafetyService, ChronoService

# ─────────────────────────────────────────────────────────────────────────────
#  Flask / SocketIO
# ─────────────────────────────────────────────────────────────────────────────

app = Flask(__name__, static_folder='static', template_folder='static')
socketio = SocketIO(app, async_mode='threading', cors_allowed_origins='*')

# ─────────────────────────────────────────────────────────────────────────────
#  Simulation objects (singletons)
# ─────────────────────────────────────────────────────────────────────────────

robot     = RobotPhysics()
occupancy = OccupancyGrid()
pathfinder= Pathfinder(occupancy)
game_objs = GameObjects()

motion    = MotionService(robot, occupancy, pathfinder)
vision    = VisionService(game_objs)
safety    = SafetyService(robot, occupancy, motion)
chrono    = ChronoService()

# ─────────────────────────────────────────────────────────────────────────────
#  Simulator UI state
# ─────────────────────────────────────────────────────────────────────────────

sim_state = {
    'team':     'yellow',
    'mode':     'target',   # 'target' | 'obstacle'
    'features': {
        'collision':    True,
        'safety':       True,
        'pathfinding':  True,
        'show_grid':    False,
        'show_trail':   True,
        'show_path':    True,
    },
    'score': 0,
}

log_buffer: list[str] = []


def log(msg: str):
    ts = f"{chrono.time_elapsed_s():.1f}s"
    entry = f"[{ts}] {msg}"
    log_buffer.append(entry)
    if len(log_buffer) > 200:
        log_buffer.pop(0)
    print(entry)


# ─────────────────────────────────────────────────────────────────────────────
#  Strategy hot-reload
# ─────────────────────────────────────────────────────────────────────────────

strategy_module  = None
strategy_thread: threading.Thread | None = None
_strategy_dir    = os.path.dirname(os.path.abspath(__file__))
_strategy_path   = os.path.join(_strategy_dir, 'strategy.py')


def load_strategy() -> bool:
    global strategy_module
    try:
        spec = importlib.util.spec_from_file_location('strategy', _strategy_path)
        mod  = importlib.util.module_from_spec(spec)

        # Inject sim globals into the module namespace
        mod.motion  = motion
        mod.vision  = vision
        mod.safety  = safety
        mod.chrono  = chrono
        mod.log     = log

        spec.loader.exec_module(mod)
        strategy_module = mod
        log("strategy.py loaded ✓")
        return True
    except Exception:
        log(f"strategy.py load error:\n{traceback.format_exc()}")
        return False


def _run_strategy_thread():
    global sim_state
    if strategy_module is None:
        log("No strategy loaded")
        return
    try:
        chrono.start()
        strategy_module.run_mission()
    except Exception:
        log(f"Strategy error:\n{traceback.format_exc()}")


def start_strategy():
    global strategy_thread
    if strategy_thread and strategy_thread.is_alive():
        log("Strategy already running")
        return
    if not load_strategy():
        return
    strategy_thread = threading.Thread(
        target=_run_strategy_thread, daemon=True, name="strategy"
    )
    strategy_thread.start()


def stop_strategy():
    motion.cancel()
    log("Strategy stopped")


def _watch_strategy():
    """Background thread: reload strategy.py on file change."""
    try:
        from watchdog.observers import Observer
        from watchdog.events import FileSystemEventHandler

        class _Handler(FileSystemEventHandler):
            _last_reload = 0.0

            def on_modified(self, event):
                if 'strategy.py' not in str(event.src_path):
                    return
                now = time.time()
                if now - self._last_reload < 0.5:   # debounce
                    return
                self._last_reload = now
                time.sleep(0.1)
                if load_strategy():
                    socketio.emit('reload', {'msg': 'strategy.py reloaded ✓'})

        obs = Observer()
        obs.schedule(_Handler(), path=_strategy_dir, recursive=False)
        obs.start()
        log("Hot-reload active (watchdog)")
    except ImportError:
        log("watchdog not installed — pip install watchdog for hot-reload")


# ─────────────────────────────────────────────────────────────────────────────
#  Physics loop (60 Hz background task)
# ─────────────────────────────────────────────────────────────────────────────

_PHYSICS_DT = 1.0 / 60.0


def _physics_loop():
    _safety_timer = 0.0
    while True:
        t0 = time.perf_counter()

        # Update physics
        robot.update(_PHYSICS_DT, occupancy)
        motion.update(_PHYSICS_DT)
        chrono.update(_PHYSICS_DT)

        _safety_timer += _PHYSICS_DT
        if _safety_timer >= 0.10:
            if sim_state['features']['safety']:
                safety.update(_safety_timer)
            _safety_timer = 0.0

        # Emit state to all clients
        socketio.emit('state', _build_state())

        elapsed = time.perf_counter() - t0
        sleep   = _PHYSICS_DT - elapsed
        if sleep > 0:
            time.sleep(sleep)


def _build_state() -> dict:
    path_pts = [[p.x, p.y] for p in motion.current_path()]

    return {
        'robot':      robot.to_dict(),
        'path':       path_pts,
        'occupancy':  occupancy.to_list(),
        'dyn_obs':    [[o.x, o.y] for o in occupancy.dynamic_obstacles],
        'game_objs':  game_objs.to_list(),
        'motion':     {
            'state':    motion.state,
            'feedrate': motion.get_feedrate(),
        },
        'safety': {
            'enabled':  sim_state['features']['safety'],
            'detected': safety.obstacle_detected,
        },
        'chrono': {
            'elapsed': chrono.time_elapsed_s(),
            'left':    chrono.time_left_s(),
            'running': chrono.is_running(),
        },
        'score':    sim_state['score'],
        'team':     sim_state['team'],
        'features': sim_state['features'],
        'mode':     sim_state['mode'],
        'log':      log_buffer[-30:],
    }


# ─────────────────────────────────────────────────────────────────────────────
#  HTTP routes
# ─────────────────────────────────────────────────────────────────────────────

@app.route('/')
def index():
    return app.send_static_file('index.html')

@app.route('/api/colors')
def api_colors():
    return jsonify({c.name: COLOR_HEX[c] for c in ObjectColor})

@app.route('/api/poi')
def api_poi():
    return jsonify([{'name': n, 'x': v.x, 'y': v.y}
                    for n, v in POI.all_named()])


# ─────────────────────────────────────────────────────────────────────────────
#  SocketIO events — Client → Server
# ─────────────────────────────────────────────────────────────────────────────

@socketio.on('connect')
def on_connect():
    emit('state', _build_state())
    log("Client connected")


@socketio.on('field_click')
def on_field_click(data):
    x, y   = data['x'], data['y']
    button = data.get('button', 0)   # 0=left, 2=right
    mode   = sim_state['mode']

    if button == 2:
        # Right-click → always toggle obstacle cell
        gx = int(x // GRID_CELL)
        gy = int(y // GRID_CELL)
        occupancy.toggle_cell(gx, gy)
        log(f"Toggle cell ({gx},{gy})")

    elif mode == 'target':
        # Left-click in target mode → set manual motion target
        motion.set_manual_target(Vec2(x, y))
        log(f"Target → ({x:.0f}, {y:.0f})")

    elif mode == 'obstacle':
        # Left-click in obstacle mode → add dynamic obstacle
        occupancy.add_dynamic_obstacle(Vec2(x, y))
        log(f"Obstacle @ ({x:.0f}, {y:.0f})")

    elif mode == 'remove_obs':
        occupancy.remove_nearest_dynamic(Vec2(x, y))


@socketio.on('set_color')
def on_set_color(data):
    poi_name   = data['name']
    color_name = data['color']
    color      = COLOR_BY_NAME.get(color_name, ObjectColor.UNKNOWN)
    vision.set_color(poi_name, color)
    log(f"Color {poi_name} → {color_name}")


@socketio.on('run_strategy')
def on_run_strategy():
    start_strategy()


@socketio.on('stop_strategy')
def on_stop_strategy():
    stop_strategy()


@socketio.on('reset')
def on_reset():
    stop_strategy()
    motion.cancel()
    robot.reset_to_start(sim_state['team'])
    occupancy.reset()
    chrono.reset()
    sim_state['score'] = 0
    log_buffer.clear()
    log("Simulation reset")


@socketio.on('set_team')
def on_set_team(data):
    team = data['team']
    sim_state['team'] = team
    robot.reset_to_start(team)
    log(f"Team → {team}")


@socketio.on('set_mode')
def on_set_mode(data):
    sim_state['mode'] = data['mode']


@socketio.on('set_feature')
def on_set_feature(data):
    key = data['feature']
    val = data['value']
    sim_state['features'][key] = val
    if key == 'pathfinding':
        motion.set_pathfinding_enabled(val)
    log(f"Feature {key} → {val}")


@socketio.on('set_feedrate')
def on_set_feedrate(data):
    f = float(data['value'])
    motion.set_feedrate(f)
    log(f"Feedrate → {f:.2f}")


@socketio.on('reload_strategy')
def on_reload_strategy():
    if load_strategy():
        emit('reload', {'msg': 'strategy.py reloaded manually ✓'})


# ─────────────────────────────────────────────────────────────────────────────
#  Startup
# ─────────────────────────────────────────────────────────────────────────────

if __name__ == '__main__':
    load_strategy()
    threading.Thread(target=_watch_strategy, daemon=True).start()
    socketio.start_background_task(_physics_loop)
    print("\n  TwinSystem Simulator")
    print("  ┌────────────────────────────────┐")
    print("  │  http://localhost:5000         │")
    print("  └────────────────────────────────┘\n")
    socketio.run(app, host='0.0.0.0', port=5000, debug=False,
                 use_reloader=False, allow_unsafe_werkzeug=True)
