#pragma once
#include "button.h"
#include "system/core/lib.h"

class LocalisationSensor : public AbstractInput<int>
{
private:
    int _pin;
    bool lastState  = false;
    uint32_t tlastUp, tlastDown, pulseWidth, lastpulseWidth;
    float _distance;
public:
    LocalisationSensor(int pin): _pin(pin){};

    void init() override{
        pinMode(_pin, INPUT);
    }

    void read() override{
        getDitance();
    }

    float getDistance() const{return _distance;}

    virtual float getDitance(){
        int16_t t = pulseIn(_pin, HIGH);
        
        if (t == 0)
        {
            _distance = -1;
        }
        else if (t > 1850)
        {
            _distance = -1;
        }
        else
        {
            _distance = (float(t) - 1000.0) * 4.0;
            // Limit minimum distance to 0.
            if (_distance < 0) { _distance = 0; } 
        }
    }


};
