#pragma once
#include "utils/geometry.h"
#include "os/jobs/asyncExecutor.h"
#include "velocityController.h"
#include "pid.h"

// ============================================================
//  PositionController — PID position → velocity → steppers
//
//  Reçoit une position cible en coordonnées monde (mm, mm, rad),
//  et pilote la VelocityController en produisant une consigne
//  de vitesse via trois PID indépendants (Vx, Vy, Vrot).
//
//  Détection de collision — deux mécanismes indépendants :
//
//    1. Bump (vitesse, rapide) :
//       Projette la vitesse OTOS réelle sur la direction commandée.
//       Compteur +2/hit, -1/bon cycle. Seuil = COLLISION_COUNT_LIMIT.
//       Gated par m_collisionEnabled (false pour les rotations pures).
//
//    2. Stall (temps, lent) :
//       Vérifie après STALL_DELAY_MS si le robot a progressé.
//       Actif pour translation ET rotation.
//       Force directement le compteur à COLLISION_COUNT_LIMIT.
// ============================================================

class PositionController : public Controller {
public:
    PositionController();

    // Appelé à chaque cycle de contrôle (PID_INTERVAL µs)
    void step()    override;
    void control() override;

    void exec()     override;
    void reset()    override;
    void start()    override;
    void complete() override;

    void onUpdate();
    void onPausing();
    void onCanceling();

    void setPosition(const Vec3& t);
    void setTarget(const Vec3& t);
    void setSteppers(Stepper* a, Stepper* b, Stepper* c);
    void setFeedrate(float feed);

    Vec3 getPosition()     const { return position / Settings::Calibration::Primary.Cartesian; }
    Vec3 getVelocity()     const { return velocity  / Settings::Calibration::Primary.Cartesian; }
    Vec3 getAcceleration() const { return acceleration; }
    Vec3 getTarget()       const { return target; }

    // true si le compteur de collision a dépassé le seuil
    bool collision() const;

    // Active/désactive la bump detection (pas le stall).
    // Doit être appelé avant start().
    // Mettre à false pour les rotations pures : l'OTOS rapporte une vélocité
    // translationnelle apparente (ω × offset mécanique) qui causerait de faux positifs.
    void setCollisionEnabled(bool enabled);

    static float shortestAngleDiff(float target, float current);

private:
    // ---- Feedrate ----
    float m_feedrate = 1.0f;

    // ---- Bump detection — tuning ----
    // Vitesse translationnelle min de commande (mm/s) sous laquelle on ne mesure pas
    static constexpr float    COLLISION_MIN_SPEED   = 80.0f;
    // Fraction manquante déclenchant un hit :
    //   projection_réelle < (1 - RATIO) × cmdMag → hit
    //   Ex: 0.50 → la vitesse réelle doit être ≥ 50% de la commande
    static constexpr float    COLLISION_RATIO        = 0.50f;
    // Délai après start() avant d'activer la détection (phase d'accélération)
    static constexpr uint32_t COLLISION_DELAY_MS     = 1000;
    // Cycles cumulés nécessaires pour confirmer une collision
    // (decay -1/cycle, accumulation +2/hit → filtre les faux positifs isolés)
    static constexpr int      COLLISION_COUNT_LIMIT  = 60;
    // Fraction min de la commande translationnelle pour activer la bump detection
    static constexpr float    COLLISION_TRANS_MIN    = 0.15f;
    // Fraction max de la commande rotationnelle au-delà de laquelle bump est ignoré
    static constexpr float    COLLISION_ROT_MAX      = 0.20f;

    // ---- Stall detection (temps) — indépendante de m_collisionEnabled ----
    // Délai avant la première vérification (ms)
    static constexpr uint32_t STALL_DELAY_MS          = 2000;
    // Période de ré-évaluation (fenêtre glissante) — ms
    static constexpr uint32_t STALL_CHECK_PERIOD_MS   = 500;
    // Déplacement translationnel minimal sur une fenêtre STALL_CHECK_PERIOD_MS (mm)
    // Valeur faible : même en décélération près de la cible, un robot libre avance un peu
    static constexpr float    STALL_RECENT_DISP_MM    = 8.0f;
    // Déplacement angulaire minimal sur une fenêtre STALL_CHECK_PERIOD_MS (rad) ~1.1°
    static constexpr float    STALL_RECENT_ANGLE_RAD  = 0.02f;
    // Amplitude translationnelle minimale de la cible pour activer le stall trans (mm)
    static constexpr float    STALL_TARGET_TRANS_MM   = 30.0f;
    // Amplitude angulaire minimale de la cible pour activer le stall rot (rad) ~2.9°
    static constexpr float    STALL_TARGET_ANGLE_RAD  = 0.05f;

    // ---- Collision state ----
    int  m_collisionCounter = 0;
    bool m_collisionEnabled = true;  // false pendant les rotations pures

    // ---- État saturé des PIDs (membres, pas static) ----
    bool m_satX = false;
    bool m_satY = false;
    bool m_satZ = false;

    // ---- Cinématique ----
    Vec3 position;
    Vec3 last_position;
    Vec3 velocity;
    Vec3 last_velocity;
    Vec3 target_velocity;
    Vec3 acceleration;
    Vec3 target;
    Vec3 newTarget;

    // ---- Timers / snapshots ----
    long     moveStart        = 0;
    uint32_t m_lastStallCheck = 0;
    uint32_t m_lastControlUs  = 0;   // horodatage du dernier onUpdate() (µs)
    Vec3     startPos         = Vec3(0);
    Vec3     m_lastStallPos   = Vec3(0);  // position au dernier check (fenêtre glissante)

    // ---- Contrôleurs ----
    VelocityController controller;
    PIDController      vx_controller;
    PIDController      vy_controller;
    PIDController      vrot_controller;

    void deccelerate();
};
