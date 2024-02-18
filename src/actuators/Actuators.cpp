#include "Actuators.h"

namespace TwinSystem
{

    Actuators::Actuators() : gripperAB({Pin::Servo::ServoB3, _GS_::AB::right_Open, _GS_::AB::right_Close, _GS_::AB::right_Grab}, Pin::Sensor::SensorRight_AB,
                                       {Pin::Servo::ServoB2, _GS_::AB::left_Open, _GS_::AB::left_Close, _GS_::AB::left_Grab}, Pin::Sensor::SensorLeft_AB,
                                       {Pin::Servo::ServoB1, _GS_::AB::elevator_Down, _GS_::AB::elevator_Up, _GS_::AB::elevator_Grab}),

                             gripperBC({Pin::Servo::ServoA3, _GS_::BC::right_Open, _GS_::BC::right_Close, _GS_::BC::right_Grab}, Pin::Sensor::SensorRight_BC,
                                       {Pin::Servo::ServoA2, _GS_::BC::left_Open, _GS_::BC::left_Close, _GS_::BC::left_Grab}, Pin::Sensor::SensorLeft_BC,
                                       {Pin::Servo::ServoA1, _GS_::BC::elevator_Down, _GS_::BC::elevator_Up, _GS_::BC::elevator_Grab}),

                             gripperCA({Pin::Servo::ServoC3, _GS_::CA::right_Open, _GS_::CA::right_Close, _GS_::CA::right_Grab}, Pin::Sensor::SensorRight_CA,
                                       {Pin::Servo::ServoC2, _GS_::CA::left_Open, _GS_::CA::left_Close, _GS_::CA::left_Grab}, Pin::Sensor::SensorLeft_CA,
                                       {Pin::Servo::ServoC1, _GS_::CA::elevator_Down, _GS_::CA::elevator_Up, _GS_::CA::elevator_Grab})
    {
        EnableTraco();
        gripperAB.rightGripper.open();
        gripperAB.leftGripper.open();
        gripperAB.elevator.open();

        gripperBC.rightGripper.open();
        gripperBC.leftGripper.open();
        gripperBC.elevator.open();

        gripperCA.rightGripper.open();
        gripperCA.leftGripper.open();
        gripperCA.elevator.open();

        // Initialisation des pins capteurs en entrée
        pinMode(Pin::Sensor::SensorRight_AB, INPUT);
        pinMode(Pin::Sensor::SensorLeft_AB, INPUT);

        pinMode(Pin::Sensor::SensorRight_BC, INPUT);
        pinMode(Pin::Sensor::SensorLeft_BC, INPUT);

        pinMode(Pin::Sensor::SensorRight_CA, INPUT);
        pinMode(Pin::Sensor::SensorLeft_CA, INPUT);

        delay(2000);
    }

    Actuators::~Actuators()
    {
    }

    void Actuators::Initialize()
    {
        pinMode(Pin::Outputs::enTraco, OUTPUT); // Enable Traco to enable Servos
        EnableTraco();
        gripperAB.elevator.enable();
        gripperAB.leftGripper.enable();
        gripperAB.rightGripper.enable();
        gripperBC.elevator.enable();
        gripperBC.leftGripper.enable();
        gripperBC.rightGripper.enable();
        gripperCA.elevator.enable();
        gripperCA.leftGripper.enable();
        gripperCA.rightGripper.enable();

        Open(RobotCompass::AB);
        Open(RobotCompass::BC);
        Open(RobotCompass::CA);

        delay(2000);
        Sleep();
    }

    // Inherited
    void Actuators::Run()
    {
        // Do something...
    }

    void Actuators::Pause()
    {
        JobExecutor::Pause();
    }

    void Actuators::Resume()
    {
        JobExecutor::Resume();
    }

    void Actuators::Cancel()
    {
        JobExecutor::Cancel();
    }

    void Actuators::Finish()
    {
        JobExecutor::Finish();
    }

