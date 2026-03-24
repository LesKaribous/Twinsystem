#include "positionController.h"
#include "config/settings.h"
#include "os/console.h"
#include "services/lidar/occupancy.h"
#include "services/localisation/localisation.h"
#include <algorithm>

#define NORMALIZE(x) ((x) > 0.0f ? 1.0f : ((x) < 0.0f ? -1.0f : 0.0f))


// ============================================================
//  Lifecycle
// ============================================================

PositionController::PositionController()
    : position(0.0f),
      last_position(0.0f),
      velocity(0.0f),
      last_velocity(0.0f),
      target_velocity(0.0f),
      acceleration(0.0f),
      target(0.0f),
      newTarget(0.0f),
      vx_controller  (4.0f,  0.0f, 100.0f),
      vy_controller  (4.0f,  0.0f, 100.0f),
      vrot_controller(10.0f, 0.0f,  70.0f)
{}

void PositionController::setFeedrate(float feed) { m_feedrate = feed; }
void PositionController::exec() {}  // unused — control() est appelé par le cycle ISR

void PositionController::reset() {
    Job::reset();
    position        = Vec3(0.0f);
    last_position   = Vec3(0.0f);
    velocity        = Vec3(0.0f);
    last_velocity   = Vec3(0.0f);
    acceleration    = Vec3(0.0f);
    target          = Vec3(0.0f);
    target_velocity = Vec3(0.0f);
    newTarget       = Vec3(0.0f);
    m_collisionCounter = 0;
    m_collisionEnabled = true;
    m_satX = m_satY = m_satZ = false;
    m_lastControlUs  = 0;
    m_lastStallCheck = 0;
    moveStart        = 0;
    startPos         = Vec3(0.0f);
    m_lastStallPos   = Vec3(0.0f);
    controller.reset();
    controller.disable();
    vx_controller.reset();
    vy_controller.reset();
    vrot_controller.reset();
}

void PositionController::start() {
    Job::start();
    controller.enable();

    if (!(newTarget == target)) target = newTarget;

    // Snapshot de départ pour la détection de blocage
    startPos         = localisation.getPosition();
    m_lastStallPos   = startPos;
    moveStart        = millis();
    m_lastStallCheck = 0;
    m_lastControlUs  = 0;  // force dt = PID_INTERVAL au premier cycle

    // Réinitialisation des flags de saturation
    m_satX = m_satY = m_satZ = false;
    m_collisionCounter = 0;
}

void PositionController::complete() {
    reset();
    m_state   = JobState::COMPLETED;
    newTarget = target = position;
}


// ============================================================
//  Collision
// ============================================================

bool PositionController::collision() const {
    // Le counter peut être monté par :
    //   - la bump detection (velocity, gated par m_collisionEnabled)
    //   - la stall detection (temps, toujours active)
    return (m_collisionCounter >= COLLISION_COUNT_LIMIT && !isCanceling());
}

void PositionController::setCollisionEnabled(bool enabled) {
    m_collisionEnabled = enabled;
    if (!enabled) m_collisionCounter = 0;
}

float PositionController::shortestAngleDiff(float tgt, float cur) {
    float diff = fmodf(tgt - cur + M_PI, 2.0f * M_PI);
    if (diff < 0.0f) diff += 2.0f * M_PI;
    diff -= M_PI;
    if (diff == -M_PI) diff = M_PI;
    return diff;
}


// ============================================================
//  onUpdate — PID + collision (appelé depuis control() à ~200Hz)
// ============================================================

