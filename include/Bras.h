#ifndef DEF_BRAS
#define DEF_BRAS

#include <Arduino.h>
#include <Servo.h>

class Bras
{
    public:

    Bras();
    ~Bras();
    void setPin(int pinServoBras, int pinServoVentouse, int pinPompe, int pinEv);
    void setPin(int pinServoBras, int pinServoVentouse, int pinPompe, int pinEv, int pinAscenseur, int pinPotard);

    void setLimit(int minServoBras, int maxServoBras, int minServoVentouse, int maxServoVentouse);
    void setLimit(int minServoBras, int maxServoBras, int minServoVentouse, int maxServoVentouse, int stopAscenseur , int minAscenseur , int maxAscenseur);

    int calcPositionBras(byte posServoBras);
    int calcPositionVentouse(byte posServoVentouse);
    int calcPositionAscenseur(byte posAscenseur);

    void setPosition(byte posServoBras, byte posServoVentouse, bool pompe, bool ev, int wait);

    void setBras(byte posServoBras);
    void setBras(byte posServoBras, int wait);

    void setAscenseur(int posAscenseur);
    void setAscenseur(int posAscenseur, int wait);

    void setVentouse(byte posServoVentouse);
    void setVentouse(byte posServoVentouse, int wait);

    int getPotardValue();

    void setPompe(bool state);
    void setPompe(bool state, int wait);
    void setEV(bool state);
    void setEV(bool state, int wait);

    private:

    Servo _servoBras ;
    Servo _servoVentouse ;
    Servo _servoAscenseur ;

    int _pinServoBras;
    int _pinServoVentouse;

    int _pinPompe ;
    int _pinEv ;

    int _pinAscenseur;
    int _pinPotard;

    int _minServoBras ;
    int _maxServoBras ;
    int _minServoVentouse ;
    int _maxServoVentouse ;
    int _minAscenseur ;
    int _maxAscenseur ;

    int _stopAscenseur ;

};

#endif
