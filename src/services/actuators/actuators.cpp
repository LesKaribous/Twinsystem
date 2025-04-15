#include "actuators.h"
#include "settings.h"
#include "os/console.h"
#include "services/ihm/ihm.h"

#define CAST_POSE(x) static_cast<int>(x)

INSTANTIATE_SERVICE(Actuators, actuators)

Actuators::Actuators() : Service(ID_ACTUATORS){}

void Actuators::onAttach(){
    Console::info() << "Actuators activated" << Console::endl;
    pinMode(Pin::Outputs::enTraco, OUTPUT); // Enable Traco to enable Servos
    enableTraco();
    
    if(ihm.isPrimary()){
        Console::info() << "Building fingers actuators groups" << Console::endl;
 
        createManipulator(RobotCompass::AB, ActuatorPresets::AB);
        createManipulator(RobotCompass::BC, ActuatorPresets::BC);
        createManipulator(RobotCompass::CA, ActuatorPresets::CA);
        
        registerPoses();

    }else{
        Console::error() << "Secondary robot actuators groups are not defined !" << Console::endl;
    }

    groupAB.enable();
    groupBC.enable();
    groupCA.enable();

    delay(1000);
    sleep();
}

void Actuators::onUpdate(){
}


void setServoPos(SmartServo& servo, ManipulatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void setServoPos(SmartServo& servo, ElevatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void Actuators::registerPoses(){

    // --- AB ---
    //Left
    if(groupAB.hasServo(MAGNET_LEFT)){
        setServoPos(groupAB.getServo(MAGNET_LEFT), ManipulatorPose::DROP, ActuatorPresets::AB.left_Drop);
        setServoPos(groupAB.getServo(MAGNET_LEFT), ManipulatorPose::GRAB, ActuatorPresets::AB.left_Grab);
    }

    //Right
    if(groupAB.hasServo(MAGNET_RIGHT)){
        setServoPos(groupAB.getServo(MAGNET_RIGHT), ManipulatorPose::DROP, ActuatorPresets::AB.right_Drop);
        setServoPos(groupAB.getServo(MAGNET_RIGHT), ManipulatorPose::GRAB, ActuatorPresets::AB.right_Grab);
    }

    //Elevator
    if(groupAB.hasServo(ELEVATOR)){
        setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::UP, ActuatorPresets::AB.elevator_Up);
        setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::AB.elevator_Down);
        setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::DROP, ActuatorPresets::AB.elevator_Drop);
    }

    //Planks
    if(groupAB.hasServo(PLANKS)){
        setServoPos(groupAB.getServo(PLANKS), ManipulatorPose::DROP, ActuatorPresets::AB.planksDrop);
        setServoPos(groupAB.getServo(PLANKS), ManipulatorPose::GRAB, ActuatorPresets::AB.planksGrab);
    }

    // --- BC ---
    //Left
    // if(groupBC.hasServo(MAGNET_LEFT)){
    // setServoPos(groupBC.getServo(MAGNET_LEFT), ManipulatorPose::DROP, ActuatorPresets::BC.left_Drop);
    // setServoPos(groupBC.getServo(MAGNET_LEFT), ManipulatorPose::GRAB, ActuatorPresets::BC.left_Grab);
    // setServoPos(groupBC.getServo(MAGNET_LEFT), ManipulatorPose::CLOSE, ActuatorPresets::BC.left_Close);
    // }

    //Right
    // if(groupBC.hasServo(MAGNET_RIGHT)){
    // setServoPos(groupBC.getServo(MAGNET_RIGHT), ManipulatorPose::DROP, ActuatorPresets::BC.right_Open);
    // setServoPos(groupBC.getServo(MAGNET_RIGHT), ManipulatorPose::GRAB, ActuatorPresets::BC.right_Grab);
    // setServoPos(groupBC.getServo(MAGNET_RIGHT), ManipulatorPose::CLOSE, ActuatorPresets::BC.right_Close);
    // }
    
    //Elevator
    if(groupBC.hasServo(ELEVATOR)){
        setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::UP, ActuatorPresets::BC.elevator_Up);
        setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::BC.elevator_Down);
        setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::DROP, ActuatorPresets::BC.elevator_Drop);
    }

    // --- CA ---
    //Left
    if(groupCA.hasServo(MAGNET_LEFT)){
    setServoPos(groupCA.getServo(MAGNET_LEFT), ManipulatorPose::DROP, ActuatorPresets::CA.left_Drop);
    setServoPos(groupCA.getServo(MAGNET_LEFT), ManipulatorPose::GRAB, ActuatorPresets::CA.left_Grab);
    }

    //Right
    if(groupCA.hasServo(MAGNET_RIGHT)){
        setServoPos(groupCA.getServo(MAGNET_RIGHT), ManipulatorPose::DROP, ActuatorPresets::CA.right_Drop);
        setServoPos(groupCA.getServo(MAGNET_RIGHT), ManipulatorPose::GRAB, ActuatorPresets::CA.right_Grab);
    }

    //Elevator
    if(groupCA.hasServo(ELEVATOR)){
        setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::UP, ActuatorPresets::CA.elevator_Up);
        setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::CA.elevator_Down);
        setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::DROP, ActuatorPresets::CA.elevator_Drop);
    }

    if(groupCA.hasServo(PLANKS)){
        setServoPos(groupCA.getServo(PLANKS), ManipulatorPose::DROP, ActuatorPresets::CA.planksDrop);
        setServoPos(groupCA.getServo(PLANKS), ManipulatorPose::GRAB, ActuatorPresets::CA.planksGrab);
    }
}


