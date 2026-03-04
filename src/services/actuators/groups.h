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
    left_Store,

    right_Grab,
    right_Drop,
    right_Store,

    elevator_Up,
    elevator_Down,
    elevator_Store;
};

namespace ActuatorPresets{
    const HuggerProperties AB = {
        //servo pins
        Pin::Servo::Servo_AB_1,    //lift servo
        Pin::Servo::Servo_AB_2,    //gripper servo

        //positions
        30, //gripper_grab,
        90, //gripper_drop,
        155, //lift_up,
        90, //lift_down,
        155  //lift_store;
    };

    const ManipulatorProperties CA = {
        //pins
        Pin::Servo::Servo_CA_1, //elevator servo
        Pin::Servo::Servo_CA_3, //right 
        Pin::Servo::Servo_CA_2, //left
        Pin::PCA9685::PUMP_CA_RIGHT,
        Pin::PCA9685::EV_CA_RIGHT,
        Pin::PCA9685::PUMP_CA_LEFT,
        Pin::PCA9685::EV_CA_LEFT,

        //position servo
        20,    // left_Grab,
        20,   // left_Drop, //min pos 10 max à 65
        65,   // left_Store, //min pos 10 max à 65

        162,   // right_Grab,
        162,   // right_Drop,
        121, // right_Store,

        50,  // elevator_Up,
        5,  // elevator_Down,
        50  //  elevator_Store;
    };
}





