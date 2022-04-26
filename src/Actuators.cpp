#include "Actuators.h"
#include "Pin.h"
#include "Match.h"
#include "Settings.h"
#include "Debugger.h"

namespace Actuators{

	//Bras
    Bras BrasAU;
    Bras BrasInit;
    Bras BrasTirette;

	void init(){

		if(Settings::ROBOT == Settings::PRIMARY ){

			BrasInit	.setPin(Pin::Servo::pinServo02,
								Pin::Servo::pinServo03,
								Pin::Servo::pinServo04,
								Pin::Servo::pinPump01);
			BrasTirette	.setPin(Pin::Servo::pinServo06,
								Pin::Servo::pinServo07,
								Pin::Servo::pinServo08,
								Pin::Servo::pinPump02);
			BrasAU		.setPin(Pin::Servo::pinServo10,
								Pin::Servo::pinServo11,
								Pin::Servo::pinServo12,
								Pin::Servo::pinPump03);
			//Set Limit
			BrasAU			.setLimit(	180,95,			// Elevator
										180,60,			// Arm
										0,180); 		// Tool
			BrasInit		.setLimit(	180,100,		// Elevator
										180,55,			// Arm
										0,180);			// Tool
			BrasTirette		.setLimit(	180,105,		// Elevator
										180,50,			// Arm
										0,180); 		// Tool
			// Set zero position
			BrasAU			.setPosition(0,0,50);
			BrasInit		.setPosition(0,0,50);
			BrasTirette		.setPosition(0,0,50,1000);
			// Wait for match launch
			BrasAU			.detachBras();
			BrasInit		.detachBras();
			BrasTirette		.detachBras();
		}
	}

	void takeElement(Bras robotArm,int location)
	{
		// Arming the arm	
		robotArm 	.setPosition(100,100,80,1000);
		robotArm	.grab();
		switch(location){
			case FLOOR :
				// Take an element lay on the floor
				robotArm .setPosition(20,100,80,1000);
			break;
			case DISPENSER :
				// Take an element lay on a 60° dispenser
			break;
			case WORK_SHED :
				// Take an element on the work shed
				robotArm .setPosition(100,70,100,500);
				robotArm .setPosition(60,70,100,1000);
				robotArm .setPosition(100,70,100,500);
				robotArm .setPosition(100,70,80,500);
			break;
			case GALLERY :
				// Take an element from the Gallery
			break;
			case PEDESTAL :
				// Take the statuette from the pedestal
			break;
		}
		robotArm .setPosition(0,0,80,500);
	}

	Bras::Bras()
	{

	}

	Bras::~Bras()
	{

	}

