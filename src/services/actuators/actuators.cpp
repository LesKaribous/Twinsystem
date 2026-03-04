#include "actuators.h"
#include "config/settings.h"
#include "os/console.h"
#include "services/ihm/ihm.h"

#define CAST_POSE(x) static_cast<int>(x)

SINGLETON_INSTANTIATE(Actuators, actuators)

Actuators::Actuators() : Service(ID_ACTUATORS){}

void Actuators::attach(){
    Console::info() << "Actuators activated" << Console::endl;
    pinMode(Pin::Outputs::enTraco, OUTPUT); // Enable Traco to enable Servos
    
    if(ihm.isPrimary()){
        Console::info() << "Building fingers actuators groups" << Console::endl;
 
        createHugger(RobotCompass::AB, ActuatorPresets::AB);
        //createHugger(RobotCompass::BC, ActuatorPresets::AB);
        createManipulator(RobotCompass::CA, ActuatorPresets::CA);
        
        registerPoses();

    }else{
        Console::error() << "Secondary robot actuators groups are not defined !" << Console::endl;
    }

    enable();
    enable();//sinon il fait pas la pose par defaut. Who knowns ?
    //sleep();
}

void Actuators::run(){
}


void setServoPos(SmartServo& servo, ManipulatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void setServoPos(SmartServo& servo, ElevatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void Actuators::moveElevatorOffset(RobotCompass rc, ElevatorPose pose, int offset, int speed){
    //SERVICE_METHOD_HEADER
    if(getActuatorGroup(rc).hasServo((int)ServoIDs::ELEVATOR)){
        SmartServo& elevator = getActuatorGroup(rc).getServo((int)ServoIDs::ELEVATOR);
        elevator.moveTo(elevator.getPose(CAST_POSE(pose)) + offset, speed);
    }
}

void Actuators::registerPoses()
{
    // --- AB ---
    //Lift
    if(groupAB.hasServo((int)ServoIDs::HUGGER_ELEVATOR)){
        setServoPos(groupAB.getServo((int)ServoIDs::HUGGER_ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::AB.lift_down);
        setServoPos(groupAB.getServo((int)ServoIDs::HUGGER_ELEVATOR), ElevatorPose::UP, ActuatorPresets::AB.lift_up);
        setServoPos(groupAB.getServo((int)ServoIDs::HUGGER_ELEVATOR), ElevatorPose::STORE, ActuatorPresets::AB.lift_store);
    }

    //Gripper
    if(groupAB.hasServo((int)ServoIDs::HUGGER_GRAB)){
        setServoPos(groupAB.getServo((int)ServoIDs::HUGGER_GRAB), ManipulatorPose::DROP, ActuatorPresets::AB.gripper_drop);
        setServoPos(groupAB.getServo((int)ServoIDs::HUGGER_GRAB), ManipulatorPose::GRAB, ActuatorPresets::AB.gripper_grab);
    }

    // --- BC ---
    
    // --- CA ---
    //Left
    if(groupCA.hasServo((int)ServoIDs::GRABBER_LEFT)){
        setServoPos(groupCA.getServo((int)ServoIDs::GRABBER_LEFT), ManipulatorPose::DROP, ActuatorPresets::CA.left_Drop);
        setServoPos(groupCA.getServo((int)ServoIDs::GRABBER_LEFT), ManipulatorPose::GRAB, ActuatorPresets::CA.left_Grab);
    }

    //Right
    if(groupCA.hasServo((int)ServoIDs::GRABBER_RIGHT)){
        setServoPos(groupCA.getServo((int)ServoIDs::GRABBER_RIGHT), ManipulatorPose::DROP, ActuatorPresets::CA.right_Drop);
        setServoPos(groupCA.getServo((int)ServoIDs::GRABBER_RIGHT), ManipulatorPose::GRAB, ActuatorPresets::CA.right_Grab);
    }

    //Elevator
    if(groupCA.hasServo((int)ServoIDs::ELEVATOR)){
        setServoPos(groupCA.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::UP, ActuatorPresets::CA.elevator_Up);
        setServoPos(groupCA.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::CA.elevator_Down);
        setServoPos(groupCA.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::STORE, ActuatorPresets::CA.elevator_Store);
    }
}

void Actuators::createManipulator(RobotCompass rc, ManipulatorProperties props){
    if(rc == RobotCompass::AB){
        Console::error("Actuators") << "No manipulator mounted on BC" << Console::endl;
    }else if(rc == RobotCompass::BC){
        Console::error("Actuators") << "No manipulator mounted on BC" << Console::endl;
    }else if(rc == RobotCompass::CA){
        groupCA.createServo(CAST_POSE(ServoIDs::GRABBER_RIGHT) ,props.grabberRightPin, props.right_Store);
        groupCA.createServo(CAST_POSE(ServoIDs::ELEVATOR) ,props.grabberElevatorPin, props.elevator_Down);
        groupCA.createServo(CAST_POSE(ServoIDs::GRABBER_LEFT) ,props.grabberLeftPin, props.left_Store);
    }
}

void Actuators::createHugger(RobotCompass rc, HuggerProperties props){
    if(rc == RobotCompass::AB){
        groupAB.createServo(CAST_POSE(ServoIDs::HUGGER_ELEVATOR), props.liftPin, props.lift_up);
        groupAB.createServo(CAST_POSE(ServoIDs::HUGGER_GRAB), props.gripperPin, props.gripper_drop);
    }else if(rc == RobotCompass::BC){
        Console::error("Actuators") << "No Hugger mounted on BC" << Console::endl;
    }else if(rc == RobotCompass::CA){
        Console::error("Actuators") << "No Hugger mounted on CA" << Console::endl;
    }
}


void Actuators::enableTraco(){
    digitalWrite(Pin::Outputs::enTraco, HIGH);
}

void Actuators::disableTraco(){
    digitalWrite(Pin::Outputs::enTraco, LOW);
}

void Actuators::enable(){
    groupAB.enable();
    groupBC.enable();
    groupCA.enable();
    enableTraco();
}

void Actuators::disable(){
    disableTraco();
    groupAB.disable();
    groupBC.disable();
    groupCA.disable();
}

void Actuators::drop(RobotCompass rc, int speed){
    //SERVICE_METHOD_HEADER

    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.moveServoToPose((int)ServoIDs::HUGGER_GRAB, CAST_POSE(ManipulatorPose::DROP), speed);
        }else{
            // if(groupAB.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupAB.hasServo((int)ServoIDs::MAGNET_LEFT))
            //     while(!moveMagnetDual(groupAB.getServo((int)ServoIDs::MAGNET_RIGHT), groupAB.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, speed));
            // else{
            //     groupAB.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
            //     groupAB.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            // }
        }
        break;

    // case RobotCompass::BC :
    //     if(speed == 100){
    //         //groupBC.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);

    //         //groupBC.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     }else{
    //         //if(groupBC.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupBC.hasServo((int)ServoIDs::MAGNET_LEFT))
    //         //    while(!moveMagnetDual(groupBC.getServo((int)ServoIDs::MAGNET_RIGHT), groupBC.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, speed));
    //         //else{
    //         //    groupBC.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
    //         //    groupBC.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
    //         //}
    //     }
    //     break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.moveServoToPose((int)ServoIDs::GRABBER_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
            groupCA.moveServoToPose((int)ServoIDs::GRABBER_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);
        }else{
            // if(groupCA.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupCA.hasServo((int)ServoIDs::MAGNET_LEFT))
            //     while(!moveMagnetDual(groupCA.getServo((int)ServoIDs::MAGNET_RIGHT), groupCA.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, speed));
            // else{
            //     groupCA.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
            //     groupCA.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            // }
        }
        break;
    
    default:
        break;
    }
}

