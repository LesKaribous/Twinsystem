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
//    async motion.goAlign(POI::targetA, RobotCompass::A, 90);
//    if (!motion.wasSuccessful()) return BlockResult::FAILED;
// ============================================================

class Motion : public Service, public Job {
public:
    Motion();

    void attach()  override;
    void run()     override;
    void exec()    override;

    void enable()  override;
    void disable() override;

    // Engage = moteurs alimentés et prêts ; Disengage = moteurs hors tension
    void engage();
    void disengage();

    // ---- Commandes de mouvement (chainables) ----
    Motion& go(Vec2 target);
    Motion& go(float x, float y);
    Motion& goAlign(Vec2 target, RobotCompass rc, float orientation);
    Motion& goPolar(float heading, float dist);
    Motion& goPolarAlign(float heading, float dist, RobotCompass rc, float orientation);
    Motion& turn(float angle);
    Motion& align(RobotCompass rc, float orientation);
    Motion& move(Vec3 target);

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
    // Utiliser dans les blocs Mission après `async motion.go(...)`.
    bool wasSuccessful() const;

    bool isAbsolute()  const;
    bool isRelative()  const;
    bool isRotating()  const;
    bool isSleeping()  const;
    bool isMoving()    const;

    // ---- Modes ----
    void setAbsolute();
    void setRelative();
    void setAsync();
    void setSync();
    void enableOptimization();
    void disableOptimization();
    void enableCruiseMode();
    void disableCruiseMode();
    void cancelOnCollide(bool state);

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

    // ---- Feedrate [0.05 – 1.0] ----
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

    // ---- Options ----
    bool m_async              = true;
    bool use_cancel_on_collide = false;
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
    Vec3 optimizeRelTarget(Vec3 relTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 relTarget);
};

SINGLETON_EXTERN(Motion, motion)
