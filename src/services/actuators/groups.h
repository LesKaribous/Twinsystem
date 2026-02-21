#pragma once
#include "config/pin.h"

struct HuggerProperties{
    //Pins and IDs
    const int 
    liftPin,    //up down servo
    gripperPin, //gripper servo

    //positions
    gripper_grab,
    gripper_drop,
    lift_up,
    lift_down,
    lift_store;
};

struct ManipulatorProperties{
    //Pins and IDs
    const int 
    grabberElevatorPin, //graber suction manipulator
    grabberRightPin, //graber suction manipulator
    grabberLeftPin, //graber suction manipulator

    pump_right_ID,   //vacuum pump
    EV__right_ID,    //electrovanne

    pump_left_ID,   //vacuum pump
    EV__left_ID,    //electrovanne

    //positions
    left_Grab,
    left_Drop,
    right_Grab,
    right_Drop,

    elevator_Up,
    elevator_Down,
    elevator_Drop;
};


namespace ActuatorPresets{
    const HuggerProperties AB = {
        //servo pins
        Pin::Servo::Servo_AB_1,    //lift servo
        Pin::Servo::Servo_AB_2,    //gripper servo

        //positions
        90, //gripper_grab,
        70, //gripper_drop,
        90, //lift_up,
        70, //lift_down,
        60  //lift_store;
    };

    const ManipulatorProperties CA = {
        //pins
        Pin::Servo::Servo_CA_1,
        Pin::Servo::Servo_CA_2,
        Pin::Servo::Servo_CA_3,
        Pin::PCA9685::PUMP_CA_RIGHT,
        Pin::PCA9685::EV_CA_RIGHT,
        Pin::PCA9685::PUMP_CA_LEFT,
        Pin::PCA9685::EV_CA_LEFT,

        //position servo
        90, // left_Grab,
        70,   // left_Drop,
        90,   // right_Grab,
        70, // right_Drop,
        90,  // elevator_Up,
        70,  // elevator_Down,
        60  //  elevator_Drop;
    };
}








