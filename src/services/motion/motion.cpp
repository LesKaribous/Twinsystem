#include "motion.h"
#include "kinematics.h"
#include "config/settings.h"
#include "os/console.h"
#include "utils/timer/timer.h"
#include "services/localisation/localisation.h"
#include <cmath>

SINGLETON_INSTANTIATE(Motion, motion)

// ============================================================
//  Construction / attach
// ============================================================

Motion::Motion()
    : Service(ID_MOTION),
      m_sA(Pin::Stepper::stepA, Pin::Stepper::dirA, !Settings::Stepper::DIR_A_POLARITY),
      m_sB(Pin::Stepper::stepB, Pin::Stepper::dirB, !Settings::Stepper::DIR_B_POLARITY),
      m_sC(Pin::Stepper::stepC, Pin::Stepper::dirC, !Settings::Stepper::DIR_C_POLARITY)
{
    disengage();
}

FLASHMEM void Motion::attach() {
    Console::info("Motion") << "Motion attached" << Console::endl;

    Job::reset();
    stepper_controller.setSteppers(&m_sA, &m_sB, &m_sC);
    cruise_controller.setSteppers(&m_sA, &m_sB, &m_sC);

    _absolute      = Settings::Motion::ABSOLUTE;
    _startPosition = {0, 0, 0};
    _position      = {0, 0, 0};
    _target        = {0, 0, 0};
    _controlPoint  = {0, 0};
    clearWaypoints();

    pinMode(Pin::Stepper::enable, OUTPUT);
}


// ============================================================
//  Service::run — machine d'état principale
// ============================================================

void Motion::run() {
    if (!enabled()) return;
    if      (isPausing())   onPausing();
    else if (isCanceling()) onCanceling();
    else if (isRunning())   onRunning();
}

void Motion::exec() { run(); }


// ============================================================
//  États en cours
// ============================================================

void Motion::onRunning() {
    if (!isBusy()) return;

    // Annulation venue du contrôleur (collision, obstacle…)
    bool controllerCanceled = false;
    if ( current_move_cruised) controllerCanceled = cruise_controller.isCanceled();
    if (!current_move_cruised) controllerCanceled = stepper_controller.isCanceled();

    if (controllerCanceled) {
        Console::warn("Motion") << "Move canceled by controller" << Console::endl;
        clearWaypoints();
        onCanceled();
        return;
    }

    // Détection de collision → cancel si l'option est active
    if (current_move_cruised && m_activeOpts.cancelOnCollide && cruise_controller.collision()) {
        Console::warn("Motion") << "Collision detected — canceling" << Console::endl;
        clearWaypoints();
        cruise_controller.cancel();
        return;
    }

    // Pass-through : si waypoint intermédiaire et robot suffisamment proche, enchaîner
    bool passThrough = (m_waypointIndex < m_waypointCount - 1)
                    && (m_waypoints[m_waypointIndex].passThrough);

    if (passThrough) {
        float distToWp = Vec2(_target - _position).mag();
        if (distToWp < WAYPOINT_RADIUS) {
            if (!advanceWaypoint()) {
                startWaypoint(m_waypoints[m_waypointIndex]);
            } else {
                complete();
            }
            return;
        }
    }

    // Fin normale du move courant
    if (hasFinished()) {
        if (!advanceWaypoint()) {
            // Il reste des waypoints → lancer le suivant
            startWaypoint(m_waypoints[m_waypointIndex]);
        } else {
            // Tous les waypoints traités
            complete();
        }
        return;
    }

    // Tick du contrôleur stepper (le cruise est piloté par son propre cycle ISR)
    if (!current_move_cruised) stepper_controller.exec();
}

void Motion::onPausing() {
    bool finished = false;

    if (current_move_cruised) {
        Vec3 pos = estimatedPosition();
        if (pos != _position) {
            _position = pos;
            cruise_controller.setPosition(pos);
        }
        cruise_controller.exec();
        finished = cruise_controller.isCanceled();
    } else {
        stepper_controller.exec();
        finished = stepper_controller.isCanceled();
    }

    if (finished) onPaused();
}

