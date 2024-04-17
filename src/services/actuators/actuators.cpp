#include "actuators.h"
#include "settings.h"
#include "os/console.h"

INSTANTIATE_SERVICE(Actuators, actuators)

#define _GS_ Settings::Actuators::Gripper
#define _CPS_ Settings::Actuators::cherryPicker

Actuators::Actuators() : Service(ID_ACTUATORS),
gripperAB(ActuatorsPreset::AB),
gripperBC(ActuatorsPreset::BC),
gripperCA(ActuatorsPreset::CA){}

void Actuators::onAttach(){
    Console::info() << "Actuators activated" << Console::endl;
    pinMode(Pin::Outputs::enTraco, OUTPUT); // Enable Traco to enable Servos
    enableTraco();
    gripperAB.elevator.enable();
    gripperAB.leftGripper.enable();
    gripperAB.rightGripper.enable();
    gripperBC.elevator.enable();
    gripperBC.leftGripper.enable();
    gripperBC.rightGripper.enable();
    gripperCA.elevator.enable();
    gripperCA.leftGripper.enable();
    gripperCA.rightGripper.enable();

    open(RobotCompass::AB);
    open(RobotCompass::BC);
    open(RobotCompass::CA);
    moveElevator(RobotCompass::AB, ElevatorPose::DOWN);
    moveElevator(RobotCompass::BC, ElevatorPose::DOWN);
    moveElevator(RobotCompass::CA, ElevatorPose::DOWN);

    delay(1000);
    sleep();
}

void Actuators::onUpdate(){
}

void Actuators::enableTraco(){
    digitalWrite(Pin::Outputs::enTraco, HIGH);
}

void Actuators::disableTraco(){
    digitalWrite(Pin::Outputs::enTraco, LOW);
}

void Actuators::enable(){
    enableTraco();
    gripperAB.elevator.enable();
    gripperAB.leftGripper.enable();
    gripperAB.rightGripper.enable();
    gripperBC.elevator.enable();
    gripperBC.leftGripper.enable();
    gripperBC.rightGripper.enable();
    gripperCA.elevator.enable();
    gripperCA.leftGripper.enable();
    gripperCA.rightGripper.enable();
}

void Actuators::disable(){
    disableTraco();
    gripperAB.elevator.disable();
    gripperAB.leftGripper.disable();
    gripperAB.rightGripper.disable();
    gripperBC.elevator.disable();
    gripperBC.leftGripper.disable();
    gripperBC.rightGripper.disable();
    gripperCA.elevator.disable();
    gripperCA.leftGripper.disable();
    gripperCA.rightGripper.disable();
}

void Actuators::close(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.rightGripper.close();
        gripperAB.leftGripper.close();
        break;

    case RobotCompass::BC :
        gripperBC.rightGripper.close();
        gripperBC.leftGripper.close();
        break;

    case RobotCompass::CA :
        gripperCA.rightGripper.close();
        gripperCA.leftGripper.close();
        break;
    
    
    default:
        break;
    }
}

void Actuators::open(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.rightGripper.open();
        gripperAB.leftGripper.open();
        break;

    case RobotCompass::BC :
        gripperBC.rightGripper.open();
        gripperBC.leftGripper.open();
        break;

    case RobotCompass::CA :
        gripperCA.rightGripper.open();
        gripperCA.leftGripper.open();
        break;
    
    default:
        break;
    }
}

void Actuators::grab(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.rightGripper.grab();
        gripperAB.leftGripper.grab();
        break;

    case RobotCompass::BC :
        gripperBC.rightGripper.grab();
        gripperBC.leftGripper.grab();
        break;

    case RobotCompass::CA :
        gripperCA.rightGripper.grab();
        gripperCA.leftGripper.grab();
        break;
    
    default:
        break;
    }
}

void Actuators::forkUp(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.forkGripper.close();
        break;
    case RobotCompass::BC :
        gripperBC.forkGripper.close();
        break;
    case RobotCompass::CA :
        gripperCA.forkGripper.close();
        break;

    default:
        break;
    }
}

void Actuators::forkDown(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.forkGripper.open();
        break;
    case RobotCompass::BC :
        gripperBC.forkGripper.open();
        break;
    case RobotCompass::CA :
        gripperCA.forkGripper.open();
        break;
    default:
        break;
    }
}

void Actuators::forkGrab(RobotCompass rc){
    switch (rc)
    {
    case RobotCompass::AB :
        gripperAB.forkGripper.grab();
        break;
    case RobotCompass::BC :
        gripperBC.forkGripper.grab();
        break;
    case RobotCompass::CA :
        gripperCA.forkGripper.grab();
        break;
    default:
        break;
    }
}

void Actuators::moveElevator(RobotCompass rc, ElevatorPose pose){
    getActuatorGroup(rc).elevator.moveToPose(static_cast<int>(pose));
}

void Actuators::moveForkElevator(RobotCompass rc, ElevatorPose pose){
    getActuatorGroup(rc).forkElevator.moveToPose(static_cast<int>(pose));
}

bool Actuators::moveGripper(AdvancedServo& gripper, int target){
    int pos = gripper.getPosition();
    if (pos < target - 2)
    {
        gripper.moveTo(pos + 2);
        return false; // Not arrived
    }
    else if (pos > target + 2)
    {
        gripper.moveTo(pos - 2);
        return false; // Not arrived
    }
    return true; // Arrived
}

ActuatorGroup &Actuators::getActuatorGroup(RobotCompass rc)
{
    switch (rc)
    {
    case RobotCompass::AB:
        return gripperAB;
    case RobotCompass::BC:
        return gripperBC;
    case RobotCompass::CA:
        return gripperCA;
    default:
        return gripperAB;
        break;
    }
}

