#pragma once

#include <Arduino.h>
#include <Servo.h>

namespace Actuators{

/*
    void init       ();
    void initBras   ();

    void arming (Side side = Side::BOTH);
    void fold   (Side side = Side::BOTH);
    void unfold (Side side = Side::BOTH);

    void suck   (Side side = Side::BOTH);
    void unsuck (Side side = Side::BOTH);

    void close  (Side side = Side::BOTH);
    void open   (Side side = Side::BOTH);

    void sleep  (Side side = Side::BOTH);
    void wake   (Side side = Side::BOTH);

    class Pincer{
    public:

        Pincer(
            int pinServoPincer, 
            int pinServoVentouse, 
            int pinPompe, 
            int pinEv,
            int foldedPos,
            int unfolded,
            int arming,
            int closed,
            int opened
        );

        void fold();
        void arming();
        void unfold();

        void suck();
        void unsuck();

        void close();
        void open();

        void sleep();
        void wake();

    private:

        Servo _servoPincer ;
        Servo _servoVentouse ;

        int _pinServoPincer;
        int _pinServoVentouse;

        int _pinPompe;
        int _pinEv;

        int _foldPos;
        int _unfoldPos;
        int _armingPos;
        int _closePos;
        int _openPos;

        bool asleep = true;
    };

    extern Pincer rightPincer ;
    extern Pincer leftPincer ;

    extern Servo servoDrapeau ;
    extern Servo servoRightArm ;
    extern Servo servoLeftArm ;
*/
}


