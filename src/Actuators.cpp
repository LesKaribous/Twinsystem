#include "Actuators.h"
#include "Pin.h"
#include "Match.h"
#include "Settings.h"

namespace Actuators
{

	Bras brasDroit;
	Bras brasGauche;

	Servo servoDrapeau;
	Servo servoBrasDroit;
	Servo servoBrasGauche;

	void init()
	{
		prepare();
		/*
		brasDroit.setPosition(0, 0, LOW, LOW, 0);
		brasGauche.setPosition(0, 0, LOW, LOW, 2000);

		brasDroit.setPosition(100, 100, HIGH, LOW, 0);
		brasGauche.setPosition(100, 100, HIGH, LOW, 2000);

		brasDroit.setPosition(0, 0, LOW, LOW, 0);
		brasGauche.setPosition(0, 0, LOW, LOW, 2000);

		servoBrasDroit.attach(Pin::ServoBrasDroit);
		servoBrasGauche.attach(Pin::ServoBrasGauche);

		initBrasMancheAir();

		servoBrasDroit.detach();
		servoBrasGauche.detach();
		*/
	}

	void prepare()
	{
		// Init des bras
		initBrasMancheAir();

		// Init du drapeau
		servoDrapeau.attach(Pin::ServoDrapeau);
		servoDrapeau.write(0);

		// Initialisation des bras
		brasDroit.setPin(Pin::ServoDroit, Pin::ServoVentouseDroit, Pin::PompeDroit, Pin::EVDroit);
		brasGauche.setPin(Pin::ServoGauche, Pin::ServoVentouseGauche, Pin::PompeGauche, Pin::EVGauche);
		// Initialisation de la pin pinBalise
		pinMode(Pin::Beacon, OUTPUT);
		digitalWrite(Pin::Beacon, LOW);

		brasDroit.setLimit(35, 140, 10, 90);
		brasGauche.setLimit(110, 10, 180, 70);
		brasDroit.setPosition(0, 0, LOW, LOW, 0);
		brasGauche.setPosition(0, 0, LOW, LOW, 2000);

		servoDrapeau.detach();
		servoBrasDroit.detach();
		servoBrasGauche.detach();
	}

	void brasVentouse(BrasVentouse stateBras)
	{
		switch (stateBras)
		{
		case BrasVentouse::PRISE_DROITE:
			brasDroit.setPosition(100, 100, HIGH, LOW, 0);
			Match::attente(200);
			brasDroit.setPosition(0, 0, HIGH, LOW, 0);
			Match::attente(200);
			break;
		case BrasVentouse::PRISE_GAUCHE:
			brasGauche.setPosition(100, 100, HIGH, LOW, 0);
			Match::attente(200);
			brasGauche.setPosition(0, 0, HIGH, LOW, 0);
			Match::attente(200);
			break;
		case BrasVentouse::DEPOSE_DROITE:
			brasDroit.setPosition(100, 100, HIGH, LOW, 0);
			Match::attente(200);
			brasDroit.setPosition(100, 100, LOW, HIGH, 0);
			Match::attente(200);
			brasDroit.setPosition(0, 0, LOW, LOW, 0);
			Match::attente(200);
			break;
		case BrasVentouse::DEPOSE_GAUCHE:
			brasGauche.setPosition(100, 100, HIGH, LOW, 0);
			Match::attente(200);
			brasGauche.setPosition(100, 100, LOW, HIGH, 0);
			Match::attente(200);
			brasGauche.setPosition(0, 0, LOW, LOW, 0);
			Match::attente(200);
			break;
		case BrasVentouse::PRISE_ENSEMBLE:
			brasDroit.setPosition(100, 100, HIGH, LOW, 0);
			brasGauche.setPosition(100, 100, HIGH, LOW, 0);
			Match::attente(200);
			brasDroit.setPosition(0, 0, HIGH, LOW, 0);
			brasGauche.setPosition(0, 0, HIGH, LOW, 0);
			Match::attente(200);
			break;
		case BrasVentouse::DEPOSE_ENSEMBLE:
			brasDroit.setPosition(100, 100, HIGH, LOW, 0);
			brasGauche.setPosition(100, 100, HIGH, LOW, 0);
			Match::attente(200);
			brasDroit.setPosition(100, 100, LOW, HIGH, 0);
			brasGauche.setPosition(100, 100, LOW, HIGH, 0);
			Match::attente(200);
			brasDroit.setPosition(0, 0, LOW, LOW, 0);
			brasGauche.setPosition(0, 0, LOW, LOW, 0);
			Match::attente(200);
			break;
		case BrasVentouse::REPOS:
			brasDroit.setPosition(0, 0, LOW, LOW, 0);
			brasGauche.setPosition(0, 0, LOW, LOW, 0);
			break;
		}
	}

	void initBrasMancheAir()
	{
		servoBrasDroit.attach(Pin::ServoBrasDroit);
		servoBrasGauche.attach(Pin::ServoBrasGauche);

		servoBrasDroit.write(Setting::POS_BRAS_D_BAS);
		servoBrasGauche.write(Setting::POS_BRAS_G_BAS);

		delay(800);

		servoBrasDroit.write(Setting::POS_BRAS_D_HAUT);
		servoBrasGauche.write(Setting::POS_BRAS_G_HAUT);

		delay(800);
		servoBrasDroit.detach();
		servoBrasGauche.detach();
	}

