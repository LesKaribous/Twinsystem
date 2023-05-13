#pragma once
#include "event/Event.h"
#include <Arduino.h>

namespace TwinSystem{

    template <class T>
    class AbstractInput{
    public:
        AbstractInput(){};

        virtual void Init() = 0;
        virtual void Read() = 0;

        virtual bool HasChanged(){
            return(_hasChanged && enabled);
        };
        virtual T GetValue() const{
            return value;
        }

        inline void Enable(){enabled = true;}
        inline void Disable(){enabled = false;}

    protected:
        bool enabled = true;
        T value = 0;
        T lastValue = 0;
        bool _hasChanged = true;
    };

    class BooleanInput : public AbstractInput<bool>{
    public:
        BooleanInput(){};
        inline void SetInverted(bool s = true){_inverted = s;};

        virtual bool GetState() const{
            return _inverted ? !value : value;
        }

    private:
        bool _inverted = false;
    };

    class Switch : public BooleanInput{
    public:
    
        Switch(int pin): _pin(pin){};
        void Init() override{
            pinMode(_pin, INPUT_PULLUP);
            lastValue = false;
            value = digitalRead(_pin);
        }
        void Read() override{
            if(!enabled) return;
            _hasChanged = false;
            lastValue = value;
            value = digitalRead(_pin);
            if(value != lastValue) _hasChanged = true;
        }
    private:
        int _pin;
    };

    class Button : public BooleanInput{
    public:
        Button(int pin): _pin(pin){};
        void Init() override{
            pinMode(_pin, INPUT_PULLUP);
            lastValue = !digitalRead(_pin);
            value = digitalRead(_pin);
        }
        void Read() override{
            if(!enabled) return;
            _hasChanged = false;
            lastValue = value;
            value = digitalRead(_pin);
            if(value != lastValue) _hasChanged = true;
        }
    private:
        int _pin;
    };

    class Inputs{
    public:
        Inputs();
        void Initialize();
        void Read();
        void Reset();
        bool HasChanged();
        void OnEvent(Event& e);

    public:
        Button resetButton;

        Switch twinSwitch;
        Switch teamSwitch;
        Switch strategySwitch;
        //Switch avoidanceSwitch;
        
        Switch starter;
    };



}