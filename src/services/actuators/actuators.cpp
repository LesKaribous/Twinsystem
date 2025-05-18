#include "actuators.h"
#include "settings.h"
#include "os/console.h"
#include "services/ihm/ihm.h"

#define CAST_POSE(x) static_cast<int>(x)

SINGLETON_INSTANTIATE(Actuators, actuators)

Actuators::Actuators() : Service(ID_ACTUATORS){}

void Actuators::onAttach(){
    Console::info() << "Actuators activated" << Console::endl;
    pinMode(Pin::Outputs::enTraco, OUTPUT); // Enable Traco to enable Servos
   
    
    if(ihm.isPrimary()){
        Console::info() << "Building fingers actuators groups" << Console::endl;
 
        createManipulator(RobotCompass::AB, ActuatorPresets::AB);
        createBannerManipulator(RobotCompass::BC, ActuatorPresets::BC);
        createManipulator(RobotCompass::CA, ActuatorPresets::CA);
        
        registerPoses();

    }else{
        Console::error() << "Secondary robot actuators groups are not defined !" << Console::endl;
    }

    enable();//sinon il fait pas la pose par defaut. Who knowns ?
    enable();
    //sleep();
}

void Actuators::onUpdate(){
}


void setServoPos(SmartServo& servo, ManipulatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void setServoPos(SmartServo& servo, ElevatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void setServoPos(SmartServo& servo, PlankManipulatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void Actuators::moveElevatorOffset(RobotCompass rc, ElevatorPose pose, int offset, int speed){
    if(getActuatorGroup(rc).hasServo((int)ServoIDs::ELEVATOR)){
        SmartServo& elevator = getActuatorGroup(rc).getServo((int)ServoIDs::ELEVATOR);
        elevator.moveTo(elevator.getPose(CAST_POSE(pose)) + offset, speed);
    }
}

void Actuators::registerPoses()
{

    // --- AB ---
    //Left
    if(groupAB.hasServo((int)ServoIDs::MAGNET_LEFT)){
        setServoPos(groupAB.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, ActuatorPresets::AB.left_Drop);
        setServoPos(groupAB.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, ActuatorPresets::AB.left_Grab);
    }

    //Right
    if(groupAB.hasServo((int)ServoIDs::MAGNET_RIGHT)){
        setServoPos(groupAB.getServo((int)ServoIDs::MAGNET_RIGHT), ManipulatorPose::DROP, ActuatorPresets::AB.right_Drop);
        setServoPos(groupAB.getServo((int)ServoIDs::MAGNET_RIGHT), ManipulatorPose::GRAB, ActuatorPresets::AB.right_Grab);
    }

    //Elevator
    if(groupAB.hasServo((int)ServoIDs::ELEVATOR)){
        setServoPos(groupAB.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::UP, ActuatorPresets::AB.elevator_Up);
        setServoPos(groupAB.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::AB.elevator_Down);
        setServoPos(groupAB.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::DROP, ActuatorPresets::AB.elevator_Drop);
    }

    //Planks
    if(groupAB.hasServo((int)ServoIDs::PLANKS)){
        setServoPos(groupAB.getServo((int)ServoIDs::PLANKS), PlankManipulatorPose::DROP, ActuatorPresets::AB.planksDrop);
        setServoPos(groupAB.getServo((int)ServoIDs::PLANKS), PlankManipulatorPose::GRAB, ActuatorPresets::AB.planksGrab);
        setServoPos(groupAB.getServo((int)ServoIDs::PLANKS), PlankManipulatorPose::STORE, ActuatorPresets::AB.planksStore);
    }

    // --- BC ---
    //Left
    // if(groupBC.hasServo((int)ServoIDs::MAGNET_LEFT)){
    // setServoPos(groupBC.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, ActuatorPresets::BC.left_Drop);
    // setServoPos(groupBC.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, ActuatorPresets::BC.left_Grab);
    // setServoPos(groupBC.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::CLOSE, ActuatorPresets::BC.left_Close);
    // }

    //Right
    // if(groupBC.hasServo((int)ServoIDs::MAGNET_RIGHT)){
    // setServoPos(groupBC.getServo((int)ServoIDs::MAGNET_RIGHT), ManipulatorPose::DROP, ActuatorPresets::BC.right_Open);
    // setServoPos(groupBC.getServo((int)ServoIDs::MAGNET_RIGHT), ManipulatorPose::GRAB, ActuatorPresets::BC.right_Grab);
    // setServoPos(groupBC.getServo((int)ServoIDs::MAGNET_RIGHT), ManipulatorPose::CLOSE, ActuatorPresets::BC.right_Close);
    // }
    
    //Elevator
    if(groupBC.hasServo((int)ServoIDs::ELEVATOR)){
        setServoPos(groupBC.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::UP, ActuatorPresets::BC.elevator_Up);
        setServoPos(groupBC.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::BC.elevator_Down);
        setServoPos(groupBC.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::DROP, ActuatorPresets::BC.elevator_Drop);
    }

    // --- CA ---
    //Left
    if(groupCA.hasServo((int)ServoIDs::MAGNET_LEFT)){
    setServoPos(groupCA.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, ActuatorPresets::CA.left_Drop);
    setServoPos(groupCA.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, ActuatorPresets::CA.left_Grab);
    }

    //Right
    if(groupCA.hasServo((int)ServoIDs::MAGNET_RIGHT)){
        setServoPos(groupCA.getServo((int)ServoIDs::MAGNET_RIGHT), ManipulatorPose::DROP, ActuatorPresets::CA.right_Drop);
        setServoPos(groupCA.getServo((int)ServoIDs::MAGNET_RIGHT), ManipulatorPose::GRAB, ActuatorPresets::CA.right_Grab);
    }

    //Elevator
    if(groupCA.hasServo((int)ServoIDs::ELEVATOR)){
        setServoPos(groupCA.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::UP, ActuatorPresets::CA.elevator_Up);
        setServoPos(groupCA.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::DOWN, ActuatorPresets::CA.elevator_Down);
        setServoPos(groupCA.getServo((int)ServoIDs::ELEVATOR), ElevatorPose::DROP, ActuatorPresets::CA.elevator_Drop);
    }

    if(groupCA.hasServo((int)ServoIDs::PLANKS)){
        setServoPos(groupCA.getServo((int)ServoIDs::PLANKS), ManipulatorPose::DROP, ActuatorPresets::CA.planksDrop);
        setServoPos(groupCA.getServo((int)ServoIDs::PLANKS), ManipulatorPose::GRAB, ActuatorPresets::CA.planksGrab);
        setServoPos(groupCA.getServo((int)ServoIDs::PLANKS), PlankManipulatorPose::STORE, ActuatorPresets::CA.planksStore);
    }
}

void Actuators::createManipulator(RobotCompass rc, ManipulatorProperties props){

    if(rc == RobotCompass::AB){
        groupAB.createServo(CAST_POSE(ServoIDs::MAGNET_RIGHT), props.magnetRightPin, props.right_Drop);
        groupAB.createServo(CAST_POSE(ServoIDs::ELEVATOR) ,props.elevatorPin, props.elevator_Down);
        groupAB.createServo(CAST_POSE(ServoIDs::MAGNET_LEFT) ,props.magnetLeftPin, props.left_Drop);
        groupAB.createServo(CAST_POSE(ServoIDs::PLANKS) ,props.planckPin, props.planksGrab);
    }else if(rc == RobotCompass::BC){
        Console::error("Actuators") << "No manipulator mounted on BC" << Console::endl;
    }else if(rc == RobotCompass::CA){
        groupCA.createServo(CAST_POSE(ServoIDs::MAGNET_RIGHT) ,props.magnetRightPin, props.right_Drop);
        groupCA.createServo(CAST_POSE(ServoIDs::ELEVATOR) ,props.elevatorPin, props.elevator_Down);
        groupCA.createServo(CAST_POSE(ServoIDs::MAGNET_LEFT) ,props.magnetLeftPin, props.left_Drop);
        groupCA.createServo(CAST_POSE(ServoIDs::PLANKS) ,props.planckPin, props.planksGrab);
    }
}

void Actuators::createBannerManipulator(RobotCompass rc, BannerManipulatorProperties props){
    
    //WARNING : Order do matter !
    //See defines in the header

    if(rc == RobotCompass::AB){
        Console::error("Actuators") << "No manipulator mounted on BC" << Console::endl;
    }else if(rc == RobotCompass::BC){
        groupBC.createServo(CAST_POSE(ServoIDs::ELEVATOR), props.servoPin, props.elevator_Up);
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
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);

            groupAB.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
        }else{
            if(groupAB.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupAB.hasServo((int)ServoIDs::MAGNET_LEFT))
                while(!moveMagnetDual(groupAB.getServo((int)ServoIDs::MAGNET_RIGHT), groupAB.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, speed));
            else{
                groupAB.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
                groupAB.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            }
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
            groupCA.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);

            groupCA.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
        }else{
            if(groupCA.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupCA.hasServo((int)ServoIDs::MAGNET_LEFT))
                while(!moveMagnetDual(groupCA.getServo((int)ServoIDs::MAGNET_RIGHT), groupCA.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::DROP, speed));
            else{
                groupCA.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
                groupCA.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
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
            groupAB.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);

            groupAB.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
        }else{
            if(groupAB.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupAB.hasServo((int)ServoIDs::MAGNET_LEFT))
                while(!moveMagnetDual(groupAB.getServo((int)ServoIDs::MAGNET_RIGHT), groupAB.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            else{
                groupAB.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
                groupAB.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            }
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
            groupCA.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::GRAB), speed);

            groupCA.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::GRAB), speed);
        }else{
            if(groupCA.hasServo((int)ServoIDs::MAGNET_RIGHT) && groupCA.hasServo((int)ServoIDs::MAGNET_LEFT))
                while(!moveMagnetDual(groupCA.getServo((int)ServoIDs::MAGNET_RIGHT), groupCA.getServo((int)ServoIDs::MAGNET_LEFT), ManipulatorPose::GRAB, speed));
            else{
                groupCA.getServo((int)ServoIDs::MAGNET_RIGHT); //just to display error
                groupCA.getServo((int)ServoIDs::MAGNET_LEFT); //just to display error
            }
        }
        break;
    
    default:
        break;
    }
}






