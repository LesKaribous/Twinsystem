#include "Actuators.h"
#include "Pin.h"
#include "Match.h"
#include "Settings.h"

namespace Actuators{

	/*
	Pincer rightPincer(
		Pin::ServoDroit, 
		Pin::ServoVentouseDroit, 
		Pin::PompeDroit,
		Pin::EVDroit,
		Settings::PINCER_R_FOLDED,
		Settings::PINCER_R_UNFOLDED,
		Settings::PINCER_R_ARMED,
		Settings::PINCER_R_CLOSED,
		Settings::PINCER_R_OPEN
	);

	Pincer leftPincer(
		Pin::ServoGauche, 
		Pin::ServoVentouseGauche, 
		Pin::PompeGauche,
		Pin::EVGauche,
		Settings::PINCER_L_FOLDED,
		Settings::PINCER_L_UNFOLDED,
		Settings::PINCER_L_ARMED,
		Settings::PINCER_L_CLOSED,
		Settings::PINCER_L_OPEN
	);

	Servo servoDrapeau;
	Servo servoRightArm;
	Servo servoLeftArm;

	bool BrasState = false;

	void init(){
		// Init des bras
		servoDrapeau.attach(Pin::ServoDrapeau);
  		servoDrapeau.write(0);

		servoRightArm.attach(Pin::ServoBrasDroit);
		servoLeftArm.attach(Pin::ServoBrasGauche);

		servoRightArm.write(Settings::POS_BRAS_D_HAUT);
		servoLeftArm.write(Settings::POS_BRAS_G_HAUT);

		fold();
		open();

		delay(800);
		
		sleep();
		servoRightArm.detach();
		servoLeftArm.detach();

		pinMode(Pin::Beacon, OUTPUT);
		digitalWrite(Pin::Beacon, HIGH);
		WAIT(1000);
		digitalWrite(Pin::Beacon, LOW);
		

	}

	void deployArm(bool state, bool push, Side side)
	{
		if ((TEAM_BLUE && side == Side::BOTH) || side == Side::RIGHT){
			servoRightArm.attach(Pin::ServoBrasDroit);
			servoRightArm.write( state ? Settings::POS_BRAS_D_BAS : Settings::POS_BRAS_D_HAUT );
			if(push) servoRightArm.write( Settings::POS_BRAS_D_PUSH);
			WAIT(800);
			servoRightArm.detach();
		}else{
			servoLeftArm.attach(Pin::ServoBrasGauche);
			servoLeftArm.write(state ? Settings::POS_BRAS_G_BAS : Settings::POS_BRAS_G_HAUT);
			if(push) servoLeftArm.write( Settings::POS_BRAS_G_PUSH);
			WAIT(800);
			servoLeftArm.detach();
		}
	}
		

	void fold(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.fold();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.fold();
	}
    void unfold(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.unfold();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.unfold();
	}
	void arming(Side s){
		if(s == Side::RIGHT || s == Side::BOTH) rightPincer.arming();
		if(s == Side::LEFT  || s == Side::BOTH) leftPincer.arming();
		open(s);
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
		int foldedPos, int unfolded, int arming,  int closed, int opened )
	{
		_pinServoPincer 	= pinServoPincer;
		_pinServoVentouse 	= pinServoVentouse;
		_pinPompe 			= pinPompe;
		_pinEv				= pinEv;

		_foldPos 	= foldedPos;
		_armingPos 	= arming;
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

	void Pincer::arming(){
		if(asleep) wake();
		_servoPincer.write(_armingPos);
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
		digitalWrite(_pinPompe, Settings::PINCER_SUCk);
		digitalWrite(_pinEv, !Settings::PINCER_EV_ON);
	}

	void Pincer::unsuck(){
		digitalWrite(_pinPompe, !Settings::PINCER_SUCk);
		digitalWrite(_pinEv, Settings::PINCER_EV_ON);
		Match::attente(700);
		digitalWrite(_pinEv, !Settings::PINCER_EV_ON);
	}

	
	void Pincer::wake(){
		_servoPincer.attach(_pinServoPincer);
		_servoVentouse.attach(_pinServoVentouse);
		asleep = false;
	}

	void Pincer::sleep(){
		digitalWrite(_pinPompe, !Settings::PINCER_SUCk);
		digitalWrite(_pinEv, !Settings::PINCER_EV_ON);
		_servoPincer.detach();
		_servoVentouse.detach();
		asleep = true;
	}
*/
}