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
        createFingerGroup(RobotCompass::AB, PrimaryPresets::AB);
        createFingerGroup(RobotCompass::BC, PrimaryPresets::BC);
        createFingerGroup(RobotCompass::CA, PrimaryPresets::CA);

        registerPrimaryPoses();

    }else{
        Console::error() << "Secondary robot actuators groups are not defined !" << Console::endl;
        /*
        Console::info() << "Building fork actuators groups" << Console::endl;
        createForkGroup(RobotCompass::AB, SecondaryPreset::AB);
        createForkGroup(RobotCompass::BC, SecondaryPreset::BC);
        createForkGroup(RobotCompass::CA, SecondaryPreset::CA);
        
        registerSecondaryPoses();
        /**/
    }

    groupAB.enable();
    groupBC.enable();
    groupCA.enable();

    if(ihm.isPrimary()){
        open(RobotCompass::AB);
        open(RobotCompass::BC);
        open(RobotCompass::CA);

        moveElevator(RobotCompass::AB, ElevatorPose::DOWN);
        moveElevator(RobotCompass::BC, ElevatorPose::DOWN);
        moveElevator(RobotCompass::CA, ElevatorPose::DOWN);

    }else{
        /*
        forkUp(RobotCompass::AB);
        forkUp(RobotCompass::BC);
        forkUp(RobotCompass::CA);

        moveForkElevator(RobotCompass::AB, ElevatorPose::DOWN);
        moveForkElevator(RobotCompass::BC, ElevatorPose::DOWN);
        moveForkElevator(RobotCompass::CA, ElevatorPose::DOWN);
        /**/
    }

    delay(1000);
    sleep();
}

void Actuators::onUpdate(){
}


void setServoPos(SmartServo& servo, GripperPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void setServoPos(SmartServo& servo, ElevatorPose pose, int position){
    servo.setPose(CAST_POSE(pose), position);
}

void Actuators::registerPrimaryPoses(){

    // --- AB ---
    //Left
    setServoPos(groupAB.getServo(GRIPPER_LEFT), GripperPose::OPEN, PrimaryPresets::AB.left_Open);
    setServoPos(groupAB.getServo(GRIPPER_LEFT), GripperPose::GRAB, PrimaryPresets::AB.left_Grab);
    setServoPos(groupAB.getServo(GRIPPER_LEFT), GripperPose::CLOSE, PrimaryPresets::AB.left_Close);

    //Right
    setServoPos(groupAB.getServo(GRIPPER_RIGHT), GripperPose::OPEN, PrimaryPresets::AB.right_Open);
    setServoPos(groupAB.getServo(GRIPPER_RIGHT), GripperPose::GRAB, PrimaryPresets::AB.right_Grab);
    setServoPos(groupAB.getServo(GRIPPER_RIGHT), GripperPose::CLOSE, PrimaryPresets::AB.right_Close);

    //Elevator
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::UP, PrimaryPresets::AB.elevator_Up);
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::DOWN, PrimaryPresets::AB.elevator_Down);
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::GRAB, PrimaryPresets::AB.elevator_Grab);
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::BORDER, PrimaryPresets::AB.elevator_Border);
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::PLANTER, PrimaryPresets::AB.elevator_Planter);


    // --- BC ---
    //Left
    setServoPos(groupBC.getServo(GRIPPER_LEFT), GripperPose::OPEN, PrimaryPresets::BC.left_Open);
    setServoPos(groupBC.getServo(GRIPPER_LEFT), GripperPose::GRAB, PrimaryPresets::BC.left_Grab);
    setServoPos(groupBC.getServo(GRIPPER_LEFT), GripperPose::CLOSE, PrimaryPresets::BC.left_Close);

    //Right
    setServoPos(groupBC.getServo(GRIPPER_RIGHT), GripperPose::OPEN, PrimaryPresets::BC.right_Open);
    setServoPos(groupBC.getServo(GRIPPER_RIGHT), GripperPose::GRAB, PrimaryPresets::BC.right_Grab);
    setServoPos(groupBC.getServo(GRIPPER_RIGHT), GripperPose::CLOSE, PrimaryPresets::BC.right_Close);

    //Elevator
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::UP, PrimaryPresets::BC.elevator_Up);
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::DOWN, PrimaryPresets::BC.elevator_Down);
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::GRAB, PrimaryPresets::BC.elevator_Grab);
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::BORDER, PrimaryPresets::BC.elevator_Border);
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::PLANTER, PrimaryPresets::BC.elevator_Planter);


    // --- CA ---
    //Left
    setServoPos(groupCA.getServo(GRIPPER_LEFT), GripperPose::OPEN, PrimaryPresets::CA.left_Open);
    setServoPos(groupCA.getServo(GRIPPER_LEFT), GripperPose::GRAB, PrimaryPresets::CA.left_Grab);
    setServoPos(groupCA.getServo(GRIPPER_LEFT), GripperPose::CLOSE, PrimaryPresets::CA.left_Close);

    //Right
    setServoPos(groupCA.getServo(GRIPPER_RIGHT), GripperPose::OPEN, PrimaryPresets::CA.right_Open);
    setServoPos(groupCA.getServo(GRIPPER_RIGHT), GripperPose::GRAB, PrimaryPresets::CA.right_Grab);
    setServoPos(groupCA.getServo(GRIPPER_RIGHT), GripperPose::CLOSE, PrimaryPresets::CA.right_Close);

    //Elevator
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::UP, PrimaryPresets::CA.elevator_Up);
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::DOWN, PrimaryPresets::CA.elevator_Down);
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::GRAB, PrimaryPresets::CA.elevator_Grab);
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::BORDER, PrimaryPresets::CA.elevator_Border);
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::PLANTER, PrimaryPresets::CA.elevator_Planter);
}

