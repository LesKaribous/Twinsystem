#pragma once

//Etat de la position demandée
#define TERMINEE 0  // Position validée et terminée
#define RECU 1      // Position reçu
#define ERRONEE 2   // Position erronée. CRC nok.
#define BIZARRE 3   // Reponse étrange à ne pas prendre en compte

namespace Motion{

    void sequenceRecalage();

    //----- COMMANDES DE DEPLACEMENT -----
    void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go);
    void turnGo(int turn, int go);
    void goTo(bool adversaire, bool recalage,bool ralentit,int X, int Y, int rot);
    void goTo(int X, int Y, int rot);
}