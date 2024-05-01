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
    }else{
        Console::info() << "Building fork actuators groups" << Console::endl;
        createForkGroup(RobotCompass::AB, SecondaryPreset::AB);
        createForkGroup(RobotCompass::BC, SecondaryPreset::BC);
        createForkGroup(RobotCompass::CA, SecondaryPreset::CA);
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
        forkUp(RobotCompass::AB);
        forkUp(RobotCompass::BC);
        forkUp(RobotCompass::CA);

        moveForkElevator(RobotCompass::AB, ElevatorPose::DOWN);
        moveForkElevator(RobotCompass::BC, ElevatorPose::DOWN);
        moveForkElevator(RobotCompass::CA, ElevatorPose::DOWN);
    }

    delay(1000);
    sleep();
}

void Actuators::onUpdate(){
}


void Actuators::createFingerGroup(RobotCompass rc, FingerGroupProperty props){
    if(rc == RobotCompass::AB){
        groupAB.createServo(props.rightServoPin, props.right_Open);
        groupAB.createServo(props.leftServoPin, props.left_Open);
        groupAB.createServo(props.elevatorServoPin, props.elevator_Down);

    }else if(rc == RobotCompass::BC){
        groupBC.createServo(props.rightServoPin, props.right_Open);
        groupBC.createServo(props.leftServoPin, props.left_Open);
        groupBC.createServo(props.elevatorServoPin, props.elevator_Down);

    }else if(rc == RobotCompass::CA){
        groupCA.createServo(props.rightServoPin, props.right_Open);
        groupCA.createServo(props.leftServoPin, props.left_Open);
        groupCA.createServo(props.elevatorServoPin, props.elevator_Down);


    }
}

void Actuators::createForkGroup(RobotCompass rc, ForkGroupProperty props){
    if(rc == RobotCompass::AB){
        groupAB.createServo(props.forkServoPin, props.fork_Open);
        groupAB.createServo(props.forkElevatorPin, props.elevatorFork_Down);

    }else if(rc == RobotCompass::BC){
        groupBC.createServo(props.forkServoPin, props.fork_Open);
        groupBC.createServo(props.forkElevatorPin, props.elevatorFork_Down);

    }else if(rc == RobotCompass::CA){
        groupCA.createServo(props.forkServoPin, props.fork_Open);
        groupCA.createServo(props.forkElevatorPin, props.elevatorFork_Down);
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

void Actuators::close(RobotCompass rc){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::CLOSE));
        groupAB.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::CLOSE));
        break;

    case RobotCompass::BC :
        groupBC.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::CLOSE));
        groupBC.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::CLOSE));
        break;

    case RobotCompass::CA :
        groupCA.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::CLOSE));
        groupCA.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::CLOSE));
        break;
    
    default:
        break;
    }
}

void Actuators::open(RobotCompass rc){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::OPEN));
        groupAB.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::OPEN));
        break;

    case RobotCompass::BC :
        groupBC.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::OPEN));
        groupBC.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::OPEN));
        break;

    case RobotCompass::CA :
        groupCA.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::OPEN));
        groupCA.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::OPEN));
        break;
    
    default:
        break;
    }
}

void Actuators::grab(RobotCompass rc){
    if(!ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::GRAB));
        groupAB.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::GRAB));
        break;

    case RobotCompass::BC :
        groupBC.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::GRAB));
        groupBC.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::GRAB));
        break;

    case RobotCompass::CA :
        groupCA.getServo(GRIPPER_RIGHT).moveToPose(CAST_POSE(GripperPose::GRAB));
        groupCA.getServo(GRIPPER_LEFT).moveToPose(CAST_POSE(GripperPose::GRAB));
        break;
    
    default:
        break;
    }
}

void Actuators::forkUp(RobotCompass rc){
    if(ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::CLOSE));
        break;

    case RobotCompass::BC :
        groupBC.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::CLOSE));
        break;

    case RobotCompass::CA :
        groupCA.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::CLOSE));
        break;

    default:
        break;
    }
}

void Actuators::forkDown(RobotCompass rc){
    if(ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::OPEN));
        break;

    case RobotCompass::BC :
        groupBC.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::OPEN));
        break;

    case RobotCompass::CA :
        groupCA.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::OPEN));
        break;
    default:
        break;
    }
}

void Actuators::forkGrab(RobotCompass rc){
    if(ihm.isPrimary()) return;
    switch (rc)
    {
    case RobotCompass::AB :
        groupAB.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::GRAB));
        break;

    case RobotCompass::BC :
        groupBC.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::GRAB));
        break;

    case RobotCompass::CA :
        groupCA.getServo(FORK_SERVO).moveToPose(CAST_POSE(GripperPose::GRAB));
        break;
    default:
        break;
    }
}

void Actuators::moveElevator(RobotCompass rc, ElevatorPose pose){
    getActuatorGroup(rc).getServo(ELEVATOR).moveToPose(CAST_POSE(pose));
}

void Actuators::moveForkElevator(RobotCompass rc, ElevatorPose pose){
    getActuatorGroup(rc).getServo(ELEVATOR).moveToPose(CAST_POSE(pose));
}

void Actuators::moveElevatorAngle(RobotCompass rc, int angle){
    getActuatorGroup(rc).getServo(ELEVATOR).moveTo(angle);
}

bool Actuators::moveGripper(SmartServo& servo, GripperPose pose){
    return servo.moveToPose(CAST_POSE(pose), true);
}

bool Actuators::moveElevator(SmartServo& servo, ElevatorPose pose){
    return servo.moveToPose(CAST_POSE(pose), true);
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
    if(ihm.isPrimary()) return true;
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
    if(ihm.isPrimary()) return true;
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

void Actuators::applause(RobotCompass rc){
    close(rc);
    delay(500);
    grab(rc);
    delay(500);
    open(rc);
    delay(500);
}

void Actuators::sleep(){
    groupAB.sleep();
    groupBC.sleep();
    groupCA.sleep();
}
