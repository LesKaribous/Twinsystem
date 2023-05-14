#pragma once
#include "core/module.h"

class Actuators : public Module{
private:
    /* data */
public:
    Actuators();
    ~Actuators();

    void update() override;
};

Actuators::Actuators() : Module(ACTUATOR){
}

Actuators::~Actuators(){
}

void Actuators::update(){
}
