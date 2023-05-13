#pragma once
#include "core/module.h"

class Screen : public Module{
public:
    Screen();
    ~Screen();

    void update() override;
    bool isStarterPlaced();

private:
};

Screen::Screen(){
}

Screen::~Screen(){
}

void Screen::update(){
}

bool Screen::isStarterPlaced(){
    return false;
}