void Motion::onCanceling() {
    bool finished = false;

    if (current_move_cruised) {
        Vec3 pos = estimatedPosition();
        if (pos != _position) {
            _position = pos;
            cruise_controller.setPosition(pos);
        }
        cruise_controller.exec();
        finished = cruise_controller.isCompleted();
    } else {
        stepper_controller.exec();
        finished = stepper_controller.isCompleted();
    }

    if (finished) onCanceled();
}

void Motion::onPaused() {
    Job::onPaused();
    _isMoving   = false;
    _isRotating = false;
    Console::info("Motion") << "Move paused" << Console::endl;

    cruise_controller.reset();
    stepper_controller.reset();
    _startPosition = _position = estimatedPosition();
}

void Motion::onCanceled() {
    Job::onCanceled();
    _isMoving   = false;
    _isRotating = false;

    cruise_controller.reset();
    stepper_controller.reset();
    _startPosition = _position = estimatedPosition();

    Console::info("Motion") << "Move canceled" << Console::endl;
}


// ============================================================
//  Cycle ISR
// ============================================================

void Motion::step() {
    SERVICE_METHOD_HEADER
    if (current_move_cruised) cruise_controller.step();
    else                      stepper_controller.step();
}

void Motion::control() {
    SERVICE_METHOD_HEADER
    if (current_move_cruised) cruise_controller.control();
    else                      stepper_controller.control();
}


// ============================================================
//  Fluent — options pour le prochain move
// ============================================================

Motion& Motion::noCollide() {
    m_pendingOpts.collisionEnabled = false;
    return *this;
}

Motion& Motion::withCollision(bool on) {
    m_pendingOpts.collisionEnabled = on;
    return *this;
}

Motion& Motion::cancelOnCollide(bool on) {
    m_pendingOpts.cancelOnCollide = on;
    return *this;
}

Motion& Motion::withOptimization(bool on) {
    m_pendingOpts.optimizeRotation = on;
    return *this;
}

Motion& Motion::feedrate(float f) {
    m_pendingOpts.feedrate = std::max(std::min(f, 1.0f), 0.05f);
    return *this;
}


// ============================================================
//  Waypoints
// ============================================================

Motion& Motion::via(float x, float y) {
    return via(Vec2(x, y));
}

Motion& Motion::via(Vec2 wp) {
    // Un via() est toujours pass-through et hérite des options pending courantes
    Vec3 target3(_absolute ? wp.a : wp.a,
                 _absolute ? wp.b : wp.b,
                 _absolute ? _position.c * RAD_TO_DEG : 0.0f);
    enqueueWaypoint(target3, true);
    return *this;
}

void Motion::enqueueWaypoint(Vec3 target, bool passThrough) {
    if (m_waypointCount >= WAYPOINT_CAPACITY) {
        Console::error("Motion") << "Waypoint queue full" << Console::endl;
        return;
    }
    m_waypoints[m_waypointCount] = { target, passThrough, m_pendingOpts };
    m_waypointCount++;
}

// Avance l'index. Retourne true si la queue est épuisée.
bool Motion::advanceWaypoint() {
    m_waypointIndex++;
    return (m_waypointIndex >= m_waypointCount);
}

void Motion::clearWaypoints() {
    m_waypointCount = 0;
    m_waypointIndex = 0;
}


// ============================================================
//  API publique — mouvements
// ============================================================

Motion& Motion::go(float x, float y) {
    _isMoving = true;
    return go(Vec2(x, y));
}

Motion& Motion::go(Vec2 target) {
    _isMoving = true;
    Vec3 t3 = _absolute ? Vec3(target.a, target.b, _position.c * RAD_TO_DEG)
                        : Vec3(target.a, target.b, 0.0f);
    enqueueWaypoint(t3, false);  // point final : pas pass-through
    // Lancer le premier waypoint
    startWaypoint(m_waypoints[0]);
    return *this;
}

