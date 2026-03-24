/**
 * app.js — TwinSystem Simulator front-end
 * Canvas rendering + SocketIO client
 */

'use strict';

// ── Constants ──────────────────────────────────────────────────────────────
const FIELD_W = 3000;
const FIELD_H = 2000;
const GRID_W  = 20;
const GRID_H  = 13;
const GRID_CELL = 150;

const COLOR_HEX = {
  UNKNOWN: '#888888', NONE: '#cccccc',
  RED:     '#e03232', GREEN: '#28c228',
  BLUE:    '#2828e0', YELLOW: '#e0c000',
  WHITE:   '#f0f0f0', BLACK:  '#222222',
};
const ALL_COLORS = Object.keys(COLOR_HEX);

const MOTION_STATE_CLASS = {
  IDLE:      'state-idle',
  RUNNING:   'state-running',
  PAUSED:    'state-paused',
  CANCELED:  'state-canceled',
  COMPLETED: 'state-completed',
};

// ── State ──────────────────────────────────────────────────────────────────
let lastState   = null;
let colorPanelOpen = true;
let activePoi   = null;   // for color popup
let scale       = 1;      // px per mm
let offX = 0, offY = 0;   // canvas field offset (px)

// ── Canvas setup ───────────────────────────────────────────────────────────
const canvas = document.getElementById('field-canvas');
const ctx    = canvas.getContext('2d');

function resizeCanvas() {
  const section = document.getElementById('field-section');
  const modeBar = document.getElementById('mode-bar');
  const coordBar = document.getElementById('coord-display');
  const W = section.clientWidth;
  const H = section.clientHeight - modeBar.offsetHeight - coordBar.offsetHeight;

  canvas.width  = W;
  canvas.height = H;

  // Fit field keeping aspect ratio with small padding
  const pad = 12;
  const sw = (W - pad * 2) / FIELD_W;
  const sh = (H - pad * 2) / FIELD_H;
  scale = Math.min(sw, sh);

  offX = (W  - FIELD_W * scale) / 2;
  offY = (H  - FIELD_H * scale) / 2;

  if (lastState) render(lastState);
}
window.addEventListener('resize', resizeCanvas);

// ── Coordinate helpers ────────────────────────────────────────────────────
function wx(mm)  { return offX + mm * scale; }        // world x → canvas x
function wy(mm)  { return offY + (FIELD_H - mm) * scale; }  // world y → canvas y (flipped)
function wlen(mm){ return mm * scale; }

function canvasToWorld(cx, cy) {
  return {
    x: (cx - offX) / scale,
    y: FIELD_H - (cy - offY) / scale,
  };
}

// ── SocketIO ──────────────────────────────────────────────────────────────
const socket = io();

socket.on('connect',    () => console.log('Connected'));
socket.on('disconnect', () => console.log('Disconnected'));

socket.on('state', state => {
  lastState = state;
  render(state);
  updateSidebar(state);
});

socket.on('reload', data => showToast(data.msg));

// ── Rendering ─────────────────────────────────────────────────────────────
function render(s) {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  drawField(s);
  if (s.features.show_grid)  drawGrid(s);
  if (s.features.show_trail) drawTrail(s);
  if (s.features.show_path)  drawPath(s);
  drawDynObs(s);
  drawGameObjects(s);
  drawPOIs();
  drawRobot(s);
}

