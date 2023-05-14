#include "modules/lidar/lidar.h"

Lidar::Lidar() : Module(LIDAR){
    m_obstacle = false;
}

Lidar::~Lidar(){}

void Lidar::update(){
    
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