Motion& Motion::goPolar(float heading, float dist) {
    _isMoving = true;
    PolarVec pv(heading * DEG_TO_RAD, dist);
    Vec3 t3;
    if (_absolute) {
        Vec2 t = _position + pv.toVec2();
        t3 = Vec3(t.a, t.b, _position.c * RAD_TO_DEG);
    } else {
        Vec2 t = pv.toVec2();
        t3 = Vec3(t.a, t.b, 0.0f);
    }
    enqueueWaypoint(t3, false);
    startWaypoint(m_waypoints[0]);
    return *this;
}

Motion& Motion::goPolarAlign(float heading, float dist, RobotCompass rc, float orientation) {
    _isMoving = true;
    PolarVec pv(heading * DEG_TO_RAD, dist);
    float angle = orientation - getCompassOrientation(rc);
    Vec3 t3;
    if (_absolute) {
        Vec2 t = _position + pv.toVec2();
        t3 = Vec3(t.a, t.b, angle);
    } else {
        Vec2 t = pv.toVec2();
        t3 = Vec3(t.a, t.b, angle);
    }
    enqueueWaypoint(t3, false);
    startWaypoint(m_waypoints[0]);
    return *this;
}

Motion& Motion::turn(float angle) {
    _isMoving   = true;
    _isRotating = true;
    Vec3 t3 = _absolute ? Vec3(_position.a, _position.b, angle)
                        : Vec3(0.0f, 0.0f, angle);
    enqueueWaypoint(t3, false);
    startWaypoint(m_waypoints[0]);
    return *this;
}

Motion& Motion::align(RobotCompass rc, float orientation) {
    return turn(orientation - getCompassOrientation(rc));
}

Motion& Motion::goAlign(Vec2 target, RobotCompass rc, float orientation) {
    _isMoving   = true;
    _isRotating = true;
    float angle = orientation - getCompassOrientation(rc);
    enqueueWaypoint(Vec3(target.a, target.b, angle), false);
    startWaypoint(m_waypoints[0]);
    return *this;
}

// move() : commande bas niveau — ne passe plus par la queue, applique directement.
// Utilisé en interne par startWaypoint().
Motion& Motion::move(Vec3 target) {
    if (!enabled()) {
        Console::error("Motion") << "Motion not enabled" << Console::endl;
        return *this;
    }

    if (isRunning()) cancel();
    Job::reset();
    engage();

    target.c *= DEG_TO_RAD;

    if (isRelative()) {
        if (target.magSq() == 0.0f) {
            Console::error("Motion") << "Move is null" << Console::endl;
            complete();
            return *this;
        }
        target = toAbsoluteTarget(target);
    } else {
        if (target == _position) {
            Console::error("Motion") << "Move is null" << Console::endl;
            complete();
            return *this;
        }
    }

    _target = target;
    Console::info("Motion") << "Target: " << _target << "  Pos: " << _position << Console::endl;

    // Résoudre le feedrate effectif (per-move override ou global)
    float effectiveFeedrate = (m_activeOpts.feedrate > 0.0f) ? m_activeOpts.feedrate : m_feedrate;

    Vec3 relTarget = toRelativeTarget(_target);

    cruise_controller.reset();
    stepper_controller.reset();

    // Bump detection désactivée pour les rotations pures, et selon l'option du move
    bool pureRotation = _isRotating
        && fabsf(_target.x - _position.x) < Settings::Motion::MIN_DISTANCE
        && fabsf(_target.y - _position.y) < Settings::Motion::MIN_DISTANCE;
    cruise_controller.setCollisionEnabled(m_activeOpts.collisionEnabled && !pureRotation);

    if (use_cruise_mode && localisation.enabled()) {
        float feed = isRotating() ? effectiveFeedrate * 0.5f : effectiveFeedrate;
        cruise_controller.setFeedrate(feed);
        cruise_controller.setPosition(_position);
        cruise_controller.setTarget(_target);
        current_move_cruised = true;
    } else {
        if (m_activeOpts.optimizeRotation) relTarget = optimizeRelTarget(relTarget);
        Vec3 steps = ik(relTarget);
        Console::info("Motion") << "Stepper move: steps=" << steps << Console::endl;
        float feed = isRotating() ? effectiveFeedrate * 0.5f : effectiveFeedrate;
        stepper_controller.setFeedrate(feed);
        stepper_controller.setTarget(steps.a, steps.b, steps.c);
        current_move_cruised = false;
    }

    start();
    return *this;
}

