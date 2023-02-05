#pragma once;
#include "core/Core.h"
#include "core/Event.h"

namespace TwinSystem{
    class Inputs{
    public:

        bool pollEvents(void(*cb)(Event& e));

    private:

    };

    class BooleanInput{
    public:
        BooleanInput(){};
        virtual bool hasChanged(){
            if(state != lastState) return true;
            else return false;
        };

    protected:
        bool state;
        bool lastState;
    };

    class Switch : public BooleanInput{
    public:
        Switch(int pin);

    private:
        int pin;
    };

    class Button : public BooleanInput{
    public:
        Button(int pin);
        
    private:
        int pin;
    };

}