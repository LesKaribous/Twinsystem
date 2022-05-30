#include "Actuators.h"

#include "Debugger.h"
#include "Twinsystem.h"

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


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

		
		BrasInit	.setPin(Pin::Servo::pinServo02,
							Pin::Servo::pinServo03,
							Pin::Servo::pinServo04,
							Pin::Pump::pinPump01,
							Pin::Pump::pinEv01);
		BrasAU		.setPin(Pin::Servo::pinServo10,
							Pin::Servo::pinServo11,
							Pin::Servo::pinServo12,
							Pin::Pump::pinPump03,
							Pin::Pump::pinEv03);

		if(Settings::primary()){	

			BrasTirette	.setPin(Pin::Servo::pinServo06,
								Pin::Servo::pinServo07,
								Pin::Servo::pinServo08,
								Pin::Pump::pinPump02,
								Pin::Pump::pinEv02);		
			//Set Limit
			BrasAU			.setLimit(	180,95,			// Elevator
										180,55,			// Arm
										0,180); 		// Tool
			BrasInit		.setLimit(	180,100,		// Elevator
										180,55,			// Arm
										0,180);			// Tool
			BrasTirette		.setLimit(	180,105,		// Elevator
										180,50,			// Arm
										0,180); 		// Tool
		}
		else{

			BrasTirette	.setPin(Pin::Servo::pinServo05,
								Pin::Servo::pinServo06,
								Pin::Servo::pinServo07,
								Pin::Servo::pinServo08,
								Pin::Pump::pinPump02,
								Pin::Pump::pinEv02);
			//Set Limit
			BrasAU			.setLimit(	150,67,			// Elevator
										160,25,			// Arm
										0,180); 		// Tool
			BrasInit		.setLimit(	180,105,		// Elevator
										135,15,			// Arm
										0,180);			// Tool
			//Set limit for "statuette" actuators							
			BrasTirette		.setLimit(	180,50,			// Pad
										10,100,			// Arm with succer
										0,82, 			// Testor 1
										180,100); 		// Testor 2
		}
			//Set Geometry
			BrasAU		.setGeometry(-120,0);
			BrasInit	.setGeometry(0,0);
			BrasTirette	.setGeometry(120,0);

			// Set zero position
			BrasAU			.setPosition(0,0,50);
			BrasInit		.setPosition(0,0,50);
			if(Settings::primary())
				BrasTirette		.setPosition(0,0,50,1000);
			else{
				BrasTirette		.setPosition(0,0,0);
				BrasTirette		.setTool2(0,1000);
			}
			// Wait for match launch
			BrasAU			.detachBras();
			BrasInit		.detachBras();
			BrasTirette		.detachBras();
		IHM::addLoad(10);
		IHM::setLoadingMsg("Actuators OK");
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

	int getProbingValue(){
		int value = analogRead(Pin::probeResistor);
		//Debugger::println(value);
		if(value <= RESISTOR_PURPLE + 60) return SQUARE_PURPLE ;
		else if(value <= RESISTOR_YELLOW + 60) return SQUARE_YELLOW ;
		else if(value <= RESISTOR_BAD + 60) return SQUARE_BAD;
		else return SQUARE_ERROR ;
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

	}

	void Bras::setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinPump, int pinEv)
	{
		setPin(pinServoElevator, pinServoArm, pinServoTool);

		_pinPump = pinPump ;
		_pinEv = pinEv ;

		ungrab();
	}

	void Bras::setPin(int pinServoElevator, int pinServoArm, int pinServoTool, int pinServoTool2, int pinPump, int pinEv)
	{
		setPin(pinServoElevator, pinServoArm, pinServoTool);

		_pinServoTool2 = pinServoTool2 ;
		_pinPump = pinPump ;
		_pinEv = pinEv ;

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

	void Bras::setLimit( int minServoElevator, int maxServoElevator, int minServoArm, int maxServoArm, int minServoTool, int maxServoTool ,int minServoTool2, int maxServoTool2 )
	{
		setLimit(minServoElevator, maxServoElevator, minServoArm, maxServoArm, minServoTool, maxServoTool );
		_minServoTool2 = minServoTool2;
		_maxServoTool2 = maxServoTool2;
	}

	void Bras::setGeometry(int angle, int centerDistance){
		_angle = angle;
		_centerDistance = centerDistance;
	}

	void Bras::setAngle(int angle){
		_angle = angle;
	}

	int Bras::GetAngle() const{
		return _angle;
	}

    int Bras::GetDecalage() const{
		return _centerDistance;
	}

	void Bras::updateElement(int elementColor){
		_elementColor = elementColor;
	}

	int Bras::GetElement() const{
		return _elementColor;
	}

	void Bras::detachBras()
	{
		_servoElevator.detach();
		_servoArm.detach();
		_servoTool.detach();
		if (_pinServoTool2 > 0) _servoTool2.detach();
	}

	void Bras::attachBras()
	{
		_servoElevator.attach(_pinServoElevator);
		_servoArm.attach(_pinServoArm);
		_servoTool.attach(_pinServoTool);
		if (_pinServoTool2 > 0) _servoTool2.attach(_pinServoTool2);
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

	int Bras::calcPositionTool2(byte posServoTool2)
	{
		// limiter la commande entre 0 et 100%
		int newPosTool2 = constrain(posServoTool2,0,100);
		// Calculer la position servo en fonction des limites min max
		newPosTool2 = map(newPosTool2,0,100,_minServoTool2,_maxServoTool2);
		return newPosTool2;
	}

	void Bras::setPosition(byte posServoElevator, byte posServoArm, byte posServoTool, int wait)
	{
		// Appliquer la position
		setElevator(posServoElevator);
		setArm(posServoArm);
		setTool(posServoTool);
		System::wait(wait);
	}

	void Bras::setElevator    (byte posServoElevator, int wait)
	{
		if(!_servoElevator.attached()) 
			_servoElevator.attach(_pinServoElevator);
		_servoElevator.write(calcPositionElevator(posServoElevator));
		System::wait(wait);
	}

	void Bras::setArm (byte posServoArm, int wait)
	{
		if(!_servoArm.attached()) 
			_servoArm.attach(_pinServoArm);
		_servoArm.write(calcPositionArm(posServoArm));
		System::wait(wait);
	}

	void Bras::setTool (byte posServoTool, int wait)
	{
		if(!_servoTool.attached()) 
			_servoTool.attach(_pinServoTool);
		_servoTool.write(calcPositionTool(posServoTool));
		System::wait(wait);
	}

	void Bras::setTool2 (byte posServoTool2, int wait)
	{
		if(!_servoTool2.attached()) 
			_servoTool2.attach(_pinServoTool2);
		_servoTool2.write(calcPositionTool2(posServoTool2));
		System::wait(wait);
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

		System::wait(wait);
	}

	void Bras::ungrab(int wait)
	{
		enablePump(false);
		openEv();
		System::wait(300);
		openEv(false);

		System::wait(wait);
	}

}