void Actuators::testSensors()
{
    CONSOLE_SERIAL.println("Sensors:");
    CONSOLE_SERIAL.print("AB: R -  ");
    CONSOLE_SERIAL.print(readSensor(RobotCompass::AB,Side::RIGHT));
    CONSOLE_SERIAL.print(" | L -  ");
    CONSOLE_SERIAL.println(readSensor(RobotCompass::AB,Side::LEFT));
    CONSOLE_SERIAL.print("BC: R -  ");
    CONSOLE_SERIAL.print(readSensor(RobotCompass::BC,Side::RIGHT));
    CONSOLE_SERIAL.print(" | L -  ");
    CONSOLE_SERIAL.println(readSensor(RobotCompass::BC,Side::LEFT));
    CONSOLE_SERIAL.print("CA: R -  ");
    CONSOLE_SERIAL.print(readSensor(RobotCompass::CA,Side::RIGHT));
    CONSOLE_SERIAL.print(" | L -  ");
    CONSOLE_SERIAL.println(readSensor(RobotCompass::CA, Side::LEFT));
}


bool Actuators::runGrabbing(RobotCompass rc, Side gs){
    ActuatorGroup &group = getActuatorGroup(rc);
    if (gs == Side::LEFT)
        return moveGripper(group.leftGripper, group.leftGripper.getGrabPosition());
    else if (gs == Side::RIGHT)
        return moveGripper(group.rightGripper, group.rightGripper.getGrabPosition());
    else if (gs == Side::BOTH){
        bool leftArrived = moveGripper(group.leftGripper, group.leftGripper.getGrabPosition());
        bool rightArrived = moveGripper(group.rightGripper, group.rightGripper.getGrabPosition());
        return leftArrived && rightArrived;
    }
    return true;
}

bool Actuators::runOpening(RobotCompass rc, Side gs){
    ActuatorGroup &group = getActuatorGroup(rc);
    if (gs == Side::LEFT)
        return moveGripper(group.leftGripper, group.leftGripper.getOpenPosition());
    else if (gs == Side::RIGHT)
        return moveGripper(group.rightGripper, group.rightGripper.getOpenPosition());
    else if (gs == Side::BOTH){
        bool leftArrived = moveGripper(group.leftGripper, group.leftGripper.getOpenPosition());
        bool rightArrived = moveGripper(group.rightGripper, group.rightGripper.getOpenPosition());
        return leftArrived && rightArrived;
    }
    return true;
}

bool Actuators::runClosing(RobotCompass rc, Side gs){
    ActuatorGroup &group = getActuatorGroup(rc);
    if (gs == Side::LEFT)
        return moveGripper(group.leftGripper, group.leftGripper.getClosePosition());
    else if (gs == Side::RIGHT)
        return moveGripper(group.rightGripper, group.rightGripper.getClosePosition());
    else if (gs == Side::BOTH){
        bool leftArrived = moveGripper(group.leftGripper, group.leftGripper.getClosePosition());
        bool rightArrived = moveGripper(group.rightGripper, group.rightGripper.getClosePosition());
        return leftArrived && rightArrived;
    }
    return true;
}

bool Actuators::runElevatorUp(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveGripper(group.elevator, group.elevator.getPose(static_cast<int>(ElevatorPose::UP)));
}

bool Actuators::runElevatorDown(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveGripper(group.elevator, group.elevator.getPose(static_cast<int>(ElevatorPose::DOWN)));
}

bool Actuators::runElevatorGrab(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveGripper(group.elevator, group.elevator.getPose(static_cast<int>(ElevatorPose::GRAB)));
}

bool Actuators::runElevatorBorder(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveGripper(group.elevator, group.elevator.getPose(static_cast<int>(ElevatorPose::BORDER)));
}

bool Actuators::runElevatorPlanter(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveGripper(group.elevator, group.elevator.getPose(static_cast<int>(ElevatorPose::PLANTER)));
}


bool Actuators::readSensor(RobotCompass rc, Side gs){
    switch (rc)
    {
    case RobotCompass::AB:
        if (gs == Side::RIGHT)
            return digitalRead(Pin::Sensor::SensorRight_AB);
        else if (gs == Side::LEFT)
            return digitalRead(Pin::Sensor::SensorLeft_AB);
        break;

    case RobotCompass::BC:
        if (gs == Side::RIGHT)
            return digitalRead(Pin::Sensor::SensorRight_BC);
        else if (gs == Side::LEFT)
            return digitalRead(Pin::Sensor::SensorLeft_BC);
        break;

    case RobotCompass::CA:
        if (gs == Side::RIGHT)
            return digitalRead(Pin::Sensor::SensorRight_CA);
        else if (gs == Side::LEFT)
            return digitalRead(Pin::Sensor::SensorLeft_CA);
        break;

    default:
        return false;
        break;
    }
    return false;
}

void Actuators::applause(RobotCompass rc){
    close(rc);
    delay(500);
    grab(rc);
    delay(500);
    open(rc);
    delay(500);
}

void Actuators::sleep(){
    gripperAB.elevator.sleep();
    gripperAB.leftGripper.sleep();
    gripperAB.rightGripper.sleep();
    gripperBC.elevator.sleep();
    gripperBC.leftGripper.sleep();
    gripperBC.rightGripper.sleep();
    gripperCA.elevator.sleep();
    gripperCA.leftGripper.sleep();
    gripperCA.rightGripper.sleep();
}