void PositionController::onUpdate() {
    // ---- dt ----
    uint32_t now = micros();
    float dt = (m_lastControlUs == 0) ? (Settings::Motion::PID_INTERVAL * 1e-6f)
                                      : ((now - m_lastControlUs) * 1e-6f);
    m_lastControlUs = now;
    dt = std::clamp(dt, 1e-5f, Settings::Motion::PID_INTERVAL * 1e-6f * 2.0f);

    // ---- Cinématique depuis OTOS ----
    float heading = localisation.getPosition().c;
    position = localisation.getPosition();
    velocity = localisation.getVelocity();
    velocity.rotateZ(-heading);  // monde → robot frame

    // ---- Erreurs ----
    float angle = shortestAngleDiff(target.c, position.c);
    Vec2  error = target - position;  // monde frame

    // ---- PID → vitesse désirée ----
    Vec3 desired(0.0f);

    if (!isCanceling() && !isPausing()) {
        desired.x = vx_controller.compute(error.x, dt, m_satX);
        desired.x = std::clamp(desired.x,
            -Settings::Motion::MAX_SPEED * m_feedrate,
             Settings::Motion::MAX_SPEED * m_feedrate);
        m_satX = (fabsf(desired.x) >= Settings::Motion::MAX_SPEED * m_feedrate);

        desired.y = vy_controller.compute(error.y, dt, m_satY);
        desired.y = std::clamp(desired.y,
            -Settings::Motion::MAX_SPEED * m_feedrate,
             Settings::Motion::MAX_SPEED * m_feedrate);
        m_satY = (fabsf(desired.y) >= Settings::Motion::MAX_SPEED * m_feedrate);

        float vz_raw = vrot_controller.compute(angle, dt, m_satZ);
        desired.c = std::clamp(vz_raw,
            -Settings::Motion::MAX_ROT_SPEED * m_feedrate,
             Settings::Motion::MAX_ROT_SPEED * m_feedrate);
        m_satZ = (fabsf(desired.c) >= Settings::Motion::MAX_ROT_SPEED * m_feedrate);
    }

    // ---- Rampe d'accélération ----
    auto ramp = [](float des, float cur, float maxA, float dt_) -> float {
        if (des > cur) return cur + maxA * dt_;
        if (des < cur) return cur - maxA * dt_;
        return cur;
    };
    target_velocity.x = ramp(desired.x, target_velocity.x, Settings::Motion::MAX_ACCEL,     dt);
    target_velocity.y = ramp(desired.y, target_velocity.y, Settings::Motion::MAX_ACCEL,     dt);
    target_velocity.c = ramp(desired.c, target_velocity.c, Settings::Motion::MAX_ROT_ACCEL, dt);

    // Atténuation légère à l'approche (évite les dépassements)
    target_velocity += (velocity - target_velocity) * 0.01f;

    // ---- Snap à zéro ----
    Vec3 final_vel = target_velocity;
    if (fabsf(error.x) < Settings::Motion::MIN_DISTANCE && fabsf(final_vel.x) < 20.0f) final_vel.x = 0.0f;
    if (fabsf(error.y) < Settings::Motion::MIN_DISTANCE && fabsf(final_vel.y) < 20.0f) final_vel.y = 0.0f;
    if (fabsf(angle)   < Settings::Motion::MIN_ANGLE    && fabsf(final_vel.c) < 0.1f)  final_vel.c = 0.0f;

    // Clamp final
    final_vel.x = std::clamp(final_vel.x, -Settings::Motion::MAX_SPEED     * m_feedrate, Settings::Motion::MAX_SPEED     * m_feedrate);
    final_vel.y = std::clamp(final_vel.y, -Settings::Motion::MAX_SPEED     * m_feedrate, Settings::Motion::MAX_SPEED     * m_feedrate);
    final_vel.c = std::clamp(final_vel.c, -Settings::Motion::MAX_ROT_SPEED * m_feedrate, Settings::Motion::MAX_ROT_SPEED * m_feedrate);

    // ---- Envoi à la velocity controller ----
    if (final_vel.magSq() > 0.0f) {
        Vec3 cmd_robot = final_vel;
        cmd_robot.rotateZ(position.c);  // monde → robot frame pour les steppers
        controller.setTargetVelocity(cmd_robot);
    } else {
        if (fabsf(error.x) < Settings::Motion::MIN_DISTANCE &&
            fabsf(error.y) < Settings::Motion::MIN_DISTANCE &&
            fabsf(angle)   < Settings::Motion::MIN_ANGLE && isRunning()) {
            complete();
        }
        controller.setTargetVelocity(Vec3(0.0f));
    }

    // ============================================================
    //  Détection de collision
    //
    //  Deux mécanismes indépendants :
    //    1. Bump (velocity) — rapide, gated par m_collisionEnabled
    //    2. Stall (temps)   — lent, toujours actif (rotation incluse)
    //
    //  Shared counter : +2 par hit, -1 par bon cycle (decay).
    //  collision() retourne true quand counter >= COLLISION_COUNT_LIMIT.
    // ============================================================

    const uint32_t elapsed = (uint32_t)(millis() - moveStart);

    // ================================================================
    //  1. BUMP DETECTION — vitesse (rapide, ~300ms)
    //     Projette la vitesse OTOS sur la direction commandée.
    //     Gated par m_collisionEnabled (false pendant les rotations)
    //     car l'OTOS rapporte une vélocité translationnelle apparente
    //     proportionnelle à ω × offset_mécanique du capteur.
    // ================================================================

    if (m_collisionEnabled) {
        Vec2  cmd_world(final_vel.x, final_vel.y);
        float cmdMag    = cmd_world.mag();
        float transNorm = cmdMag / (Settings::Motion::MAX_SPEED     * m_feedrate + 1e-6f);
        float rotNorm   = fabsf(final_vel.c) / (Settings::Motion::MAX_ROT_SPEED * m_feedrate + 1e-6f);
        bool  translationDominant = (transNorm > COLLISION_TRANS_MIN) && (rotNorm < COLLISION_ROT_MAX);

        if (translationDominant && cmdMag > COLLISION_MIN_SPEED && elapsed > COLLISION_DELAY_MS) {
            Vec3 otos = localisation.getVelocity();
            Vec2 act_world(otos.x, otos.y);

            float projected = Vec2::dot(act_world, cmd_world) / cmdMag;
            float shortfall = cmdMag - projected;

            if (shortfall > COLLISION_RATIO * cmdMag) {
                m_collisionCounter += 2;
                if (m_collisionCounter > COLLISION_COUNT_LIMIT + 10)
                    m_collisionCounter = COLLISION_COUNT_LIMIT + 10;
            } else {
                if (m_collisionCounter > 0) m_collisionCounter--;
            }
        } else {
            if (m_collisionCounter > 0) m_collisionCounter--;
        }
    } else {
        if (m_collisionCounter > 0) m_collisionCounter--;
    }

    // ================================================================
    //  2. STALL DETECTION — déplacement/temps (lente, STALL_DELAY_MS)
    //     Toujours active — fonctionne pour translation ET rotation.
    //     Vérifie périodiquement si le robot a progressé vers sa cible.
    //     Fires même sur une rotation pure (turn/align bloqué).
    // ================================================================

    if (elapsed > STALL_DELAY_MS) {
        uint32_t nowMs = millis();

        if (m_lastStallCheck == 0 || (nowMs - m_lastStallCheck) >= STALL_CHECK_PERIOD_MS) {
            m_lastStallCheck = nowMs;

            // Fenêtre glissante : déplacement depuis le DERNIER check (pas depuis startPos)
            // → détecte un blocage en fin de move (phase décélération) où le robot
            //   n'a pas bougé récemment même s'il a déjà parcouru la majorité du trajet.
            Vec2  recentTrans(position.x - m_lastStallPos.x, position.y - m_lastStallPos.y);
            float recentAng = fabsf(shortestAngleDiff(position.c, m_lastStallPos.c));

            float transTarget = Vec2(target.x - startPos.x, target.y - startPos.y).mag();
            float angTarget   = fabsf(shortestAngleDiff(target.c, startPos.c));

            // Stall translationnel : cible avec composante trans, mais robot immobile
            bool transStall = (transTarget > STALL_TARGET_TRANS_MM)
                           && (recentTrans.mag() < STALL_RECENT_DISP_MM);

            // Stall rotationnel : cible avec composante rot, mais robot immobile
            bool angStall = (angTarget > STALL_TARGET_ANGLE_RAD)
                         && (recentAng  < STALL_RECENT_ANGLE_RAD);

            if (transStall || angStall) {
                m_collisionCounter = COLLISION_COUNT_LIMIT;
                Console::warn("PositionController")
                    << "Stall: recent_trans=" << (int)recentTrans.mag() << "mm"
                    << " recent_ang=" << (int)(recentAng * RAD_TO_DEG) << "deg"
                    << " t=" << (int)(elapsed / 1000) << "s"
                    << Console::endl;
            }

            // Mise à jour du snapshot pour la prochaine fenêtre
            m_lastStallPos = position;
        }
    }

    last_position = position;
    last_velocity = velocity;
}


