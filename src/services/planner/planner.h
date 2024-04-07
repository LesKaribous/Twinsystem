#pragma once
#include "services/service.h"
#include "utils/job.h"
#include "utils/geometry.h"

#include "services/lidar/lidar.h"
#include "services/motion/motion.h"
/*
class Planner : public Service, public Job{
public:
    void enable()override;
    void disable()override;
    void onUpdate()override;
    void onAttach()override;
    
    Planner& go(Vec2);
    Planner& go(float x, float y);
    Planner& goPolar(float angle, float dist);
    Planner& turn(float w);
    Planner& align(RobotCompass, float orientation);
    Planner& move(Vec3 target);

    Planner(): Service(ID_PLANNER){};
    SERVICE(Planner)


};
*/