// ── Field background ──────────────────────────────────────────────────────
function drawField(s) {
  // Field bg
  ctx.fillStyle = '#1a2a1a';
  ctx.fillRect(wx(0), wy(FIELD_H), wlen(FIELD_W), wlen(FIELD_H));

  // Team zones (transparent overlays)
  ctx.fillStyle = 'rgba(224,192,0,.07)';
  ctx.fillRect(wx(0), wy(FIELD_H), wlen(1500), wlen(FIELD_H));

  ctx.fillStyle = 'rgba(40,40,224,.07)';
  ctx.fillRect(wx(1500), wy(FIELD_H), wlen(1500), wlen(FIELD_H));

  // Center divider
  ctx.strokeStyle = 'rgba(255,255,255,.12)';
  ctx.lineWidth   = 1;
  ctx.setLineDash([4, 4]);
  ctx.beginPath();
  ctx.moveTo(wx(1500), wy(0));
  ctx.lineTo(wx(1500), wy(FIELD_H));
  ctx.stroke();
  ctx.setLineDash([]);

  // Field border
  ctx.strokeStyle = '#ffffff';
  ctx.lineWidth   = Math.max(1.5, scale * 6);
  ctx.strokeRect(wx(0), wy(FIELD_H), wlen(FIELD_W), wlen(FIELD_H));

  // Grid lines every 500mm (light)
  ctx.strokeStyle = 'rgba(255,255,255,.06)';
  ctx.lineWidth   = .5;
  for (let x = 500; x < FIELD_W; x += 500) {
    ctx.beginPath(); ctx.moveTo(wx(x), wy(0)); ctx.lineTo(wx(x), wy(FIELD_H)); ctx.stroke();
  }
  for (let y = 500; y < FIELD_H; y += 500) {
    ctx.beginPath(); ctx.moveTo(wx(0), wy(y)); ctx.lineTo(wx(FIELD_W), wy(y)); ctx.stroke();
  }

  // Construction areas outlines
  const constructAreas = [
    [775,2000,  425,2000, '#e0c000'],
    [2225,2000,2775,2000, '#1e88e5'],
  ];
  // (simplified — draw a box near top edge for each team)
  ctx.strokeStyle = 'rgba(224,192,0,.35)'; ctx.lineWidth = 1;
  ctx.strokeRect(wx(575), wy(FIELD_H), wlen(550), wlen(100));
  ctx.strokeStyle = 'rgba(40,40,224,.35)';
  ctx.strokeRect(wx(1875), wy(FIELD_H), wlen(550), wlen(100));
}

// ── Occupancy grid overlay ────────────────────────────────────────────────
function drawGrid(s) {
  // Grid lines
  ctx.strokeStyle = 'rgba(100,180,255,.18)';
  ctx.lineWidth   = .5;
  for (let gx = 0; gx <= GRID_W; gx++) {
    ctx.beginPath(); ctx.moveTo(wx(gx*GRID_CELL), wy(0));
    ctx.lineTo(wx(gx*GRID_CELL), wy(FIELD_H)); ctx.stroke();
  }
  for (let gy = 0; gy <= GRID_H; gy++) {
    ctx.beginPath(); ctx.moveTo(wx(0), wy(gy*GRID_CELL));
    ctx.lineTo(wx(FIELD_W), wy(gy*GRID_CELL)); ctx.stroke();
  }

  // Occupied cells
  ctx.fillStyle = 'rgba(233,69,96,.45)';
  for (const cell of s.occupancy) {
    const gx = cell.gx, gy = cell.gy;
    ctx.fillRect(
      wx(gx * GRID_CELL) + 1, wy((gy+1) * GRID_CELL) + 1,
      wlen(GRID_CELL) - 2,     wlen(GRID_CELL) - 2
    );
  }
}

// ── Dynamic obstacles ─────────────────────────────────────────────────────
function drawDynObs(s) {
  for (const o of s.dyn_obs) {
    ctx.beginPath();
    ctx.arc(wx(o[0]), wy(o[1]), wlen(150), 0, Math.PI * 2);
    ctx.fillStyle   = 'rgba(233,69,96,.35)';
    ctx.fill();
    ctx.strokeStyle = '#e94560';
    ctx.lineWidth   = 1.5;
    ctx.stroke();
  }
}

// ── Path ──────────────────────────────────────────────────────────────────
function drawPath(s) {
  if (!s.path || s.path.length < 2) return;
  ctx.strokeStyle = 'rgba(0,180,216,.7)';
  ctx.lineWidth   = 2;
  ctx.setLineDash([6, 4]);
  ctx.beginPath();
  ctx.moveTo(wx(s.path[0][0]), wy(s.path[0][1]));
  for (let i = 1; i < s.path.length; i++) {
    ctx.lineTo(wx(s.path[i][0]), wy(s.path[i][1]));
  }
  ctx.stroke();

  // Waypoint diamonds
  for (let i = 1; i < s.path.length - 1; i++) {
    const px = wx(s.path[i][0]), py = wy(s.path[i][1]);
    const d  = wlen(8);
    ctx.fillStyle = 'rgba(0,180,216,.8)';
    ctx.beginPath();
    ctx.moveTo(px, py - d); ctx.lineTo(px + d, py);
    ctx.lineTo(px, py + d); ctx.lineTo(px - d, py);
    ctx.closePath(); ctx.fill();
  }
  ctx.setLineDash([]);
}