void Actuators::createManipulator(RobotCompass rc, ManipulatorProperties props){
    
    //WARNING : Order do matter !
    //See defines in the header

    if(rc == RobotCompass::AB){
        groupAB.createServo(props.magnetRightPin, props.left_Grab);
        groupAB.createServo(props.elevatorPin, props.elevator_Down);
        groupAB.createServo(props.magnetLeftPin, props.left_Grab);
        groupAB.createServo(props.planckPin, props.planksDrop);
    }else if(rc == RobotCompass::BC){
        Console::error("Actuators") << "No manipulator mounted on BC" << Console::endl;
    }else if(rc == RobotCompass::CA){
        groupCA.createServo(props.magnetRightPin, props.left_Grab);
        groupCA.createServo(props.elevatorPin, props.elevator_Down);
        groupCA.createServo(props.magnetLeftPin, props.left_Grab);
        groupCA.createServo(props.planckPin, props.planksDrop);
    }
}

void Actuators::createManipulator(RobotCompass rc, BannerManipulatorProperties props){
    
    //WARNING : Order do matter !
    //See defines in the header

    if(rc == RobotCompass::AB){
        Console::error("Actuators") << "No manipulator mounted on BC" << Console::endl;
    }else if(rc == RobotCompass::BC){
        groupBC.createServo(props.servoPin, props.elevator_Down);
    }else if(rc == RobotCompass::CA){
        Console::error("Actuators") << "No manipulator mounted on BC" << Console::endl;
    }
}


void Actuators::enableTraco(){
    digitalWrite(Pin::Outputs::enTraco, HIGH);
}

void Actuators::disableTraco(){
    digitalWrite(Pin::Outputs::enTraco, LOW);
}

void Actuators::enable(){
    enableTraco();
    groupAB.enable();
    groupBC.enable();
    groupCA.enable();
}

void Actuators::disable(){
    disableTraco();
    groupAB.disable();
    groupBC.disable();
    groupCA.disable();
}