    void Actuators::GoUp(RobotCompass rc)
    {
        switch (rc)
        {
        case RobotCompass::AB:
            gripperAB.elevator.close();
            break;

        case RobotCompass::BC:
            gripperBC.elevator.close();
            break;

        case RobotCompass::CA:
            gripperCA.elevator.close();
            break;

        default:
            break;
        }
    }

    void Actuators::GoDown(RobotCompass rc)
    {
        switch (rc)
        {
        case RobotCompass::AB:
            gripperAB.elevator.open();
            break;

        case RobotCompass::BC:
            gripperBC.elevator.open();
            break;

        case RobotCompass::CA:
            gripperCA.elevator.open();
            break;

        default:
            break;
        }
    }

    void Actuators::GoGrab(RobotCompass rc)
    {
        switch (rc)
        {
        case RobotCompass::AB:
            gripperAB.elevator.grab();
            break;

        case RobotCompass::BC:
            gripperBC.elevator.grab();
            break;

        case RobotCompass::CA:
            gripperCA.elevator.grab();
            break;

        default:
            break;
        }
    }

    void Actuators::Close(RobotCompass rc)
    {
        switch (rc)
        {
        case RobotCompass::AB:
            gripperAB.rightGripper.close();
            gripperAB.leftGripper.close();
            break;

        case RobotCompass::BC:
            gripperBC.rightGripper.close();
            gripperBC.leftGripper.close();
            break;

        case RobotCompass::CA:
            gripperCA.rightGripper.close();
            gripperCA.leftGripper.close();
            break;

        default:
            break;
        }
    }

    void Actuators::Open(RobotCompass rc)
    {
        switch (rc)
        {
        case RobotCompass::AB:
            gripperAB.rightGripper.open();
            gripperAB.leftGripper.open();
            break;

        case RobotCompass::BC:
            gripperBC.rightGripper.open();
            gripperBC.leftGripper.open();
            break;

        case RobotCompass::CA:
            gripperCA.rightGripper.open();
            gripperCA.leftGripper.open();
            break;

        default:
            break;
        }
    }

    void Actuators::Grab(RobotCompass rc)
    {
        switch (rc)
        {
        case RobotCompass::AB:
            gripperAB.rightGripper.grab();
            gripperAB.leftGripper.grab();
            break;

        case RobotCompass::BC:
            gripperBC.rightGripper.grab();
            gripperBC.leftGripper.grab();
            break;

        case RobotCompass::CA:
            gripperCA.rightGripper.grab();
            gripperCA.leftGripper.grab();
            break;

        default:
            break;
        }
    }

    void Actuators::Ungrab(RobotCompass rc)
    {
        Open(rc);
    }

    void Actuators::TestSensors()
    {
        Serial.println("Sensors:");
        Serial.print("AB: R -  ");
        Serial.print(ReadSensor(RobotCompass::AB,GripperSide::RIGHT));
        Serial.print(" | L -  ");
        Serial.println(ReadSensor(RobotCompass::AB,GripperSide::LEFT));
        Serial.print("BC: R -  ");
        Serial.print(ReadSensor(RobotCompass::BC,GripperSide::RIGHT));
        Serial.print(" | L -  ");
        Serial.println(ReadSensor(RobotCompass::BC,GripperSide::LEFT));
        Serial.print("CA: R -  ");
        Serial.print(ReadSensor(RobotCompass::CA,GripperSide::RIGHT));
        Serial.print(" | L -  ");
        Serial.println(ReadSensor(RobotCompass::CA,GripperSide::LEFT));
    }

    bool Actuators::ReadSensor(RobotCompass rc, GripperSide gs)
    {
        switch (rc)
        {
        case RobotCompass::AB:
            if (gs == GripperSide::RIGHT)
                return digitalRead(Pin::Sensor::SensorRight_AB);
            else
                return digitalRead(Pin::Sensor::SensorLeft_AB);
            break;

        case RobotCompass::BC:
            if (gs == GripperSide::RIGHT)
                return digitalRead(Pin::Sensor::SensorRight_BC);
            else
                return digitalRead(Pin::Sensor::SensorLeft_BC);
            break;

        case RobotCompass::CA:
            if (gs == GripperSide::RIGHT)
                return digitalRead(Pin::Sensor::SensorRight_CA);
            else
                return digitalRead(Pin::Sensor::SensorLeft_CA);
            break;

        default:
            return false;
            break;
        }
    }