// ============================================================
//  onPausing / onCanceling
// ============================================================

void PositionController::onPausing() {
    onCanceling();
}

void PositionController::onCanceling() {
    float dt = Settings::Motion::PID_INTERVAL * 1e-6f;

    // Décélération exponentielle
    target_velocity *= 0.9f;

    velocity = localisation.getVelocity();
    velocity.rotateZ(-position.c);
    position = position + (velocity * dt);

    Vec3 final_vel = target_velocity;
    if (fabsf(final_vel.x) < 20.0f) final_vel.x = 0.0f;
    if (fabsf(final_vel.y) < 20.0f) final_vel.y = 0.0f;
    if (fabsf(final_vel.c) < 0.1f)  final_vel.c = 0.0f;

    if (final_vel.magSq() > 0.0f) {
        final_vel.rotateZ(position.c);
        controller.setTargetVelocity(final_vel);
    } else {
        controller.setTargetVelocity(Vec3(0.0f));
        reset();
        m_state = JobState::CANCELING;
        onCanceled();
    }
}


// ============================================================
//  step / control
// ============================================================

void PositionController::step() {
    if (!isBusy()) return;
    controller.step();
}

void PositionController::control() {
    if (!isBusy()) return;

    static long lastTime = 0;
    if (micros() - lastTime < (long)Settings::Motion::PID_MIN_INTERVAL) return;
    lastTime = micros();

    if      (isPausing())   onCanceling();
    else if (isCanceling()) onCanceling();
    else if (isCanceled())  return;
    else if (isCompleted()) return;
    else                    onUpdate();
}