	void brasMancheAir(bool state)
	{
		if (TEAM_BLUE)
			servoBrasDroit.attach(Pin::ServoBrasDroit);
		else
			servoBrasGauche.attach(Pin::ServoBrasGauche);

		if (state)
		{
			if (TEAM_BLUE)
				servoBrasDroit.write(Setting::POS_BRAS_D_BAS);
			else
				servoBrasGauche.write(Setting::POS_BRAS_G_BAS);
		}
		else
		{
			if (TEAM_BLUE)
				servoBrasDroit.write(Setting::POS_BRAS_D_HAUT);
			else
				servoBrasGauche.write(Setting::POS_BRAS_G_HAUT);
		}
		Match::attente(800);
		if (TEAM_BLUE)
			servoBrasDroit.detach();
		if (TEAM_YELLOW)
			servoBrasGauche.detach();
	}

	//----------------INIT ACTIONNEUR-------------

	void Bras::setPin(int pinServoBras, int pinServoVentouse, int pinPompe, int pinEv)
	{
		_pinServoBras = pinServoBras;
		_pinServoVentouse = pinServoVentouse;
		_pinPompe = pinPompe;
		_pinEv = pinEv;

		_servoBras.detach();
		_servoVentouse.detach();

		_servoBras.attach(_pinServoBras);
		_servoVentouse.attach(_pinServoVentouse);

		pinMode(_pinPompe, OUTPUT);
		pinMode(_pinEv, OUTPUT);
	}

	void Bras::setPin(int pinServoBras, int pinServoVentouse, int pinPompe, int pinEv, int pinAscenseur, int pinPotard)
	{
		setPin(pinServoBras, pinServoVentouse, pinPompe, pinEv);

		_pinAscenseur = pinAscenseur;
		_pinPotard = pinPotard;
	}

	void Bras::setLimit(int minServoBras, int maxServoBras, int minServoVentouse, int maxServoVentouse)
	{
		_minServoBras = minServoBras;
		_maxServoBras = maxServoBras;
		_minServoVentouse = minServoVentouse;
		_maxServoVentouse = maxServoVentouse;
	}

	void Bras::setLimit(int minServoBras, int maxServoBras, int minServoVentouse, int maxServoVentouse, int stopAscenseur, int minAscenseur, int maxAscenseur)
	{
		setLimit(minServoBras, maxServoBras, minServoVentouse, maxServoVentouse);
		_stopAscenseur = stopAscenseur;
		_minAscenseur = minAscenseur;
		_maxAscenseur = maxAscenseur;
	}

	int Bras::calcPositionBras(byte posServoBras)
	{
		// limiter la commande entre 0 et 100%
		int newPosBras = constrain(posServoBras, 0, 100);
		// Calculer la position servo en fonction des limites min max
		newPosBras = map(newPosBras, 0, 100, _minServoBras, _maxServoBras);
		return newPosBras;
	}

	int Bras::calcPositionVentouse(byte posServoVentouse)
	{
		// limiter la commande entre 0 et 100%
		int newPosVentouse = constrain(posServoVentouse, 0, 100);
		// Calculer la position servo en fonction des limites min max
		newPosVentouse = map(newPosVentouse, 0, 100, _minServoVentouse, _maxServoVentouse);
		return newPosVentouse;
	}

	int Bras::calcPositionAscenseur(byte posAscenseur)
	{
		// limiter la commande entre 0 et 100%
		int newPosAscenseur = constrain(posAscenseur, 0, 100);
		// Calculer la position servo en fonction des limites min max
		newPosAscenseur = map(newPosAscenseur, 0, 100, _minAscenseur, _maxAscenseur);
		return newPosAscenseur;
	}

	void Bras::setPosition(byte posServoBras, byte posServoVentouse, bool pompe, bool ev, int wait)
	{
		// Appliquer la position
		_servoBras.write(calcPositionBras(posServoBras));
		_servoVentouse.write(calcPositionVentouse(posServoVentouse));

		// Appliquer la commande de pompe/EV
		digitalWrite(_pinPompe, pompe);
		digitalWrite(_pinEv, ev);

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

		while (Input > (posAscenseur + 2) || Input < (posAscenseur - 2))
		{
			Input = analogRead(_pinPotard);
			if (Input > posAscenseur)
				_servoAscenseur.write(_stopAscenseur - 12);
			else
				_servoAscenseur.write(_stopAscenseur + 12);
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
		digitalWrite(_pinPompe, state);
	}

	void Bras::setPompe(bool state, int wait)
	{
		digitalWrite(_pinPompe, state);
		delay(wait);
	}

	void Bras::setEV(bool state)
	{
		digitalWrite(_pinEv, state);
	}

	void Bras::setEV(bool state, int wait)
	{
		digitalWrite(_pinEv, state);
		delay(wait);
	}

}