// ── Robot trail ───────────────────────────────────────────────────────────
function drawTrail(s) {
  const trail = s.robot.trail;
  if (!trail || trail.length < 2) return;
  for (let i = 1; i < trail.length; i++) {
    const alpha = i / trail.length;
    ctx.strokeStyle = `rgba(0,180,216,${alpha * .4})`;
    ctx.lineWidth   = 1.5;
    ctx.beginPath();
    ctx.moveTo(wx(trail[i-1][0]), wy(trail[i-1][1]));
    ctx.lineTo(wx(trail[i][0]),   wy(trail[i][1]));
    ctx.stroke();
  }
}

// ── Robot ─────────────────────────────────────────────────────────────────
function drawRobot(s) {
  const r   = s.robot;
  const cx  = wx(r.x);
  const cy  = wy(r.y);
  const rad = wlen(r.radius);
  const th  = -r.theta;   // canvas Y is flipped

  // Collision radius circle
  ctx.beginPath();
  ctx.arc(cx, cy, rad, 0, Math.PI * 2);
  ctx.strokeStyle = r.collided ? 'rgba(233,69,96,.8)' : 'rgba(255,255,255,.2)';
  ctx.lineWidth   = 1.5;
  ctx.stroke();

  // Body (equilateral triangle)
  ctx.save();
  ctx.translate(cx, cy);
  ctx.rotate(th);
  const rs = rad * 0.85;
  ctx.beginPath();
  ctx.moveTo(rs, 0);
  ctx.lineTo(-rs * .6,  rs * .87);
  ctx.lineTo(-rs * .6, -rs * .87);
  ctx.closePath();
  const teamColor = s.team === 'yellow' ? '#e0c000' : '#1e88e5';
  ctx.fillStyle   = r.collided ? '#e94560' : teamColor;
  ctx.fill();
  ctx.strokeStyle = 'rgba(255,255,255,.6)';
  ctx.lineWidth   = 1.5;
  ctx.stroke();

  // Heading arrow
  ctx.strokeStyle = '#ffffff';
  ctx.lineWidth   = 2;
  ctx.beginPath();
  ctx.moveTo(rs * .3, 0);
  ctx.lineTo(rs * .95, 0);
  const ar = rs * .15;
  ctx.lineTo(rs * .85,  ar * .5);
  ctx.moveTo(rs * .95, 0);
  ctx.lineTo(rs * .85, -ar * .5);
  ctx.stroke();

  ctx.restore();

  // Safety indicator ring
  if (s.safety.enabled && s.safety.detected) {
    ctx.beginPath();
    ctx.arc(cx, cy, rad + wlen(15), 0, Math.PI * 2);
    ctx.strokeStyle = 'rgba(255,180,0,.7)';
    ctx.lineWidth   = 3;
    ctx.setLineDash([5, 4]);
    ctx.stroke();
    ctx.setLineDash([]);
  }
}

// ── POI markers ───────────────────────────────────────────────────────────
const POI_COLORS = {
  stock:      '#ccaaff',
  constArea:  '#aaddff',
  waitPoint:  '#aaffcc',
  banner:     '#ffcc44',
  waypoint:   '#777777',
  y1: '#e0c000', y2: '#e0c000', y3: '#e0c000',
  b1: '#1e88e5', b2: '#1e88e5', b3: '#1e88e5',
  reservedYellow: '#e0c000', reservedBlue: '#1e88e5',
};

function poiColor(name) {
  if (name.startsWith('stock'))     return POI_COLORS.stock;
  if (name.startsWith('constArea')) return POI_COLORS.constArea;
  if (name.startsWith('waitPoint')) return POI_COLORS.waitPoint;
  if (name.startsWith('banner'))    return POI_COLORS.banner;
  if (name.startsWith('Waypoint') || name.includes('Waypoint')) return POI_COLORS.waypoint;
  return POI_COLORS[name] || '#888';
}

let poiData = [];
fetch('/api/poi').then(r => r.json()).then(d => { poiData = d; });