// ============================================================
//  Setters
// ============================================================

void PositionController::setPosition(const Vec3& t) { position = t; }
void PositionController::setTarget(const Vec3& t)   { newTarget = t; }
void PositionController::setSteppers(Stepper* a, Stepper* b, Stepper* c) {
    controller.setSteppers(a, b, c);
    controller.setTargetVelocity(Vec3(0.0f));
}


// ============================================================
//  deccelerate (utilisée si besoin par le caller)
// ============================================================

void PositionController::deccelerate() {
    float dt = Settings::Motion::PID_INTERVAL * 1e-6f;

    float currentSpeed = velocity.mag();
    if (currentSpeed > 0.0f) {
        acceleration = Vec2::normalize(velocity) * (-Settings::Motion::MAX_ACCEL * m_feedrate);
    } else {
        acceleration = Vec2(0.0f);
    }

    if (velocity.c > 0.0f)
        acceleration.c = NORMALIZE(velocity.c) * (-Settings::Motion::MAX_ROT_ACCEL * m_feedrate);
    else
        acceleration.c = 0.0f;

    target_velocity.x = 0.98f  * (target_velocity.x + acceleration.x * dt);
    target_velocity.y = 0.98f  * (target_velocity.y + acceleration.y * dt);
    target_velocity.c = 0.998f * (target_velocity.c + acceleration.c * dt);

    Vec3 final_vel = target_velocity;
    if (fabsf(final_vel.x) < 5.0f) final_vel.x = 0.0f;
    if (fabsf(final_vel.y) < 5.0f) final_vel.y = 0.0f;
    if (fabsf(final_vel.c) < 0.5f) final_vel.c = 0.0f;

    if (final_vel.magSq() > 100.0f) {
        final_vel.rotateZ(position.c);
        controller.setTargetVelocity(final_vel);
    } else {
        controller.setTargetVelocity(Vec3(0.0f));
        Console::success("PositionController") << "Move canceled successfully" << Console::endl;
        newTarget = target = position;
        onCanceled();
        reset();
        m_state = JobState::CANCELED;
    }
}
