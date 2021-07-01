#include "Intercom.h"
#include "Settings.h"

#include <Arduino.h>
#include <FastCRC8.h>
#include <Wire.h>

using namespace Intercom;

FastCRC8 _CRC8;

int navError = 0; // Nombre d'erreur de com avec la navigation

bool 
_team = Setting::EQUIPE_BLEU, 
_globalDetection 	= false, 
_detectionState 	= false, 
_recalibrationState = false, 
_speedState 		= false;

int _nbrBadCRC = 0;  // Nombre de CRC érronés
int _nbrBizarre = 0; // Nombre de réponses bizarres

byte _bufNavRelatif[5] = {0, 0, 0, 0, 0}; // Buffer d'envoi des ordres de navigation relatifs
byte _crcNavRelatif = 0;                  // CRC de controle pour les ordres de navigation relatifs

byte _bufNavAbsolu[6] = {0, 0, 0, 0, 0, 0}; // Buffer d'envoi des ordres de navigation absolus
byte _crcNavAbsolu = 0;                     // CRC de controle pour les ordres de navigation absolus




void Intercom::init(){
    Wire.begin(); //Demarrage de la liaison I2C
}

void Intercom::setTeam(bool team){
  _team = team ;
}
void Intercom::setGlobalDetection(bool globalDetection){
  _globalDetection = globalDetection ;
}
void Intercom::setDetection(bool detection){
  _detectionState = detection ;
}
void Intercom::setRecalibration(bool recalibration){
  _recalibrationState = recalibration ;
}
void Intercom::slowMode(bool speed){
  _speedState = speed ;
}
int Intercom::getNbrBadCRC(){
  return _nbrBadCRC;
}

//----------------DEMANDE L'ETAT DU DEPLACEMENT----------------
int Intercom::askNavigation()
{
  int etatNavigation ;
  char reponseNavigation ;
  Wire.requestFrom(Setting::PILOT_ADRESS, 1);
  while(Wire.available())
  {
    reponseNavigation = Wire.read();
  }
  // Serial.print("repNav:");
  // Serial.println(reponseNavigation);
  if (reponseNavigation=='N') etatNavigation = _RECU ;
  else if (reponseNavigation=='O') etatNavigation = _TERMINEE ;
  else if (reponseNavigation=='E') etatNavigation = _ERRONEE ;
  else reponseNavigation = _BIZARRE ;
	return etatNavigation;
}

//----------------ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU----------------
void Intercom::sendNavigation(byte fonction, int X, int Y, int rot)
{
	// Stockage des valeurs à envoyer dans le buffer
  _bufNavAbsolu[0]=fonction;
	_bufNavAbsolu[1]=X >> 8;
	_bufNavAbsolu[2]=X & 255;
	_bufNavAbsolu[3]=Y >> 8;
	_bufNavAbsolu[4]=Y & 255;
  _bufNavAbsolu[5]=rot >> 8;
  _bufNavAbsolu[6]=rot & 255;

	// Calcul du CRC
	_crcNavRelatif = _CRC8.smbus(_bufNavAbsolu, sizeof(_bufNavAbsolu));
	//Serial.println(_crcNavRelatif);

	// Envoi des données
	Wire.beginTransmission(Setting::PILOT_ADRESS);
	for(int i=0;i<=6;i++)
	{
		Wire.write(_bufNavRelatif[i]);
	}
	//Wire.write(crcNavRelatif);
	Wire.endTransmission();
}

//----------------ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF----------------
void Intercom::sendNavigation(byte fonction, int rot, int dist)
{
	if ( _team == Setting::EQUIPE_JAUNE ) rot = -rot ;
	// Stockage des valeurs à envoyer dans le buffer
	_bufNavRelatif[0]=fonction;
	_bufNavRelatif[1]=rot >> 8;
	_bufNavRelatif[2]=rot & 255;
	_bufNavRelatif[3]=dist >> 8;
	_bufNavRelatif[4]=dist & 255;
	// Calcul du CRC
	_crcNavRelatif = _CRC8.smbus(_bufNavRelatif, sizeof(_bufNavRelatif));
	//Serial.println(_crcNavRelatif);
	// Envoi des données
	Wire.beginTransmission(Setting::PILOT_ADRESS);
	for(int i=0;i<=4;i++)
	{
		Wire.write(_bufNavRelatif[i]);
	}
	Wire.write(_crcNavRelatif);
	Wire.endTransmission();
}

//----------------ENVOI UNE COMMANDE TURN GO----------------
void Intercom::turnGo(int turn, int go)
{
  turnGo(_detectionState,_recalibrationState,_speedState,turn, go);
}

void Intercom::turnGo(bool detection, bool recalibration,bool speed,int turn, int go)
{
  bool optionDetection = _globalDetection || detection;
  byte optionNavigation = 0;

	bitWrite(optionNavigation,0,optionDetection);
	bitWrite(optionNavigation,1,recalibration);
	bitWrite(optionNavigation,2,speed);
	sendNavigation(optionNavigation, turn, go);

  setDetection(detection);
  setRecalibration(recalibration);
  slowMode(speed);
}

//----------------ENVOI UNE COMMANDE GO TO-----------------
void Intercom::goTo(int X, int Y, int rot)
{
  goTo(_detectionState,_recalibrationState,_speedState,X, Y, rot);
}
void Intercom::goTo(bool detection, bool recalibration,bool speed, int X, int Y, int rot)
{
  bool optionDetection = _globalDetection || detection;
  byte optionNavigation = 0;

	bitWrite(optionNavigation,0,optionDetection);
	bitWrite(optionNavigation,1,recalibration);
	bitWrite(optionNavigation,2,speed);
	sendNavigation(optionNavigation, X, Y, rot);

  setDetection(detection);
  setRecalibration(recalibration);
  slowMode(speed);
}
