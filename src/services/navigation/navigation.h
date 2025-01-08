#pragma once
#include "services/service.h"
#include "utils/job.h"
#include "utils/geometry.h"
#include "movement.h"
#include "settings.h"
#include <vector>
#include <memory>

enum ControlMode {
    OPEN_LOOP,    // Only pure translation or rotation in step (position) mode
    CLOSED_LOOP   // Velocity-based, possibly arcs or combined motion
};

class Navigation : public Service, public Job{
public:
    // Service class methods
    void onAttach() override;
    void onUpdate() override;
    void enable() override;
    void disable() override;
    
    // Job class methods
    void run()override;
    void pause() override;
    void resume() override;
    void cancel() override;
    void complete() override;
    void forceCancel();

    // Movement routines
    Navigation& go(Vec2);
    Navigation& go(float x, float y);
    Navigation& goPolar(float angle, float dist);
    Navigation& turn(float w);
    Navigation& align(RobotCompass, float orientation);
    Navigation& move(Vec3 target);

    Navigation& begin();
    void end();
    
    void enableOptimization(); // Use rotation optimization (see optmizeRelTarget)
    void disableOptimization();// disable rotation optimization (see optmizeRelTarget)

    bool isAbsolute() const;
    bool isRelative() const;

    Vec3 targetToSteps(Vec3 relativeTarget);
    Vec3 toRelativeTarget(Vec3 absTarget);
    Vec3 toAbsoluteTarget(Vec3 absTarget);

    void setAsync(); //Non blocking mode
    void setSync(); //Blocking mode

    void setAbsolute();
    void setRelative();

    void setPosition(Vec3);  //mm, mm, rad : Absolute
    Vec3 getPosition() const;//mm, mm, rad : Absolute

    void setCalibration(CalibrationProfile c);

    /*
    float getTargetDirection() const;
    float getTargetDistance() const;
    */

    Navigation(): Service(ID_NAVIGATION){};
    SERVICE(Navigation)
 
private : 
    Vec3 optmizeRelTarget(Vec3 relTarget);

    Vec3 m_position     = {-1,-1, 0}; //Absolute mm, mm, rad : current position
    Vec3 m_calibration 	= { 1, 1, 1};
    Vec2 m_controlPoint = { 0, 0};
    
    std::vector<std::unique_ptr<Movement>> m_moves; // Movement buffer

    ControlMode m_controlMode = CLOSED_LOOP;
    bool m_chainning = false;
    bool m_absolute = true;
    bool m_optimizeRotation = true;
    bool m_async = true; //non blocking by default
};
EXTERN_DECLARATION(Navigation, nav)