// startWaypoint() : extrait les options du waypoint, les installe en m_activeOpts,
//                   puis lance move().
void Motion::startWaypoint(const Waypoint& wp) {
    m_activeOpts = wp.opts;
    // Pour les via() les options pending ont déjà été capturées à l'enqueue.
    // Les options pending sont consommées ici et resetées pour le prochain move.
    m_pendingOpts = MoveOptions{};  // reset aux defaults

    move(wp.target);
}


// ============================================================
//  start / pause / resume / cancel / complete
// ============================================================

void Motion::start() {
    if (isPending()) return;
    Job::start();

    const char* mode = current_move_cruised ? "(cruise)" : "(stepper)";

    if (m_async) {
        if (current_move_cruised) cruise_controller.start();
        else                      stepper_controller.start();
        Console::info("Motion") << "Start " << mode << Console::endl;
    } else {
        if (current_move_cruised) {
            cruise_controller.start();
            Console::info("Motion") << "Start sync " << mode << Console::endl;
            while (cruise_controller.isBusy()) cruise_controller.exec();
        } else {
            stepper_controller.start();
            Console::info("Motion") << "Start sync " << mode << Console::endl;
            while (stepper_controller.isBusy()) stepper_controller.exec();
        }
        complete();
    }
}

void Motion::pause() {
    Job::pause();
    if (!isPausing()) return;
    if (current_move_cruised) cruise_controller.cancel();
    else                      stepper_controller.cancel();
}

void Motion::resume() {
    if (!isPaused()) return;
    Job::resume();

    if (current_move_cruised) {
        Vec3 pos = estimatedPosition();
        cruise_controller.reset();
        stepper_controller.reset();
        cruise_controller.setPosition(pos);
        float effectiveFeedrate = (m_activeOpts.feedrate > 0.0f) ? m_activeOpts.feedrate : m_feedrate;
        cruise_controller.setFeedrate(effectiveFeedrate);
        cruise_controller.setTarget(_target);
        if (m_async) {
            cruise_controller.start();
        } else {
            cruise_controller.start();
            while (cruise_controller.isPending()) cruise_controller.exec();
            complete();
        }
    } else {
        _position = estimatedPosition();
        cruise_controller.reset();
        stepper_controller.reset();
        Vec3 rel = toRelativeTarget(_target);
        if (m_activeOpts.optimizeRotation) rel = optimizeRelTarget(rel);
        Vec3 steps = ik(rel);
        float effectiveFeedrate = (m_activeOpts.feedrate > 0.0f) ? m_activeOpts.feedrate : m_feedrate;
        stepper_controller.setFeedrate(effectiveFeedrate);
        stepper_controller.setTarget(steps.a, steps.b, steps.c);
        if (m_async) {
            stepper_controller.start();
        } else {
            stepper_controller.start();
            while (stepper_controller.isPending()) stepper_controller.exec();
            complete();
        }
    }
}

void Motion::cancel() {
    if (isCanceling() || isCanceled() || isCompleted()) return;
    Job::cancel();
    if (isCanceling()) {
        if (current_move_cruised) cruise_controller.cancel();
        else                      stepper_controller.cancel();
    }
}