/*
void Actuators::registerSecondaryPoses(){
    // --- AB ---
    //Fork
    setServoPos(groupAB.getServo(FORK_SERVO), GripperPose::CLOSE, SecondaryPreset::AB.fork_Up);
    setServoPos(groupAB.getServo(FORK_SERVO), GripperPose::OPEN, SecondaryPreset::AB.fork_Down);
    setServoPos(groupAB.getServo(FORK_SERVO), GripperPose::GRAB, SecondaryPreset::AB.fork_Grab);

    //Elevator
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::UP, SecondaryPreset::AB.elevatorFork_Up);
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::DOWN, SecondaryPreset::AB.elevatorFork_Down);
    setServoPos(groupAB.getServo(ELEVATOR), ElevatorPose::BORDER, SecondaryPreset::AB.elevatorFork_Border);

    // --- BC ---
    //Fork
    setServoPos(groupBC.getServo(FORK_SERVO), GripperPose::CLOSE, SecondaryPreset::BC.fork_Up);
    setServoPos(groupBC.getServo(FORK_SERVO), GripperPose::OPEN, SecondaryPreset::BC.fork_Down);
    setServoPos(groupBC.getServo(FORK_SERVO), GripperPose::GRAB, SecondaryPreset::BC.fork_Grab);

    //Elevator
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::UP, SecondaryPreset::BC.elevatorFork_Up);
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::DOWN, SecondaryPreset::BC.elevatorFork_Down);
    setServoPos(groupBC.getServo(ELEVATOR), ElevatorPose::BORDER, SecondaryPreset::BC.elevatorFork_Border);

    // --- CA ---
    //Left
    setServoPos(groupCA.getServo(FORK_SERVO), GripperPose::CLOSE, SecondaryPreset::CA.fork_Up);
    setServoPos(groupCA.getServo(FORK_SERVO), GripperPose::OPEN, SecondaryPreset::CA.fork_Down);
    setServoPos(groupCA.getServo(FORK_SERVO), GripperPose::GRAB, SecondaryPreset::CA.fork_Grab);

    //Elevator
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::UP, SecondaryPreset::CA.elevatorFork_Up);
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::DOWN, SecondaryPreset::CA.elevatorFork_Down);
    setServoPos(groupCA.getServo(ELEVATOR), ElevatorPose::BORDER, SecondaryPreset::CA.elevatorFork_Border);
}
/**/

void Actuators::createFingerGroup(RobotCompass rc, FingerGroupProperty props){
    
    //WARNING : Order do matter !
    //See defines in the header

    if(rc == RobotCompass::AB){
        groupAB.createServo(props.rightServoPin, props.right_Open);
        groupAB.createServo(props.elevatorServoPin, props.elevator_Down);
        groupAB.createServo(props.leftServoPin, props.left_Open);
    }else if(rc == RobotCompass::BC){
        groupBC.createServo(props.rightServoPin, props.right_Open);
        groupBC.createServo(props.elevatorServoPin, props.elevator_Down);
        groupBC.createServo(props.leftServoPin, props.left_Open);
    }else if(rc == RobotCompass::CA){
        groupCA.createServo(props.rightServoPin, props.right_Open);
        groupCA.createServo(props.elevatorServoPin, props.elevator_Down);
        groupCA.createServo(props.leftServoPin, props.left_Open);
    }
}

