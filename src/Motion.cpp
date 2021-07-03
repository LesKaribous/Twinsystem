#include "Motion.h"
#include "Match.h"
#include "Intercom.h"

namespace Motion
{
	//----------------ENVOI UNE COMMANDE TURN GO----------------
	void turnGo(int turn, int go)
	{
		Intercom::turnGo(turn, go);
		Match::attente(100);

		int reponseNavigation = Intercom::askNavigation();
		while (reponseNavigation != TERMINEE)
		{
			if (reponseNavigation == ERRONEE || reponseNavigation == BIZARRE)
			{
				Intercom::turnGo(turn, go);
				Intercom::badCRC();
			}
			Match::attente(200);
			reponseNavigation = Intercom::askNavigation();
		}
	}

	void turnGo(bool adversaire, bool recalage, bool ralentit, int turn, int go)
	{
		Intercom::turnGo(adversaire, recalage, ralentit, turn, go);
		Match::attente(100);
		int reponseNavigation = Intercom::askNavigation();
		while (reponseNavigation != TERMINEE)
		{
			if (reponseNavigation == ERRONEE || reponseNavigation == BIZARRE)
			{
				Intercom::turnGo(adversaire, recalage, ralentit, turn, go);
				Intercom::badCRC();
			}
			Match::attente(200);
			reponseNavigation = Intercom::askNavigation();
		}
	}

	void goTo(int X, int Y, int rot)
	{
		Intercom::goTo(X, Y, rot);
		int reponseNavigation = Intercom::askNavigation();
		while (reponseNavigation != TERMINEE)
		{
			if (reponseNavigation == ERRONEE)
			{
				Intercom::goTo(X, Y, rot);
				Intercom::badCRC();
			}
			Match::attente(200);
			reponseNavigation = Intercom::askNavigation();
		}
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