void Motion::complete() {
    _isMoving   = false;
    _isRotating = false;
    clearWaypoints();
    m_pendingOpts = MoveOptions{};  // reset des options pending
    _startPosition = _position = estimatedPosition();
    cruise_controller.reset();
    stepper_controller.reset();
    Job::complete();
    Console::info("Motion") << "Move completed" << Console::endl;
}

void Motion::forceCancel() {
    _isMoving   = false;
    _isRotating = false;
    clearWaypoints();
    m_pendingOpts = MoveOptions{};
    _startPosition = _position = estimatedPosition();
    cruise_controller.reset();
    stepper_controller.reset();
    Job::forceCancel();
    Console::info("Motion") << "Move force-canceled" << Console::endl;
}


// ============================================================
//  enable / disable / engage / disengage
// ============================================================

void Motion::enable()    { Service::enable(); }
void Motion::disable()   { Service::disable(); }

void Motion::engage() {
    SERVICE_METHOD_HEADER
    _engaged = true;
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
}

void Motion::disengage() {
    SERVICE_METHOD_HEADER
    _engaged = false;
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}


// ============================================================
//  Position / estimation
// ============================================================

Vec3 Motion::estimatedPosition() {
    if (localisation.enabled()) {
        localisation.run();
        return localisation.getPosition();
    }
    return _position + stepper_controller.getDisplacement();
}

bool Motion::hasFinished() {
    if (current_move_cruised) return cruise_controller.isCompleted();
    else                      return stepper_controller.isCompleted();
}

bool Motion::wasSuccessful() const { return isCompleted(); }


// ============================================================
//  Modes globaux
// ============================================================

void Motion::enableCruiseMode() {
    if (isMoving()) Console::error("Motion") << "Cannot toggle cruise while moving" << Console::endl;
    else use_cruise_mode = true;
}

void Motion::disableCruiseMode() {
    if (isMoving()) Console::error("Motion") << "Cannot toggle cruise while moving" << Console::endl;
    else use_cruise_mode = false;
}


// ============================================================
//  Conversions cibles
// ============================================================

Vec3 Motion::toRelativeTarget(Vec3 abs) {
    abs.sub(_position);
    abs.rotateZ(_position.c);
    return abs;
}

Vec3 Motion::toAbsoluteTarget(Vec3 rel) {
    rel.rotateZ(-_position.c);
    rel.add(_position);
    return rel;
}

Vec3 Motion::optimizeRelTarget(Vec3 rel) {
    while (rel.c >  M_PI) rel.c -= 2.0f * M_PI;
    while (rel.c <= -M_PI) rel.c += 2.0f * M_PI;
    return rel;
}


// ============================================================
//  Setters / Getters
// ============================================================

void  Motion::setOrientation(float a)     { _position.c = a; }
float Motion::getOrientation()            { return _position.c; }
Vec3  Motion::getAbsPosition() const      { return _position; }
Vec3  Motion::getAbsTarget()   const      { return _target; }

float Motion::getTargetDirection() const  { return 0.0f; }  // stub
float Motion::getAbsoluteTargetDirection() const { return Vec2(_target - _position).heading(); }
float Motion::getTargetDistance()  const  { return Vec2(_target - _position).mag(); }

bool Motion::isAbsolute()  const { return _absolute; }
bool Motion::isRelative()  const { return !_absolute; }
bool Motion::isRotating()  const { return _isRotating; }
bool Motion::isMoving()    const { return _isMoving; }
bool Motion::isSleeping()  const { return _sleeping; }

void Motion::setAbsPosition(Vec3 p) {
    _position = _startPosition = p;
    localisation.setPosition(p);
}

void Motion::setAbsTarget(Vec3 t) { _target = t; }
void Motion::setAbsolute()        { _absolute = true; }
void Motion::setRelative()        { _absolute = false; }

void Motion::setFeedrate(float f) { m_feedrate = std::max(std::min(f, 1.0f), 0.05f); }
float Motion::getFeedrate() const { return m_feedrate; }

void Motion::setAsync() { m_async = true; }
void Motion::setSync()  { m_async = false; }
