#include "routines.h"
#include "services/ihm/ihm.h"
#include "services/motion/motion.h"
#include "services/actuators/actuators.h"

OS& os = OS::instance();
IHM& ihm = IHM::instance();
Motion& motion = Motion::instance();
Actuators& actuators = Actuators::instance();

void onRobotBoot(){
    os.attachService(&ihm); 
    ihm.drawBootProgress("Linking ihm...");
    ihm.addBootProgress(10); 
    
    ihm.drawBootProgress("Linking motion...");
    os.attachService(&motion); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking actuators...");
    os.attachService(&actuators); ihm.addBootProgress(10);
    ihm.setPage(IHM::Page::INIT);
}

void onRobotIdle(){

}

void onRobotRun(){

}

void onRobotStop(){

}