/*
void Actuators::createForkGroup(RobotCompass rc, ForkGroupProperty props){
    if(rc == RobotCompass::AB){
        groupAB.createServo(props.forkServoPin, props.fork_Up);
        groupAB.createServo(props.forkElevatorPin, props.elevatorFork_Down);

    }else if(rc == RobotCompass::BC){
        groupBC.createServo(props.forkServoPin, props.fork_Up);
        groupBC.createServo(props.forkElevatorPin, props.elevatorFork_Down);

    }else if(rc == RobotCompass::CA){
        groupCA.createServo(props.forkServoPin, props.fork_Up);
        groupCA.createServo(props.forkElevatorPin, props.elevatorFork_Down);
    }
}
/**/

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

void Actuators::close(RobotCompass rc, int speed){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
            groupAB.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
        }else
            while(!moveGripperDual(groupAB.getServo(GRIPPER_RIGHT), groupAB.getServo(GRIPPER_LEFT), GripperPose::CLOSE, speed));
        break;

    case RobotCompass::BC :
        if(speed == 100){
            groupBC.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
            groupBC.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
        }else
            while(!moveGripperDual(groupBC.getServo(GRIPPER_RIGHT), groupBC.getServo(GRIPPER_LEFT), GripperPose::CLOSE, speed));
        break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
            groupCA.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
        }else
            while(!moveGripperDual(groupCA.getServo(GRIPPER_RIGHT), groupCA.getServo(GRIPPER_LEFT), GripperPose::CLOSE, speed));
        break;
    
    default:
        break;
    }
}

void Actuators::open(RobotCompass rc, int speed){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        if(speed == 100){
            groupAB.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
            groupAB.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
        }else
            while(!moveGripperDual(groupAB.getServo(GRIPPER_RIGHT), groupAB.getServo(GRIPPER_LEFT), GripperPose::OPEN, speed));
        break;

    case RobotCompass::BC :
        if(speed == 100){
            groupBC.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
            groupBC.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
        }else
            while(!moveGripperDual(groupBC.getServo(GRIPPER_RIGHT), groupBC.getServo(GRIPPER_LEFT), GripperPose::OPEN, speed));
        break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
            groupCA.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
        }else
            while(!moveGripperDual(groupCA.getServo(GRIPPER_RIGHT), groupCA.getServo(GRIPPER_LEFT), GripperPose::OPEN, speed));
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
            groupAB.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
            groupAB.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
        }else
            while(!moveGripperDual(groupAB.getServo(GRIPPER_RIGHT), groupAB.getServo(GRIPPER_LEFT), GripperPose::GRAB, speed));
        break;

    case RobotCompass::BC :
        if(speed == 100){
            groupBC.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
            groupBC.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
        }else
            while(!moveGripperDual(groupBC.getServo(GRIPPER_RIGHT), groupBC.getServo(GRIPPER_LEFT), GripperPose::GRAB, speed));
        break;

    case RobotCompass::CA :
        if(speed == 100){
            groupCA.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
            groupCA.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
        }else
            while(!moveGripperDual(groupCA.getServo(GRIPPER_RIGHT), groupCA.getServo(GRIPPER_LEFT), GripperPose::GRAB, speed));
        break;
    
    default:
        break;
    }
}

/*
void Actuators::forkUp(RobotCompass rc, int speed){
    if(ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
        break;

    case RobotCompass::BC :
        groupBC.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
        break;

    case RobotCompass::CA :
        groupCA.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::CLOSE), speed);
        break;

    default:
        break;
    }
}
/**/

/*
void Actuators::forkDown(RobotCompass rc, int speed){
    if(ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
        break;

    case RobotCompass::BC :
        groupBC.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
        break;

    case RobotCompass::CA :
        groupCA.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::OPEN), speed);
        break;
    default:
        break;
    }
}
/**/

/*
void Actuators::forkGrab(RobotCompass rc, int speed){
    if(ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
        break;

    case RobotCompass::BC :
        groupBC.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
        break;

    case RobotCompass::CA :
        groupCA.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::GRAB), speed);
        break;
    default:
        break;
    }
}
/**/

void Actuators::moveElevator(RobotCompass rc, ElevatorPose pose, int speed){
    getActuatorGroup(rc).getServo(ELEVATOR).moveToPose(CAST_POSE(pose), speed);
}

/*
void Actuators::moveForkElevator(RobotCompass rc, ElevatorPose pose, int speed){
    getActuatorGroup(rc).getServo(ELEVATOR).moveToPose(CAST_POSE(pose), speed);
}
/**/

void Actuators::moveElevatorAngle(RobotCompass rc, int angle, int speed){
    getActuatorGroup(rc).getServo(ELEVATOR).moveTo(angle, speed);
}

