#include "routines.h"
#include "os.h"
#include "services/ihm/ihm.h"
OS& os = OS::instance();
IHM& ihm = IHM::instance();

void onRobotBoot(){
    os.attachService(&ihm);
}

void onRobotIdle(){

}

void onRobotRun(){

}

void onRobotStop(){

}