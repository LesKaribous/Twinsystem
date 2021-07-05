#include "Actuators.h"
#include "Pin.h"
#include "Match.h"
#include "Settings.h"

namespace Actuators{
	Pincer rightPincer(
		Pin::ServoDroit, 
		Pin::ServoVentouseDroit, 
		Pin::PompeDroit,
		Pin::EVDroit,
		Setting::PINCER_R_FOLDED,
		Setting::PINCER_R_UNFOLDED,
		Setting::PINCER_R_CLOSED,
		Setting::PINCER_R_OPEN
	);

	Pincer leftPincer(
		Pin::ServoGauche, 
		Pin::ServoVentouseGauche, 
		Pin::PompeGauche,
		Pin::EVGauche,
		Setting::PINCER_L_FOLDED,
		Setting::PINCER_L_UNFOLDED,
		Setting::PINCER_L_CLOSED,
		Setting::PINCER_L_OPEN
	);

	Servo servoDrapeau;
	Servo servoRightArm;
	Servo servoLeftArm;

	void init(){
		// Init des bras
		servoDrapeau.attach(Pin::ServoDrapeau);
  		servoDrapeau.write(0);

		servoRightArm.attach(Pin::ServoBrasDroit);
		servoLeftArm.attach(Pin::ServoBrasGauche);

		servoRightArm.write(Setting::POS_BRAS_D_HAUT);
		servoLeftArm.write(Setting::POS_BRAS_G_HAUT);

		fold();
		open();

		delay(800);
		
		sleep();
		servoRightArm.detach();
		servoLeftArm.detach();

		pinMode(Pin::Beacon, OUTPUT);
		digitalWrite(Pin::Beacon, LOW);
		

	}

	void deployArm(bool state)
	{
		if (TEAM_BLUE){
			servoRightArm.attach(Pin::ServoBrasDroit);
			servoRightArm.write( state ? Setting::POS_BRAS_D_BAS : Setting::POS_BRAS_D_HAUT );
			servoRightArm.detach();
		}else{
			servoLeftArm.attach(Pin::ServoBrasGauche);
			servoLeftArm.write(state ? Setting::POS_BRAS_G_BAS : Setting::POS_BRAS_G_HAUT);
			servoLeftArm.detach();
		}
		WAIT(800);
	}
		
	void grab(Side side){
		unfold(side);
		WAIT(1000);
		suck(side);
		close(side);
		WAIT(1000);
		fold(side);
	}

	void release(Side side){
		unfold(side);
		WAIT(1000);
		unsuck(side);
		open(side);
		WAIT(1000);
		fold(side);
	}

	void fold(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.fold();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.fold();
	}
    void unfold(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.unfold();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.unfold();
	}

    void suck(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.suck();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.suck();
	}
    void unsuck(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.unsuck();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.unsuck();
	}

    void close(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.close();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.close();
	}
    void open(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.open();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.open();
	}

	void sleep(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.sleep();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.sleep();
	}
	void wake(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.wake();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.wake();
	}
	//----------------INIT ACTIONNEUR-------------

	Pincer::Pincer(
		int pinServoPincer, int pinServoVentouse, int pinPompe, int pinEv,
		int foldedPos, int unfolded, int closed, int opened )
	{
		_pinServoPincer 	= pinServoPincer;
		_pinServoVentouse 	= pinServoVentouse;
		_pinPompe 			= pinPompe;
		_pinEv				= pinEv;

		_foldPos 	= foldedPos;
		_unfoldPos 	= unfolded; 
		_closePos 	= closed;
		_openPos 	= opened;

		pinMode(_pinPompe, OUTPUT);
		pinMode(_pinEv, OUTPUT);
	}


	void Pincer::unfold(){
		if(asleep) wake();
		_servoPincer.write(_unfoldPos);
	}

	void Pincer::fold(){
		if(asleep) wake();
		_servoPincer.write(_foldPos);
	}

	void Pincer::close(){
		if(asleep) wake();
		_servoVentouse.write(_closePos);
	}

	void Pincer::open(){
		if(asleep) wake();
		_servoVentouse.write(_openPos);
	}

	void Pincer::suck(){
		digitalWrite(_pinPompe, Setting::PINCER_SUCk);
		digitalWrite(_pinEv, !Setting::PINCER_EV_ON);
	}

	void Pincer::unsuck(){
		digitalWrite(_pinPompe, !Setting::PINCER_SUCk);
		digitalWrite(_pinEv, Setting::PINCER_EV_ON);
		Match::attente(400);
		digitalWrite(_pinEv, !Setting::PINCER_EV_ON);
	}

	
	void Pincer::wake(){
		_servoPincer.attach(_pinServoPincer);
		_servoVentouse.attach(_pinServoVentouse);
		asleep = false;
	}

	void Pincer::sleep(){
		digitalWrite(_pinPompe, !Setting::PINCER_SUCk);
		digitalWrite(_pinEv, !Setting::PINCER_EV_ON);
		_servoPincer.detach();
		_servoVentouse.detach();
		asleep = true;
	}

}