function drawPOIs() {
  for (const poi of poiData) {
    const px = wx(poi.x), py = wy(poi.y);
    const r  = wlen(8);
    ctx.beginPath();
    ctx.arc(px, py, r, 0, Math.PI * 2);
    ctx.fillStyle   = poiColor(poi.name) + '33';
    ctx.fill();
    ctx.strokeStyle = poiColor(poi.name) + 'aa';
    ctx.lineWidth   = 1;
    ctx.stroke();

    if (scale > 0.12) {
      ctx.fillStyle = '#aaaacc';
      ctx.font      = `${Math.max(8, wlen(22))}px monospace`;
      ctx.textAlign = 'center';
      ctx.fillText(poi.name.replace('constArea','⬛').replace('stock','S')
                          .replace('waitPoint','W').replace('banner','🚩')
                          .replace('Waypoint','→'), px, py - r - 2);
    }
  }
}

// ── Game objects (colored) ────────────────────────────────────────────────
function drawGameObjects(s) {
  for (const obj of s.game_objs) {
    if (obj.color === 'UNKNOWN' || obj.color === 'NONE') continue;
    const hex  = COLOR_HEX[obj.color] || '#888';
    const px   = wx(obj.x), py = wy(obj.y);
    const r    = wlen(25);
    ctx.beginPath();
    ctx.arc(px, py, r, 0, Math.PI * 2);
    ctx.fillStyle   = hex + 'cc';
    ctx.fill();
    ctx.strokeStyle = hex;
    ctx.lineWidth   = 2;
    ctx.stroke();
  }
}

// ── Sidebar ───────────────────────────────────────────────────────────────
let prevLog = [];

function updateSidebar(s) {
  const r = s.robot;

  document.getElementById('s-pos').textContent =
    `(${r.x.toFixed(0)}, ${r.y.toFixed(0)}) mm`;
  document.getElementById('s-theta').textContent =
    `${(r.theta * 180 / Math.PI).toFixed(1)}°`;
  document.getElementById('s-speed').textContent =
    `${Math.hypot(r.vx, r.vy).toFixed(0)} mm/s`;

  const mEl = document.getElementById('s-mstate');
  mEl.textContent = s.motion.state;
  mEl.className   = 'value ' + (MOTION_STATE_CLASS[s.motion.state] || '');

  // Chrono
  const left    = s.chrono.left;
  const elapsed = s.chrono.elapsed;
  const total   = left + elapsed;
  document.getElementById('s-time').textContent = `${left.toFixed(1)} s`;
  document.getElementById('s-score').textContent = `${s.score} pts`;

  const pct = total > 0 ? (left / total * 100).toFixed(1) : 100;
  const bar = document.getElementById('chrono-bar');
  bar.style.width = `${pct}%`;
  bar.style.background = left < 10 ? '#e94560' : left < 30 ? '#fdd835' : '#00b4d8';

  // Log
  if (s.log.length !== prevLog.length ||
      (s.log.length && s.log[s.log.length-1] !== prevLog[prevLog.length-1])) {
    prevLog = s.log;
    const logEl = document.getElementById('log-output');
    logEl.innerHTML = [...s.log].reverse().map(l => {
      let cls = '';
      if (l.includes('✓') || l.includes('SUCCESS') || l.includes('loaded ✓')) cls = 'ok';
      else if (l.includes('✗') || l.includes('FAILED') || l.includes('error')) cls = 'err';
      else if (l.includes('Stall') || l.includes('Collision') || l.includes('warn')) cls = 'warn';
      return `<div class="log-line ${cls}">${escHtml(l)}</div>`;
    }).join('');
  }

  // Color panel (only when data changes)
  updateColorPanel(s.game_objs);
}

let prevObjs = '[]';
function updateColorPanel(objs) {
  const key = JSON.stringify(objs.map(o => o.color));
  if (key === prevObjs) return;
  prevObjs = key;

  const list = document.getElementById('color-list');
  list.innerHTML = objs.map(o => `
    <div class="color-item" onclick="openColorPopup(event, '${o.name}')">
      <div class="color-dot" style="background:${COLOR_HEX[o.color] || '#888'};
           border-color:${o.color !== 'UNKNOWN' ? COLOR_HEX[o.color] : '#555'}80"></div>
      <span class="color-name">${o.name.replace('stock_','S').replace('constArea','C')
                                      .replace('waitPoint','W').replace('banner','B')
                                      .replace('Yellow','Y').replace('Blue','B')}</span>
    </div>`).join('');
}