void Actuators::dropPlank(RobotCompass rc, int speed){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
            groupAB.moveServoToPose((int)ServoIDs::PLANKS, CAST_POSE(PlankManipulatorPose::DROP), speed);
        break;

    // case RobotCompass::BC :
    //     //groupBC.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     //groupBC.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     break;

    case RobotCompass::CA :
            groupCA.moveServoToPose((int)ServoIDs::PLANKS, CAST_POSE(PlankManipulatorPose::DROP), speed);
        break;
    
    default:
        break;
    }
}


void Actuators::grabPlank(RobotCompass rc, int speed){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
            groupAB.moveServoToPose((int)ServoIDs::PLANKS, CAST_POSE(PlankManipulatorPose::GRAB), speed);
        break;

    // case RobotCompass::BC :
    //     //groupBC.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     //groupBC.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     break;

    case RobotCompass::CA :
            groupCA.moveServoToPose((int)ServoIDs::PLANKS, CAST_POSE(PlankManipulatorPose::GRAB), speed);
        break;
    
    default:
        break;
    }
}

void Actuators::storePlank(RobotCompass rc, int speed){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
            groupAB.moveServoToPose((int)ServoIDs::PLANKS, CAST_POSE(PlankManipulatorPose::STORE), speed);
        break;

    // case RobotCompass::BC :
    //     //groupBC.moveServoToPose((int)ServoIDs::MAGNET_RIGHT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     //groupBC.moveServoToPose((int)ServoIDs::MAGNET_LEFT, CAST_POSE(ManipulatorPose::DROP), speed);
    //     break;

    case RobotCompass::CA :
            groupCA.moveServoToPose((int)ServoIDs::PLANKS, CAST_POSE(PlankManipulatorPose::STORE), speed);
        break;
    
    default:
        break;
    }
}


void Actuators::moveElevator(RobotCompass rc, ElevatorPose pose, int speed){
    if(getActuatorGroup(rc).hasServo((int)ServoIDs::ELEVATOR))
        getActuatorGroup(rc).getServo((int)ServoIDs::ELEVATOR).moveToPose(CAST_POSE(pose), speed);
}

/*
void Actuators::moveElevator(RobotCompass rc, PlankManipulatorPose pose, int speed){
    if(getActuatorGroup(rc).hasServo((int)ServoIDs::ELEVATOR))
    getActuatorGroup(rc).getServo((int)ServoIDs::ELEVATOR).moveToPose(CAST_POSE(pose), speed);
}*/

void Actuators::moveElevatorAngle(RobotCompass rc, int angle, int speed){
    if(getActuatorGroup(rc).hasServo((int)ServoIDs::ELEVATOR))
        getActuatorGroup(rc).getServo((int)ServoIDs::ELEVATOR).moveTo(angle, speed);
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
