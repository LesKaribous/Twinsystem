#include "Motion.h"
#include "Controller.h"
#include "Match.h"

namespace Motion
{
	Vec2 position = {0,0};

	//----------------ENVOI UNE COMMANDE TURN GO----------------
	void go(Vec2 target){
		Vec2 move = target.sub(position);
		Controller::go(go);
	}

	void goTo(int X, int Y, int rot) //undefined behavior
	{
		Intercom::goTo(X, Y, rot);
	}

	void turnGo(bool adversaire, bool recalage, bool ralentit, int X, int Y, int rot)
	{
		Intercom::goTo(adversaire, recalage, ralentit, X, Y, rot);
		int reponseNavigation = Intercom::askNavigation();
		while (reponseNavigation != TERMINEE)
		{
			if (reponseNavigation == ERRONEE)
			{
				Intercom::goTo(adversaire, recalage, ralentit, X, Y, rot);
				Intercom::badCRC();
			}
			Match::attente(200);
			reponseNavigation = Intercom::askNavigation();
		}
	}
}