// ── Color popup ───────────────────────────────────────────────────────────
function openColorPopup(e, poiName) {
  e.stopPropagation();
  activePoi = poiName;
  const popup = document.getElementById('color-popup');
  const choices = document.getElementById('color-choices');
  document.getElementById('color-popup-title').textContent = `Color: ${poiName}`;

  choices.innerHTML = ALL_COLORS.map(c =>
    `<div class="color-choice" onclick="applyColor('${c}')">
       <div class="color-dot" style="background:${COLOR_HEX[c]}"></div>
       <span>${c}</span>
     </div>`
  ).join('');

  const rect = e.currentTarget.getBoundingClientRect();
  popup.style.left = `${rect.right + 6}px`;
  popup.style.top  = `${Math.min(rect.top, window.innerHeight - 260)}px`;
  popup.classList.remove('hidden');
}

function applyColor(colorName) {
  if (!activePoi) return;
  socket.emit('set_color', { name: activePoi, color: colorName });
  document.getElementById('color-popup').classList.add('hidden');
  activePoi = null;
}

document.addEventListener('click', () => {
  document.getElementById('color-popup').classList.add('hidden');
  activePoi = null;
});

// ── Canvas interaction ────────────────────────────────────────────────────
canvas.addEventListener('mousemove', e => {
  const rect  = canvas.getBoundingClientRect();
  const world = canvasToWorld(e.clientX - rect.left, e.clientY - rect.top);
  const inField = world.x >= 0 && world.x <= FIELD_W && world.y >= 0 && world.y <= FIELD_H;
  document.getElementById('coord-display').textContent =
    inField ? `x: ${world.x.toFixed(0)} mm   y: ${world.y.toFixed(0)} mm` : 'x:— y:—';
});

canvas.addEventListener('click', e => {
  const rect  = canvas.getBoundingClientRect();
  const world = canvasToWorld(e.clientX - rect.left, e.clientY - rect.top);
  if (world.x < 0 || world.x > FIELD_W || world.y < 0 || world.y > FIELD_H) return;
  socket.emit('field_click', { x: world.x, y: world.y, button: 0 });
});

canvas.addEventListener('contextmenu', e => {
  e.preventDefault();
  const rect  = canvas.getBoundingClientRect();
  const world = canvasToWorld(e.clientX - rect.left, e.clientY - rect.top);
  if (world.x < 0 || world.x > FIELD_W || world.y < 0 || world.y > FIELD_H) return;
  socket.emit('field_click', { x: world.x, y: world.y, button: 2 });
});

// ── Controls ──────────────────────────────────────────────────────────────
function runStrategy()    { socket.emit('run_strategy'); }
function stopStrategy()   { socket.emit('stop_strategy'); }
function resetSim()       { socket.emit('reset'); }
function reloadStrategy() { socket.emit('reload_strategy'); }

function setTeam(team) {
  socket.emit('set_team', { team });
  document.getElementById('btn-yellow').classList.toggle('active', team === 'yellow');
  document.getElementById('btn-blue').classList.toggle('active',   team === 'blue');
}

function setMode(mode)    { socket.emit('set_mode', { mode }); }
function setFeature(k, v) { socket.emit('set_feature', { feature: k, value: v }); }

function setFeedrate(v) {
  document.getElementById('feedrate-val').textContent = parseFloat(v).toFixed(2);
  socket.emit('set_feedrate', { value: parseFloat(v) });
}

function toggleColorPanel() {
  colorPanelOpen = !colorPanelOpen;
  document.getElementById('color-list').style.display =
    colorPanelOpen ? '' : 'none';
}

function clearLog() {
  prevLog = [];
  document.getElementById('log-output').innerHTML = '';
}

// ── Toast ─────────────────────────────────────────────────────────────────
function showToast(msg) {
  const t = document.getElementById('reload-toast');
  t.textContent = msg;
  t.classList.remove('hidden');
  t.style.opacity = '1';
  setTimeout(() => {
    t.style.opacity = '0';
    setTimeout(() => t.classList.add('hidden'), 400);
  }, 2000);
}

// ── Utility ───────────────────────────────────────────────────────────────
function escHtml(s) {
  return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
}

// ── Init ──────────────────────────────────────────────────────────────────
window.addEventListener('load', () => {
  resizeCanvas();
});
