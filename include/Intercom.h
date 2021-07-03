#pragma once

typedef unsigned char byte; 

namespace Intercom{

  void init();

  void setTeam(bool team);
  void setGlobalDetection(bool globalDetection);
  void setDetection(bool detection);
  void setRecalibration(bool recalibration);
  void slowMode(bool speed);

  int getNbrBadCRC();
  void badCRC();

  //DEMANDE L'ETAT DU DEPLACEMENT
  int askNavigation();
  //ENVOI UNE COMMANDE TURN GO
  void turnGo(int turn, int go);
  void turnGo(bool detection, bool recalibration, bool speed, int turn, int go);
  //ENVOI UNE COMMANDE GOTO
  void goTo(int X, int Y, int rot);
  void goTo(bool detection, bool recalibration, bool speed, int X, int Y, int rot);
  //ENVOI UNE COMMANDE DE DEPLACEMENT ABSOLU
  void sendNavigation(byte fonction, int X, int Y, int rot);
  //ENVOI UNE COMMANDE DE DEPLACEMENT RELATIF
  void sendNavigation(byte fonction, int rot, int dist);

  const byte _TERMINEE = 0; // Position validée et terminée
  const byte _RECU = 1;     // Position reçu
  const byte _ERRONEE = 2;  // Position erronée. CRC nok.
  const byte _BIZARRE = 3;  // Reponse étrange à ne pas prendre en compte

};
