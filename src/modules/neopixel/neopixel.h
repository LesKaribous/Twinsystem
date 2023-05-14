#pragma once
#include "core/module.h"
#include <Adafruit_NeoPixel.h>

class NeoPixel : public Module{
private:
    Adafruit_NeoPixel strip;
public:
    NeoPixel();
    ~NeoPixel();

    void update() override;
    void enable() override;
    void disable() override;
};

