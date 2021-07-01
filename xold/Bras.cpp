#include "Bras.h"

Bras::Bras()
{

}

Bras::~Bras()
{

}

void Bras::setPin(int pinServoBras, int pinServoVentouse, int pinPompe, int pinEv)
{
  _pinServoBras = pinServoBras ;
  _pinServoVentouse = pinServoVentouse ;
  _pinPompe = pinPompe ;
  _pinEv = pinEv ;

  _servoBras.detach();
  _servoVentouse.detach();

  _servoBras.attach(_pinServoBras);
  _servoVentouse.attach(_pinServoVentouse);

  pinMode(_pinPompe,OUTPUT);
  pinMode(_pinEv,OUTPUT);
}

void Bras::setPin(int pinServoBras, int pinServoVentouse, int pinPompe, int pinEv, int pinAscenseur, int pinPotard)
{
  setPin(pinServoBras, pinServoVentouse, pinPompe, pinEv) ;

  _pinAscenseur = pinAscenseur ;
  _pinPotard = pinPotard ;

}

void Bras::setLimit(int minServoBras, int maxServoBras, int minServoVentouse, int maxServoVentouse)
{
  _minServoBras = minServoBras ;
  _maxServoBras = maxServoBras ;
  _minServoVentouse = minServoVentouse ;
  _maxServoVentouse = maxServoVentouse ;
}

void Bras::setLimit(int minServoBras, int maxServoBras, int minServoVentouse, int maxServoVentouse, int stopAscenseur,int minAscenseur , int maxAscenseur)
{
  setLimit(minServoBras, maxServoBras, minServoVentouse, maxServoVentouse);
  _stopAscenseur = stopAscenseur ;
  _minAscenseur = minAscenseur ;
  _maxAscenseur = maxAscenseur ;
}

int Bras::calcPositionBras(byte posServoBras)
{
  // limiter la commande entre 0 et 100%
  int newPosBras = constrain(posServoBras,0,100);
  // Calculer la position servo en fonction des limites min max
  newPosBras = map(newPosBras,0,100,_minServoBras,_maxServoBras);
  return newPosBras;
}

int Bras::calcPositionVentouse(byte posServoVentouse)
{
  // limiter la commande entre 0 et 100%
  int newPosVentouse = constrain(posServoVentouse,0,100);
  // Calculer la position servo en fonction des limites min max
  newPosVentouse = map(newPosVentouse,0,100,_minServoVentouse,_maxServoVentouse);
  return newPosVentouse;
}

int Bras::calcPositionAscenseur(byte posAscenseur)
{
  // limiter la commande entre 0 et 100%
  int newPosAscenseur = constrain(posAscenseur,0,100);
  // Calculer la position servo en fonction des limites min max
  newPosAscenseur = map(newPosAscenseur,0,100,_minAscenseur,_maxAscenseur);
  return newPosAscenseur;
}

void Bras::setPosition(byte posServoBras, byte posServoVentouse, bool pompe, bool ev, int wait)
{
  // Appliquer la position
  _servoBras.write(calcPositionBras(posServoBras));
  _servoVentouse.write(calcPositionVentouse(posServoVentouse));

  // Appliquer la commande de pompe/EV
  digitalWrite(_pinPompe,pompe);
  digitalWrite(_pinEv,ev);

  delay(wait);
}

void Bras::setBras(byte posServoBras)
{
  _servoBras.write(calcPositionBras(posServoBras));
}

void Bras::setBras(byte posServoBras, int wait)
{
  _servoBras.write(calcPositionBras(posServoBras));
  delay(wait);
}

void Bras::setAscenseur(int posAscenseur)
{
  int Input = analogRead(_pinPotard);

  posAscenseur = calcPositionAscenseur(posAscenseur);

  _servoAscenseur.detach();
  _servoAscenseur.attach(_pinAscenseur);

  while(Input > (posAscenseur + 2) || Input < (posAscenseur - 2))
  {
    Input = analogRead(_pinPotard);
    if (Input > posAscenseur) _servoAscenseur.write(_stopAscenseur-12);
    else _servoAscenseur.write(_stopAscenseur+12);
  }
  _servoAscenseur.write(_stopAscenseur);
  delay(200);
  _servoAscenseur.detach();
}

void Bras::setAscenseur(int posAscenseur, int wait)
{
  setAscenseur(posAscenseur);
  delay(wait);
}

int Bras::getPotardValue()
{
  return analogRead(_pinPotard);
}

void Bras::setVentouse(byte posServoVentouse)
{
  _servoVentouse.write(calcPositionVentouse(posServoVentouse));
}

void Bras::setVentouse(byte posServoVentouse, int wait)
{
  _servoVentouse.write(calcPositionVentouse(posServoVentouse));
  delay(wait);
}

void Bras::setPompe(bool state)
{
  digitalWrite(_pinPompe,state);
}

void Bras::setPompe(bool state, int wait)
{
  digitalWrite(_pinPompe,state);
  delay(wait);
}

void Bras::setEV(bool state)
{
  digitalWrite(_pinEv,state);
}

void Bras::setEV(bool state, int wait)
{
  digitalWrite(_pinEv,state);
  delay(wait);
}
