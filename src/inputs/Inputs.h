#pragma once
#include "event/Event.h"
#include <Arduino.h>

namespace TwinSystem{

    template <class T>
    class AbstractInput{
    public:
        AbstractInput(){};

        virtual void init() = 0;
        virtual void update() = 0;
        virtual bool hasChanged(){
            if(value != lastValue && enabled) return true;
            else return false;
        };
        virtual bool GetValue() const{
            return value;
        }

        inline void Enable(){enabled = true;}
        inline void Disable(){enabled = false;}

    protected:
        bool enabled;
        T value;
        T lastValue;
    };

    class BooleanInput : public AbstractInput<bool>{
    public:
        BooleanInput(){};

        virtual bool GetState() const{
            return value;
        }
    };

    class Switch : public BooleanInput{
    public:
    
        Switch(int pin): _pin(pin){};
        void init() override{
            pinMode(_pin, INPUT_PULLUP);
        }
        void update() override{
            if(!enabled) return;
            lastValue = value;
            value = digitalRead(_pin);
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
            if(!enabled) return;
            lastValue = value;
            value = digitalRead(_pin);
        }
    private:
        int _pin;
    };

    class DummyBooleanInput : public BooleanInput{
    public:
        DummyBooleanInput(bool& reference) : _reference(reference){};
        void init() override{}
        void update() override{
            if(!enabled) return;
            lastValue = value;
            value = _reference;
        }

    private:
        bool& _reference;
    };

    class Inputs{
    public:
        Inputs();
        void Initialize();
        void Update();
        bool HasChanged();
        //bool pollEvents(std::function<void(Event&)>);

    public:
        Button resetButton;

        Switch twinSwitch;
        Switch teamSwitch;
        Switch strategySwitch;
        //Switch avoidanceSwitch;
        
        Switch starter;
    };




    class DummyDigitalInput : public AbstractInput<int>{
    public:
        DummyDigitalInput(int& reference) : _reference(reference){};

        void init() override{}
        void update() override{
            if(!enabled) return;
            lastValue = value;
            value = _reference;
        }

    private:
        int& _reference;
    };




    class References{
    public:
        References();
        void Initialize();
        void Update();
        bool HasChanged();
        //bool pollEvents(std::function<void(Event&)>);

    public:
        DummyDigitalInput x, y, z;
        DummyBooleanInput team, strategy, avoidance, intercom;
    };
}