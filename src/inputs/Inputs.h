#pragma once
#include "event/Event.h"
#include <Arduino.h>

namespace TwinSystem{

    class BooleanInput{
    public:
        BooleanInput(){};

        virtual void init() = 0;
        virtual void update() = 0;
        virtual bool hasChanged(){
            if(state != lastState) return true;
            else return false;
        };
        virtual bool GetState() const{
            return state;
        }

    protected:
        bool state;
        bool lastState;
    };

    class Switch : public BooleanInput{
    public:
    
        Switch(int pin): _pin(pin){};
        void init() override{
            pinMode(_pin, INPUT_PULLUP);
        }
        void update() override{
            lastState = state;
            state = digitalRead(_pin);
        }
    private:
        int _pin;
    };

    class Button : public BooleanInput{
    public:
        Button(int pin): _pin(pin){};
        void init() override{
            pinMode(_pin, INPUT_PULLUP);
        }
        void update() override{
            lastState = state;
            state = digitalRead(_pin);
        }
    private:
        int _pin;
    };

    class Inputs{
    public:
        Inputs();
        void initialize();
        void update();
        bool hasChanged();
        //bool pollEvents(std::function<void(Event&)>);

    public:
        Button init;
        Switch team;
        Switch starter;
        Switch strategy;
    };



}