#pragma once

#include <Arduino.h>
#include <Servo.h>

namespace Actuators{

    enum class Side {
        RIGHT,
        LEFT,
        BOTH
    };

    void init();
    void initBras();

    void brasMancheAir(bool state);
    void brasVentouse(Side stateBras);

    class Pincer{
    public:

        Pincer(
            int pinServoPincer, 
            int pinServoVentouse, 
            int pinPompe, 
            int pinEv
        );

        void fold();
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
        int _closePos;
        int _openPos;

    };



    extern Pincer brasDroit ;
    extern Pincer brasGauche ;

    extern Servo servoDrapeau ;
    extern Servo servoBrasDroit ;
    extern Servo servoBrasGauche ;

}


