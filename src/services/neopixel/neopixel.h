#pragma once
#include "system/core/service.h"
#include <Adafruit_NeoPixel.h>

class NeoPixel : public Service{
private:
    Adafruit_NeoPixel strip;
public:
    NeoPixel();
    ~NeoPixel();

    void update() override;
    void enable() override;
    void disable() override;
};

