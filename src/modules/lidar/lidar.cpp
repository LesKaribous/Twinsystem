#include "modules/lidar/lidar.h"
#include "debug/console.h"

Lidar::Lidar() : Module(LIDAR){
    m_obstacle = false;
}

Lidar::~Lidar(){}

void Lidar::update(){
    intercom.update();
}

void Lidar::enable(){
    Module::enable();
}

void Lidar::disable(){
    Module::disable();
    m_obstacle = false;
}

bool Lidar::obstacleDetected(){
	return m_obstacle && m_enabled;
}

bool Lidar::isConnected(){
	return intercom.isConnected();
}

void Lidar::activateDisplay(){
	intercom.sendRequest("displayLidar");
}

void Lidar::checkLidar(float heading){
	if(m_enabled){
		if(millis() - _lastLidarCheck > 20){
			_lastLidarCheck = millis();
			heading = fmod(heading, 360.0);
			if(heading < 0) heading += 360.0;

			intercom.sendRequest("checkLidar(" + String(heading) + ")", OnDummyRequestResponse);
		}

		if(obstacleDetected())_lastSeen = millis();
		
		if(obstacleDetected() && motion.isRunning() && !motion.IsRotating()){
			motion.Pause();
		}else if(motion.IsPaused() && !obstacleDetected()){
			if(millis() - _lastSeen > 1000) motion.Resume();
		}
	}
}

//mm rad (absolute)
float Lidar::getMaxLidarDist(Vec2 pos, float angle){

	Vec2 tableHit = Vec2(3000,0);
	tableHit.rotate(angle);
	tableHit = Vec2::add(pos, tableHit);
	if(tableHit.a > 3000) tableHit.a = 3000;
	if(tableHit.a < 0) tableHit.a = 0;
	if(tableHit.b > 2000) tableHit.b = 2000;
	if(tableHit.b <0) tableHit.b = 0;
	tableHit.sub(pos);
	float maxdist = tableHit.mag();

	Console::print("Lidar current pos : {");
	Console::print(pos.a);
	Console::print(",");
	Console::print(pos.b);
	Console::print("}, Angle : ");
	Console::print((angle * RAD_TO_DEG));
	Console::print(", max lidar dist : ");
	Console::println(maxdist);

	return maxdist;
}