void Actuators::grab(RobotCompass rc, int speed){
    //SERVICE_METHOD_HEADER
    
    if(!ihm.isPrimary()) return;

    switch (rc)    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.moveServoToPose((int)ServoIDs::HUGGER_GRAB, CAST_POSE(ManipulatorPose::GRAB), speed);
        }else{
            // if(groupAB.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupAB.hasServo((int)ServoIDs::MAGNET_LEFT))
            //     while(!moveMagnetDual(groupAB.getServo((int)ServoIDs::MAGNET_RIGHT), groupAB.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            // else{
            //     groupAB.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
            //     groupAB.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            // }
        }
        break;

    // case RobotCompass::BC :
    //     if(speed == 100){
    //         groupBC.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);

    //         groupBC.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
    //     }else{
    //         if(groupBC.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupBC.hasServo((int)ServoIDs::MAGNET_LEFT))
    //             while(!moveMagnetDual(groupBC.getServo((int)ServoIDs::MAGNET_RIGHT), groupBC.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
    //         else{
    //             groupBC.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
    //             groupBC.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
    //         }
    //     }
    //     break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.moveServoToPose((int)ServoIDs::GRABBER_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
            groupCA.moveServoToPose((int)ServoIDs::GRABBER_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);
        }else{
            // if(groupCA.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupCA.hasServo((int)ServoIDs::MAGNET_LEFT))
            //     while(!moveMagnetDual(groupCA.getServo((int)ServoIDs::MAGNET_RIGHT), groupCA.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            // else{
            //     groupCA.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
            //     groupCA.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            // }
        }
        break;
    
    default:
        break;
    }
}



