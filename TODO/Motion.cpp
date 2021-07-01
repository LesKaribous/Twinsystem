#include "Motion.h"


//----------------ENVOI UNE COMMANDE TURN GO----------------
void turnGo(int turn, int go)
{
  nav.turnGo(turn,go);
  attente(100);
  int reponseNavigation = nav.askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE || reponseNavigation==BIZARRE)
    {
      nav.turnGo(turn,go);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
	}

}

void turnGo(bool adversaire, bool recalage,bool ralentit,int turn, int go)
{
	nav.turnGo(adversaire,recalage,ralentit,turn,go);
  attente(100);
  int reponseNavigation = nav.askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE || reponseNavigation==BIZARRE)
    {
      nav.turnGo(adversaire,recalage,ralentit,turn,go);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
	}

}

void goTo  (int X, int Y, int rot)
{
  nav.goTo(X,Y,rot);
  int reponseNavigation = nav.askNavigation();
	while(reponseNavigation!=TERMINEE)
	{
    if (reponseNavigation==ERRONEE)
    {
      nav.goTo(X,Y,rot);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
	}
}

void turnGo(bool adversaire, bool recalage,bool ralentit,int X, int Y, int rot)
{
  nav.goTo(adversaire,recalage,ralentit,X,Y,rot);
  int reponseNavigation = nav.askNavigation();
  while(reponseNavigation!=TERMINEE)
  {
    if (reponseNavigation==ERRONEE)
    {
      nav.goTo(adversaire,recalage,ralentit,X,Y,rot);
      navError++;
    }
    attente(200);
    reponseNavigation = nav.askNavigation();
  }
}
