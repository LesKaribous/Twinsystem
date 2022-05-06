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

	Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

	void init(){

		pwm.begin();
		pwm.setPWMFreq(1000);
		Wire.setClock(400000);

		Settings::init(); // To solve the "type robot issue"

		if(Settings::ROBOT == Settings::PRIMARY ){

			BrasInit	.setPin(Pin::Servo::pinServo02,
								Pin::Servo::pinServo03,
								Pin::Servo::pinServo04,
								Pin::Pump::pinPump01,
								Pin::Pump::pinEv01);
			BrasTirette	.setPin(Pin::Servo::pinServo06,
								Pin::Servo::pinServo07,
								Pin::Servo::pinServo08,
								Pin::Pump::pinPump02,
								Pin::Pump::pinEv02);
			BrasAU		.setPin(Pin::Servo::pinServo10,
								Pin::Servo::pinServo11,
								Pin::Servo::pinServo12,
								Pin::Pump::pinPump03,
								Pin::Pump::pinEv03);
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
			//Set Geometry
			BrasAU		.setGeometry();
			BrasInit		.setGeometry();
			BrasTirette	.setGeometry();

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

	void sleep(){
		BrasAU			.detachBras();
		BrasInit		.detachBras();
		BrasTirette		.detachBras();
	}

	void unsuck(){
		BrasAU			.ungrab();
		BrasInit		.ungrab();
		BrasTirette		.ungrab();
	}

	Bras::Bras()
	{

	}

	Bras::~Bras()
	{

	}

	void Bras::setPin(int pinServoElevator, int pinServoArm, int pinServoTool)
	{
		_pinServoElevator = pinServoElevator ;
		_pinServoArm      = pinServoArm ;
		_pinServoTool     = pinServoTool ;
		
		//_servoElevator.attach(_pinServoElevator);
		//_servoArm.attach(_pinServoArm);
		//_servoTool.attach(_pinServoTool);

	}

	void Bras::setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinPump, int pinEv)
	{
		setPin(pinServoElevator, pinServoArm, pinServoTool);

		_pinPump = pinPump ;
		_pinEv = pinEv ;

		pinMode(_pinPump,OUTPUT);
		pinMode(_pinEv,OUTPUT);

		ungrab();
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

	void Bras::setGeometry(int angle, int centerDistance){
		_angle = angle;
		_centerDistance = centerDistance;
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

	void Bras::setPosition(byte posServoElevator, byte posServoArm, byte posServoTool, int wait)
	{
		// Appliquer la position
		setElevator(posServoElevator);
		setArm(posServoArm);
		setTool(posServoTool);
		Match::wait(wait);
	}

	void Bras::setElevator    (byte posServoElevator, int wait)
	{
		if(!_servoElevator.attached()) 
			_servoElevator.attach(_pinServoElevator);
		_servoElevator.write(calcPositionElevator(posServoElevator));
		Match::wait(wait);
	}

	void Bras::setArm (byte posServoArm, int wait)
	{
		if(!_servoArm.attached()) 
			_servoArm.attach(_pinServoArm);
		_servoArm.write(calcPositionArm(posServoArm));
		Match::wait(wait);
	}

	void Bras::setTool (byte posServoTool, int wait)
	{
		if(!_servoTool.attached()) 
			_servoTool.attach(_pinServoTool);
		_servoTool.write(calcPositionTool(posServoTool));
		Match::wait(wait);
	}

	void Bras::enablePump (bool state){
		if(!state) pwm.setPWM(_pinPump, 4096, 0);
		else pwm.setPWM(_pinPump, 0, 4096);
	}

    void Bras::openEv     (bool state){
		if(!state) pwm.setPWM(_pinEv, 4096, 0);
		else pwm.setPWM(_pinEv, 0, 4096);
	}

	void Bras::grab(int wait)
	{
		enablePump();
		openEv(false);

		Match::wait(wait);
	}

	void Bras::ungrab(int wait)
	{
		enablePump(false);
		openEv();
		Match::wait(500);
		openEv(false);

		Match::wait(wait);
	}

}