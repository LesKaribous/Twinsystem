#pragma once
#include "config/settings.h"
#include "services/service.h"
#include "os/jobs/job.h"
#include "utils/geometry.h"
#include "services/motion/controller/positionController.h"
#include "services/motion/controller/stepperController.h"
#include "services/motion/stepper.h"

#include <Wire.h>
#include <SPI.h>

// ============================================================
//  Motion — service de déplacement holonomique
//
//  Deux contrôleurs disponibles :
//    - CruiseMode (PositionController) : PID + OTOS, précis,
//      utilisé quand localisation est active.
//    - StepperMode (StepperController) : open-loop pas-à-pas,
//      fallback ou déplacements courts.
//
//  Usage typique dans un bloc Mission :
//
//    // Move simple
//    async motion.go(500, 200);
//    if (!motion.wasSuccessful()) return BlockResult::FAILED;
//
//    // Move avec options (reset automatique après le move) :
//    async motion.cancelOnCollide().feedrate(0.7f).go(500, 200);
//    async motion.noCollide().turn(90);
//
//    // Chemin multi-waypoints (via = pass-through, go = stop final) :
//    async motion.via(200, 0).via(200, 300).go(500, 300);
//    async motion.via(200, 0).cancelOnCollide().go(500, 300);
// ============================================================

class Motion : public Service, public Job {
public:

    // ============================================================
    //  Options par move — POD appliqué dans move(), reset ensuite
    //
    //  Utiliser les fluent setters avant la commande de mouvement.
    //  Les options ne s'appliquent qu'au move suivant et sont
    //  automatiquement réinitialisées aux valeurs par défaut.
    // ============================================================
    struct MoveOptions {
        bool  collisionEnabled = true;   // active la bump + stall detection
        bool  cancelOnCollide  = false;  // annule le move si collision détectée
        bool  optimizeRotation = true;   // minimise la rotation (stepper mode)
        float feedrate         = -1.0f;  // -1 → utilise le feedrate global
    };

    // ============================================================
    //  Waypoint interne — entrée de la queue
    // ============================================================
    struct Waypoint {
        Vec3 target;
        bool passThrough;  // true = ne pas s'arrêter, accepter dans WAYPOINT_RADIUS
        MoveOptions opts;  // options propres à CE segment
    };

    Motion();

    void attach()  override;
    void run()     override;
    void exec()    override;

    void enable()  override;
    void disable() override;

    // Engage = moteurs alimentés et prêts ; Disengage = moteurs hors tension
    void engage();
    void disengage();

    // ---- Fluent — options pour le prochain move (reset après move()) ----
    // Enchaîner avant la commande de déplacement :
    //   async motion.cancelOnCollide().feedrate(0.8f).go(x, y);

    Motion& noCollide();                      // désactive bump + stall
    Motion& withCollision(bool on = true);    // contrôle fin de la détection
    Motion& cancelOnCollide(bool on = true);  // annule si collision détectée
    Motion& withOptimization(bool on = true); // rotation optimisation (stepper)
    Motion& feedrate(float f);               // feedrate pour CE move uniquement

    // ---- Waypoints — chaîner via() avant go() ----
    // via() ajoute un point intermédiaire en mode pass-through.
    // go() / goAlign() / etc. ajoutent le point final (stop complet).
    //   async motion.via(A).via(B).go(C);
    Motion& via(Vec2 wp);
    Motion& via(float x, float y);

    // ---- Commandes de mouvement (chainables) ----
    Motion& go(Vec2 target);
    Motion& go(float x, float y);
    Motion& goAlign(Vec2 target, RobotCompass rc, float orientation);
    Motion& goPolar(float heading, float dist);
    Motion& goPolarAlign(float heading, float dist, RobotCompass rc, float orientation);
    Motion& turn(float angle);
    Motion& align(RobotCompass rc, float orientation);
    Motion& move(Vec3 target);  // commande bas niveau — applique m_pendingOpts

    // ---- Cycle ISR (appelé depuis le CycleManager) ----
    void step();
    void control();

    // ---- Cycle de vie du Job ----
    void start()       override;
    void pause()       override;
    void resume()      override;
    void cancel()      override;
    void forceCancel() override;
    void complete()    override;

    void onPausing()   override;
    void onCanceling() override;
    void onPaused()    override;
    void onCanceled()  override;

    // ---- Requêtes d'état ----
    Vec3  estimatedPosition();
    bool  hasFinished();

    // true si le dernier move s'est terminé normalement (pas CANCELED)
    bool wasSuccessful() const;

    bool isAbsolute()  const;
    bool isRelative()  const;
    bool isRotating()  const;
    bool isSleeping()  const;
    bool isMoving()    const;

    // ---- Modes globaux (persistants entre les moves) ----
    void setAbsolute();
    void setRelative();
    void setAsync();
    void setSync();
    void enableCruiseMode();
    void disableCruiseMode();

    // ---- Position / target ----
    void  setAbsPosition(Vec3);
    Vec3  getAbsPosition() const;
    void  setAbsTarget(Vec3);
    Vec3  getAbsTarget()   const;

    float getTargetDirection()         const;
    float getAbsoluteTargetDirection() const;
    float getTargetDistance()          const;

    float getOrientation();
    void  setOrientation(float angle);

    // ---- Feedrate global [0.05 – 1.0] (utilisé si feedrate per-move = -1) ----
    void  setFeedrate(float feed);
    float getFeedrate() const;

    SINGLETON(Motion);

private:
    // ---- Steppers ----
    Stepper m_sA, m_sB, m_sC;

    // ---- Contrôleurs ----
    PositionController cruise_controller;
    StepperController  stepper_controller;

    bool use_cruise_mode      = true;
    bool current_move_cruised = false;

    // ---- Options en attente (pour le prochain move) ----
    MoveOptions m_pendingOpts;   // accumulées par les fluent setters
    MoveOptions m_activeOpts;    // options du move en cours (pour onRunning)

    // ---- Waypoint queue ----
    static constexpr int WAYPOINT_CAPACITY = 8;
    static constexpr float WAYPOINT_RADIUS = 80.0f;  // mm — acceptance radius pass-through

    Waypoint m_waypoints[WAYPOINT_CAPACITY];
    int  m_waypointCount = 0;   // nombre de waypoints en queue (inclut le final)
    int  m_waypointIndex = 0;   // index du waypoint courant

    void enqueueWaypoint(Vec3 target, bool passThrough);
    bool advanceWaypoint();      // passe au waypoint suivant, retourne true si plus rien
    void clearWaypoints();

    // ---- Modes globaux ----
    bool m_async              = true;
    bool _optimizeRotation    = true;
    bool _absolute            = true;
    bool _debug               = true;

    // ---- État ----
    bool _engaged    = false;
    bool _sleeping   = false;
    bool _isMoving   = false;
    bool _isRotating = false;

    float m_feedrate = 1.0f;

    Vec3 _startPosition = {0, 0, 0};
    Vec3 _position      = {0, 0, 0};
    Vec3 _target        = {0, 0, 0};
    Vec2 _controlPoint  = {0, 0};

    // ---- Helpers ----
    void onRunning();
    void startWaypoint(const Waypoint& wp);  // lance le move pour un waypoint
    Vec3 optimizeRelTarget(Vec3 relTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 relTarget);
};

SINGLETON_EXTERN(Motion, motion)
