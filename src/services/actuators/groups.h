#pragma once
#include "pin.h"

struct ManipulatorProperties{
    //Pins and IDs
    const int 
    magnetLeftPin, //hall magnet Left 
    magnetRightPin, //hall magnet Right
    planckPin, //plank manipulator
    elevatorPin, //elevator
    pump_ID,   //vacuum pump
    EV_ID,     //electrovanne

    //positions
    left_Grab,
    left_Drop,
    right_Grab,
    right_Drop,
    planksGrab,
    planksDrop,
    elevator_Up,
    elevator_Down,
    elevator_Drop;
};

struct BannerManipulatorProperties{
    const int 
    servoPin, //elevator
    elevator_Up,
    elevator_Down,
    elevator_Drop;
};



namespace ActuatorPresets{
    const ManipulatorProperties AB = {
        //pins
        Pin::Servo::Servo_AB_1,    //hall magnet Left
        Pin::Servo::Servo_AB_2,    //hall magnet Right
        Pin::Servo::Servo_AB_3,    //plank manipulator
        Pin::Servo::Servo_AB_4,    //elevator
        Pin::PCA9685::PUMP_AB,
        Pin::PCA9685::EV_AB,
        //position servo
        170,    // left_Grab,
        0,      // left_Drop,
        30,     // right_Grab,
        180,    // right_Drop,
        130,    // planksUp,
        90,     // planksDrop,
        20,     // elevator_Up,
        120,    // elevator_Down,
        80      // elevator_Drop;
    };
        
    const BannerManipulatorProperties BC = {
        //pins
        Pin::Servo::Servo_BC_4,
        36,//elevator_Up,
        17,//elevator_Down,
        26 //elevator_Drop;
    };

    const ManipulatorProperties CA = {
        //pins
        Pin::Servo::Servo_CA_1, //rightServoPin
        Pin::Servo::Servo_CA_2, //leftServoPin
        Pin::Servo::Servo_CA_3, //elevatorServoPin
        Pin::Servo::Servo_CA_4, //elevatorServoPin
        Pin::PCA9685::PUMP_CA,
        Pin::PCA9685::EV_CA,
        //position servo
        180,
        0,
        0, 
        180,
        130,
        90,
        20,
        120, 
        70
    };
}
