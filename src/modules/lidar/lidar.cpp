#include "modules/lidar/lidar.h"
#include "settings.h"
#include "debug/console.h"

Lidar::Lidar() : Module(LIDAR){
	intercom.initialize();
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
}

bool Lidar::obstacleDetected(){
	return (((millis() -_lastSeen) < Settings::Lidar::persitency)) && m_enabled;
}


void Lidar::onObstacleResponse(String answer){
    if(answer.startsWith("obstacle")){
		_lastSeen = millis();
	}
}

bool Lidar::isConnected(){
	return intercom.isConnected();
}

void Lidar::displayRadar(bool s){
	if(s)intercom.sendRequest("displayLidar");
	else intercom.sendRequest("displayIntercom");
}

void Lidar::checkLidar(float heading){
	THROW("Checking lidar")
	if(m_enabled){
		THROW("Checking lidar enabled")
		if(millis() - _lastLidarCheck > 20){
			_lastLidarCheck = millis();
			heading = fmod(heading, 360.0);
			if(heading < 0) heading += 360.0;

			intercom.sendRequest("checkLidar(" + String(heading) + ")");
		}

		if(intercom.available()){
			Request& req = intercom.getReadyRequest();
			String response = req.getResponse();
			THROW(response)

			onObstacleResponse(response);

			req.close();
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