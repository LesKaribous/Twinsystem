#pragma once
#include "core/module.h"

class Lidar : public Module{
private:
    /* data */
public:
    Lidar();
    ~Lidar();

    void update() override;
};

Lidar::Lidar(){
}

Lidar::~Lidar(){
}

void Lidar::update(){
}
