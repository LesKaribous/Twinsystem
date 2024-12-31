#pragma once

#include "pin.h"

struct FingerGroupProperty{
const int 
    rightServoPin,
    leftServoPin,
    elevatorServoPin;

//position servo
const int 
    right_Close,
    right_Open,
    right_Grab,
    left_Close,
    left_Open,
    left_Grab,
    elevator_Down,
    elevator_Up,
    elevator_Grab,
    elevator_Border,
    elevator_Planter;
};

namespace PrimaryPresets{
    const FingerGroupProperty AB = {
        //pins
        Pin::Servo::ServoB3, //rightServoPin
        Pin::Servo::ServoB2, //leftServoPin
        Pin::Servo::ServoB1, //elevatorServoPin
        //position servo
        100, //right_Close
        10,  //right_Open   
        180, //right_Grab   
        60, //left_Close   
        0,  //left_Open    
        150, //left_Grab    
        15,  //elevator_Down
        110, //elevator_Up  
        25,   //elevator_Grab
        60,   //elevator_Border
        40,   //elevator_Planter
    };
        
    const FingerGroupProperty BC = {
        //pins
        Pin::Servo::ServoA3, //rightServoPin
        Pin::Servo::ServoA2, //leftServoPin
        Pin::Servo::ServoA1, //elevatorServoPin
        //position servo
        80, //right_Close
        10,  //right_Open   
        160, //right_Grab   
        80, //left_Close   
        10,  //left_Open    
        160, //left_Grab    
        0,   //elevator_Down
        140, //elevator_Up  
        20,   //elevator_Grab
        70,   //elevator_Border
        40   //elevator_Planter
    };

    const FingerGroupProperty CA = {
        //pins
        Pin::Servo::ServoC3, //rightServoPin
        Pin::Servo::ServoC2, //leftServoPin
        Pin::Servo::ServoC1, //elevatorServoPin
        //position servo
        100, //right_Close
        10,  //right_Open   
        180, //right_Grab   
        100, //left_Close   
        10,  //left_Open    
        180, //left_Grab    
        0,   //elevator_Down
        105, //elevator_Up  
        18,   //elevator_Grab
        53,   //elevator_Border
        30   //elevator_Planter
    };
}
