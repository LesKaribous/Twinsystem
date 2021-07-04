#include "Actuators.h"
#include "Pin.h"
#include "Match.h"
#include "Settings.h"

namespace Actuators
{

	Pincer brasDroit;
	Pincer brasGauche;

	Servo servoDrapeau;
	Servo servoPincerDroit;
	Servo servoPincerGauche;

	void init(){
		// Init des bras
		initBras();
		// Initialisation de la pin pinBalise
		pinMode(Pin::Beacon, OUTPUT);
		digitalWrite(Pin::Beacon, LOW);

		//brasDroit.setLimit(35, 140, 10, 90);
		//brasGauche.setLimit(110, 10, 180, 70);

		//servoDrapeau.detach();
		//servoBrasDroit.detach();
		//servoBrasGauche.detach();
	}


	void initBras()
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

	Pincer::Pincer(int pinServoPincer, int pinServoVentouse, int pinPompe, int pinEv)
	{
		_pinServoPincer = pinServoPincer;
		_pinServoVentouse = pinServoVentouse;
		_pinPompe = pinPompe;
		_pinEv = pinEv;

		_servoPincer.detach();
		_servoVentouse.detach();

		_servoPincer.attach(_pinServoPincer);
		_servoVentouse.attach(_pinServoVentouse);

		pinMode(_pinPompe, OUTPUT);
		pinMode(_pinEv, OUTPUT);
	}


	void Pincer::unfold(){
		_servoPincer.write(_unfoldPos);
	}

	void Pincer::fold(){
		_servoPincer.write(_foldPos);
	}

	void Pincer::close(){
		_servoVentouse.write(_unfoldPos);
	}

	void Pincer::open(){
		_servoVentouse.write(_foldPos);
	}

	void Pincer::suck(){
		digitalWrite(_pinPompe, Setting::PINCER_SUCk);
		digitalWrite(_pinEv, !Setting::PINCER_EV_ON);
	}

	void Pincer::unsuck(){
		digitalWrite(_pinPompe, Setting::PINCER_SUCk);
		digitalWrite(_pinEv, Setting::PINCER_EV_ON);
	}

	
	void Pincer::wake(){
		_servoPincer.attach(_pinServoPincer);
		_servoVentouse.attach(_pinServoVentouse);
	}

	void Pincer::sleep(){
		digitalWrite(_pinPompe, !Setting::PINCER_SUCk);
		digitalWrite(_pinEv, !Setting::PINCER_EV_ON);
		_servoPincer.detach();
		_servoVentouse.detach();
	}

}