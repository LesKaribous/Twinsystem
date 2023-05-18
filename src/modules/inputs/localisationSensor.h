#pragma once
#include "core/module.h"
#include "modules/inputs/button.h"
#include "core/lib.h"

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
        bool state = digitalReadFast(_pin);
        if(state != lastState){
            lastState = state;
            if(state){
                tlastUp = micros();   //Montant
            }else{
                tlastDown = micros(); //Descendant
                pulseWidth = tlastDown - tlastUp;
                _distance = 3/4 * (pulseWidth - 1000);

                return  _distance; // return distance
            }
        }

        return 0;
    }


};