	void Bras::setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinPump)
	{
		_pinServoElevator = pinServoElevator ;
		_pinServoArm      = pinServoArm ;
		_pinServoTool     = pinServoTool ;
		_pinPump          = pinPump ;

		_servoElevator.attach(_pinServoElevator);
		_servoArm.attach(_pinServoArm);
		_servoTool.attach(_pinServoTool);

		pinMode(_pinPump,OUTPUT);
		ungrab();
	}

	void Bras::setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinPump, int pinEv)
	{
		setPin(pinServoElevator, pinServoArm, pinServoTool, pinPump);
		_pinEv = pinEv ;
		_pinEvAvailable = true ;
	}

	void Bras::setLimit( int minServoElevator, int maxServoElevator, int minServoArm, int maxServoArm, int minServoTool, int maxServoTool )
	{
		_minServoElevator = minServoElevator;
		_maxServoElevator = maxServoElevator;

		_minServoArm = minServoArm ;
		_maxServoArm = maxServoArm ;

		_minServoTool = minServoTool;
		_maxServoTool = maxServoTool;
	}

	void Bras::setLimitMin(int servoToLimit, int valueToLimit){
		switch(servoToLimit){
			case SERVO_ELEVATOR :
				_minServoElevator = valueToLimit;
			break;
			case SERVO_ARM :
				_minServoArm = valueToLimit;
			break;
			case SERVO_TOOL :
				_minServoTool = valueToLimit;
			break;
		}
	}

	void Bras::setLimitMax(int servoToLimit, int valueToLimit){
		switch(servoToLimit){
			case SERVO_ELEVATOR :
				_maxServoElevator = valueToLimit;
			break;
			case SERVO_ARM :
				_maxServoArm = valueToLimit;
			break;
			case SERVO_TOOL :
				_maxServoTool = valueToLimit;
			break;
		}
	}

	int Bras::getMin(int servo){
		switch(servo){
			case SERVO_ELEVATOR :
				return _minServoElevator;
			break;
			case SERVO_ARM :
				return _minServoArm;
			break;
			case SERVO_TOOL :
				return _minServoTool;
			break;
		}
	}

	int Bras::getMax(int servo){
		switch(servo){
			case SERVO_ELEVATOR :
				return _maxServoElevator;
			break;
			case SERVO_ARM :
				return _maxServoArm;
			break;
			case SERVO_TOOL :
				return _maxServoTool;
			break;
		}
	}

	void Bras::detachBras()
	{
		_servoElevator.detach();
		_servoArm.detach();
		_servoTool.detach();
	}

	void Bras::attachBras()
	{
		_servoElevator.attach(_pinServoElevator);
		_servoArm.attach(_pinServoArm);
		_servoTool.attach(_pinServoTool);
	}

	int Bras::calcPositionElevator(byte posServoElevator)
	{
		// limiter la commande entre 0 et 100%
		int newPosElevator = constrain(posServoElevator,0,100);
		// Calculer la position servo en fonction des limites min max
		newPosElevator = map(newPosElevator,0,100,_minServoElevator,_maxServoElevator);
		return newPosElevator;
	}

	int Bras::calcPositionArm(byte posServoArm)
	{
		// limiter la commande entre 0 et 100%
		int newPosArm = constrain(posServoArm,0,100);
		// Calculer la position servo en fonction des limites min max
		newPosArm = map(newPosArm,0,100,_minServoArm,_maxServoArm);
		return newPosArm;
	}

	int Bras::calcPositionTool(byte posServoTool)
	{
		// limiter la commande entre 0 et 100%
		int newPosTool = constrain(posServoTool,0,100);
		// Calculer la position servo en fonction des limites min max
		newPosTool = map(newPosTool,0,100,_minServoTool,_maxServoTool);
		return newPosTool;
	}

	void Bras::setPosition(byte posServoElevator, byte posServoArm, byte posServoTool)
	{
		// Appliquer la position
		setElevator(posServoElevator);
		setArm(posServoArm);
		setTool(posServoTool);
	}

	void Bras::setPosition(byte posServoElevator, byte posServoArm, byte posServoTool, int wait)
	{
		setPosition(posServoElevator, posServoArm, posServoTool);
		delay(wait);
	}

	void Bras::setElevator    (byte posServoElevator)
	{
		if(!_servoElevator.attached()) _servoElevator.attach(_pinServoElevator);
		_servoElevator.write(calcPositionElevator(posServoElevator));
	}

	void Bras::setElevator    (byte posServoElevator, int wait)
	{
		setElevator(posServoElevator);
		delay(wait);
	}

	void Bras::setArm (byte posServoArm)
	{
		if(!_servoArm.attached()) _servoArm.attach(_pinServoArm);
		_servoArm.write(calcPositionArm(posServoArm));
	}

	void Bras::setArm (byte posServoArm, int wait)
	{
		setArm(posServoArm);
		delay(wait);
	}

	void Bras::setTool (byte posServoTool)
	{
		if(!_servoTool.attached()) _servoTool.attach(_pinServoTool);
		_servoTool.write(calcPositionTool(posServoTool));
	}

	void Bras::setTool (byte posServoTool, int wait)
	{
		setTool(posServoTool);
		delay(wait);
	}

	void Bras::grab()
	{
		digitalWrite(_pinPump,LOW);
		//Debugger::log("ON pump");
	}

	void Bras::grab(int wait)
	{
		grab();
		delay(wait);
	}

	void Bras::ungrab()
	{
		digitalWrite(_pinPump,HIGH);
		//Debugger::log("OFF pump");
	}

	void Bras::ungrab(int wait)
	{
		ungrab();
		delay(wait);
	}



}