void Actuators::drop(RobotCompass rc, int speed){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.moveServoToPose(MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);

            groupAB.moveServoToPose(MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
        }else{
            if(groupAB.hasServo(MAGNET_RIGHT) && groupAB.hasServo(MAGNET_LEFT))
                while(!moveMagnetDual(groupAB.getServo(MAGNET_RIGHT), groupAB.getServo(MAGNET_LEFT), ManipulatorPose::DROP, speed));
            else{
                groupAB.getServo(MAGNET_RIGHT); //just to display error
                groupAB.getServo(MAGNET_LEFT); //just to display error
            }
        }
        break;

    // case RobotCompass::BC :
    //     if(speed == 100){
    //         //groupBC.moveServoToPose(MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);

    //         //groupBC.moveServoToPose(MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     }else{
    //         //if(groupBC.hasServo(MAGNET_RIGHT) && groupBC.hasServo(MAGNET_LEFT))
    //         //    while(!moveMagnetDual(groupBC.getServo(MAGNET_RIGHT), groupBC.getServo(MAGNET_LEFT), ManipulatorPose::DROP, speed));
    //         //else{
    //         //    groupBC.getServo(MAGNET_RIGHT); //just to display error
    //         //    groupBC.getServo(MAGNET_LEFT); //just to display error
    //         //}
    //     }
    //     break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.moveServoToPose(MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);

            groupCA.moveServoToPose(MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
        }else{
            if(groupCA.hasServo(MAGNET_RIGHT) && groupCA.hasServo(MAGNET_LEFT))
                while(!moveMagnetDual(groupCA.getServo(MAGNET_RIGHT), groupCA.getServo(MAGNET_LEFT), ManipulatorPose::DROP, speed));
            else{
                groupCA.getServo(MAGNET_RIGHT); //just to display error
                groupCA.getServo(MAGNET_LEFT); //just to display error
            }
        }
        break;
    
    default:
        break;
    }
}

void Actuators::grab(RobotCompass rc, int speed){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.moveServoToPose(MAGNET_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);

            groupAB.moveServoToPose(MAGNET_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
        }else{
            if(groupAB.hasServo(MAGNET_RIGHT) && groupAB.hasServo(MAGNET_LEFT))
                while(!moveMagnetDual(groupAB.getServo(MAGNET_RIGHT), groupAB.getServo(MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            else{
                groupAB.getServo(MAGNET_RIGHT); //just to display error
                groupAB.getServo(MAGNET_LEFT); //just to display error
            }
        }
        break;

    // case RobotCompass::BC :
    //     if(speed == 100){
    //         groupBC.moveServoToPose(MAGNET_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);

    //         groupBC.moveServoToPose(MAGNET_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
    //     }else{
    //         if(groupBC.hasServo(MAGNET_RIGHT) && groupBC.hasServo(MAGNET_LEFT))
    //             while(!moveMagnetDual(groupBC.getServo(MAGNET_RIGHT), groupBC.getServo(MAGNET_LEFT), ManipulatorPose::GRAB, speed));
    //         else{
    //             groupBC.getServo(MAGNET_RIGHT); //just to display error
    //             groupBC.getServo(MAGNET_LEFT); //just to display error
    //         }
    //     }
    //     break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.moveServoToPose(MAGNET_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);

            groupCA.moveServoToPose(MAGNET_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
        }else{
            if(groupCA.hasServo(MAGNET_RIGHT) && groupCA.hasServo(MAGNET_LEFT))
                while(!moveMagnetDual(groupCA.getServo(MAGNET_RIGHT), groupCA.getServo(MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            else{
                groupCA.getServo(MAGNET_RIGHT); //just to display error
                groupCA.getServo(MAGNET_LEFT); //just to display error
            }
        }
        break;
    
    default:
        break;
    }
}

void Actuators::moveElevator(RobotCompass rc, ElevatorPose pose, int speed){
    if(getActuatorGroup(rc).hasServo(ELEVATOR))
        getActuatorGroup(rc).getServo(ELEVATOR).moveToPose(CAST_POSE(pose), speed);
}

void Actuators::moveElevatorAngle(RobotCompass rc, int angle, int speed){
    if(getActuatorGroup(rc).hasServo(ELEVATOR))
        getActuatorGroup(rc).getServo(ELEVATOR).moveTo(angle, speed);
}

bool Actuators::moveMagnetDual(SmartServo &a, SmartServo &b, ManipulatorPose pose, int speed){
    return a.moveToPose(CAST_POSE(pose), speed, true) || b.moveToPose(CAST_POSE(pose), speed, true);
}

bool Actuators::moveMagnet(SmartServo &servo, ManipulatorPose pose, int speed)
{
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