void Actuators::store(RobotCompass rc, int speed){
    //SERVICE_METHOD_HEADER
    
    if(!ihm.isPrimary()) return;

    switch (rc)    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.moveServoToPose((int)ServoIDs::HUGGER_GRAB, CAST_POSE(ManipulatorPose::STORE), speed);
        }else{
            // if(groupAB.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupAB.hasServo((int)ServoIDs::MAGNET_LEFT))
            //     while(!moveMagnetDual(groupAB.getServo((int)ServoIDs::MAGNET_RIGHT), groupAB.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            // else{
            //     groupAB.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
            //     groupAB.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            // }
        }
        break;

    // case RobotCompass::BC :
    //     if(speed == 100){
    //         groupBC.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);

    //         groupBC.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
    //     }else{
    //         if(groupBC.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupBC.hasServo((int)ServoIDs::MAGNET_LEFT))
    //             while(!moveMagnetDual(groupBC.getServo((int)ServoIDs::MAGNET_RIGHT), groupBC.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
    //         else{
    //             groupBC.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
    //             groupBC.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
    //         }
    //     }
    //     break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.moveServoToPose((int)ServoIDs::GRABBER_LEFT, CAST_POSE(ManipulatorPose::STORE), speed);
            groupCA.moveServoToPose((int)ServoIDs::GRABBER_RIGHT, CAST_POSE(ManipulatorPose::STORE), speed);
        }else{
            // if(groupCA.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupCA.hasServo((int)ServoIDs::MAGNET_LEFT))
            //     while(!moveMagnetDual(groupCA.getServo((int)ServoIDs::MAGNET_RIGHT), groupCA.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            // else{
            //     groupCA.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
            //     groupCA.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            // }
        }
        break;
    
    default:
        break;
    }
}



void Actuators::moveElevator(RobotCompass rc, ElevatorPose pose, int speed){

    if (rc == RobotCompass::AB){
        if(getActuatorGroup(rc).hasServo((int)ServoIDs::HUGGER_ELEVATOR))
            getActuatorGroup(rc).getServo((int)ServoIDs::HUGGER_ELEVATOR).moveToPose(CAST_POSE(pose), speed);
    }
    
    if (rc == RobotCompass::CA){
        if(getActuatorGroup(rc).hasServo((int)ServoIDs::ELEVATOR))
            getActuatorGroup(rc).getServo((int)ServoIDs::ELEVATOR).moveToPose(CAST_POSE(pose), speed);
    }
}

void Actuators::moveElevatorAngle(RobotCompass rc, int angle, int speed){

    if (rc == RobotCompass::AB){
        if(getActuatorGroup(rc).hasServo((int)ServoIDs::HUGGER_ELEVATOR))
            getActuatorGroup(rc).getServo((int)ServoIDs::HUGGER_ELEVATOR).moveTo(angle, speed);
    }
    
    if (rc == RobotCompass::CA){
        if(getActuatorGroup(rc).hasServo((int)ServoIDs::ELEVATOR))
            getActuatorGroup(rc).getServo((int)ServoIDs::ELEVATOR).moveTo(angle, speed);

    }
}

bool Actuators::moveManipulator(SmartServo &servo, ManipulatorPose pose, int speed){
    return servo.moveToPose(CAST_POSE(pose), speed, true);
}

bool Actuators::moveElevator(SmartServo& servo, ElevatorPose pose, int speed){
    return servo.moveToPose(CAST_POSE(pose), speed, true);
}

ActuatorGroup &Actuators::getActuatorGroup(RobotCompass rc)
{
    switch (rc)
    {
    case RobotCompass::AB:
        return groupAB;
    case RobotCompass::BC:
        return groupBC;
    case RobotCompass::CA:
        return groupCA;
    default:
        return groupBC; //groupAB
        break;
    }
}

void Actuators::sleep(){
    groupAB.sleep();
    groupBC.sleep();
    groupCA.sleep();
}