    bool Actuators::moveGripper(BistableServo &gripper, int target)
    {
        int pos = gripper.getPosition();
        if (pos < target - 2)
        {
            gripper.goTo(pos + 2);
            return false; // Not arrived
        }
        else if (pos > target + 2)
        {
            gripper.goTo(pos - 2);
            return false; // Not arrived
        }
        return true; // Arrived
    }

    GripperGroup &Actuators::getGripperGroup(RobotCompass rc)
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
            break;
        }
    }

    bool Actuators::runGrabbing(RobotCompass rc)
    {
        GripperGroup &group = getGripperGroup(rc);
        bool leftArrived = moveGripper(group.leftGripper, group.leftGripper.getGrabPosition());
        bool rightArrived = moveGripper(group.rightGripper, group.rightGripper.getGrabPosition());
        return leftArrived && rightArrived;
    }

    bool Actuators::runGrabbing(RobotCompass rc, GripperSide gs){
        GripperGroup &group = getGripperGroup(rc);
        if (gs == GripperSide::LEFT)
            return moveGripper(group.leftGripper, group.leftGripper.getGrabPosition());
        else
            return moveGripper(group.rightGripper, group.rightGripper.getGrabPosition());
    }

    bool Actuators::runOpening(RobotCompass rc)
    {
        GripperGroup &group = getGripperGroup(rc);
        bool leftArrived = moveGripper(group.leftGripper, group.leftGripper.getOpenPosition());
        bool rightArrived = moveGripper(group.rightGripper, group.rightGripper.getOpenPosition());
        return leftArrived && rightArrived;
    }

    bool Actuators::runOpening(RobotCompass rc, GripperSide gs)
    {
        GripperGroup &group = getGripperGroup(rc);
        if (gs == GripperSide::LEFT)
            return moveGripper(group.leftGripper, group.leftGripper.getOpenPosition());
        else
            return moveGripper(group.rightGripper, group.rightGripper.getOpenPosition());
    }

    bool Actuators::runClosing(RobotCompass rc)
    {
        GripperGroup &group = getGripperGroup(rc);
        bool leftArrived = moveGripper(group.leftGripper, group.leftGripper.getClosePosition());
        bool rightArrived = moveGripper(group.rightGripper, group.rightGripper.getClosePosition());
        return leftArrived && rightArrived;
    }

    bool Actuators::runClosing(RobotCompass rc, GripperSide gs)
    {
        GripperGroup &group = getGripperGroup(rc);
        if (gs == GripperSide::LEFT)
            return moveGripper(group.leftGripper, group.leftGripper.getClosePosition());
        else
            return moveGripper(group.rightGripper, group.rightGripper.getClosePosition());
    }

    bool Actuators::runElevatorUp(RobotCompass rc)
    {
        GripperGroup &group = getGripperGroup(rc);
        return moveGripper(group.elevator, group.elevator.getClosePosition());
    }

    bool Actuators::runElevatorDown(RobotCompass rc)
    {
        GripperGroup &group = getGripperGroup(rc);
        return moveGripper(group.elevator, group.elevator.getOpenPosition());
    }

    bool Actuators::runElevatorGrab(RobotCompass rc)
    {
        GripperGroup &group = getGripperGroup(rc);
        return moveGripper(group.elevator, group.elevator.getGrabPosition());
    }

    void Actuators::EnableTraco()
    {
        digitalWrite(Pin::Outputs::enTraco, HIGH);
    }

    void Actuators::DisableTraco()
    {
        digitalWrite(Pin::Outputs::enTraco, LOW);
    }

    void Actuators::Sleep()
    {
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

    void Actuators::Engage()
    {
        EnableTraco();
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

    void Actuators::Disengage()
    {
        DisableTraco();
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

}