bool Actuators::moveGripperDual(SmartServo &a, SmartServo &b, GripperPose pose, int speed){
    return a.moveToPose(CAST_POSE(pose), speed, true) || b.moveToPose(CAST_POSE(pose), speed, true);
}

bool Actuators::moveGripper(SmartServo &servo, GripperPose pose, int speed)
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
        return groupAB;
        break;
    }
}

/*
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
/**/

/*
bool Actuators::runGrabbing(RobotCompass rc, Side gs){
    if(!ihm.isPrimary()) return true;
    ActuatorGroup &group = getActuatorGroup(rc);
    GripperPose pose = GripperPose::GRAB;

    if (gs == Side::LEFT)
        return moveGripper(group.getServo(GRIPPER_LEFT), pose);
    else if (gs == Side::RIGHT)
        return moveGripper(group.getServo(GRIPPER_RIGHT), pose);
    else if (gs == Side::BOTH){
        bool leftArrived = moveGripper(group.getServo(GRIPPER_LEFT), pose);
        bool rightArrived = moveGripper(group.getServo(GRIPPER_RIGHT), pose);
        return leftArrived && rightArrived;
    }
    return true;
}

bool Actuators::runOpening(RobotCompass rc, Side gs){
    if(!ihm.isPrimary()) return true;
    ActuatorGroup &group = getActuatorGroup(rc);
    GripperPose pose = GripperPose::OPEN;

    if (gs == Side::LEFT)
        return moveGripper(group.getServo(GRIPPER_LEFT), pose);
    else if (gs == Side::RIGHT)
        return moveGripper(group.getServo(GRIPPER_RIGHT), pose);
    else if (gs == Side::BOTH){
        bool leftArrived = moveGripper(group.getServo(GRIPPER_LEFT), pose);
        bool rightArrived = moveGripper(group.getServo(GRIPPER_RIGHT), pose);
        return leftArrived && rightArrived;
    }
    return true;
}

bool Actuators::runClosing(RobotCompass rc, Side gs){
    if(!ihm.isPrimary()) return true;
    ActuatorGroup &group = getActuatorGroup(rc);
    GripperPose pose = GripperPose::CLOSE;

    if (gs == Side::LEFT)
        return moveGripper(group.getServo(GRIPPER_LEFT), pose);
    else if (gs == Side::RIGHT)
        return moveGripper(group.getServo(GRIPPER_RIGHT), pose);
    else if (gs == Side::BOTH){
        bool leftArrived = moveGripper(group.getServo(GRIPPER_LEFT), pose);
        bool rightArrived = moveGripper(group.getServo(GRIPPER_RIGHT), pose);
        return leftArrived && rightArrived;
    }
    return true;
}

bool Actuators::runElevatorUp(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveElevator(group.getServo(ELEVATOR), ElevatorPose::UP);
}

bool Actuators::runElevatorDown(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveElevator(group.getServo(ELEVATOR), ElevatorPose::DOWN);
}

bool Actuators::runElevatorGrab(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveElevator(group.getServo(ELEVATOR), ElevatorPose::GRAB);
}

bool Actuators::runElevatorBorder(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveElevator(group.getServo(ELEVATOR), ElevatorPose::BORDER);
}

bool Actuators::runElevatorPlanter(RobotCompass rc){
    ActuatorGroup &group = getActuatorGroup(rc);
    return moveElevator(group.getServo(ELEVATOR), ElevatorPose::PLANTER);
}
/**/

/*
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
/**/

/*
int Actuators::howManyPlant(RobotCompass rc){
    int count = 0;
    switch (rc)
    {
    case RobotCompass::AB:
        count += digitalRead(Pin::Sensor::SensorRight_AB) ? 0 : 1;
        count += digitalRead(Pin::Sensor::SensorLeft_AB) ? 0 : 1;
        break;

    case RobotCompass::BC:
        count += digitalRead(Pin::Sensor::SensorRight_BC) ? 0 : 1;
        count += digitalRead(Pin::Sensor::SensorLeft_BC)  ? 0 : 1;
        break;

    case RobotCompass::CA:
        count += digitalRead(Pin::Sensor::SensorRight_CA)  ? 0 : 1;
        count += digitalRead(Pin::Sensor::SensorLeft_CA) ? 0 : 1;
        break;

    default:
        return 0;
        break;
    }
    return count;
}
/**/

/*
void Actuators::applause(RobotCompass rc){
    close(rc);
    delay(500);
    grab(rc);
    delay(500);
    open(rc);
    delay(500);
}/**/

void Actuators::sleep(){
    groupAB.sleep();
    groupBC.sleep();
    groupCA.sleep();
}
