#pragma once
#include "inputs/Inputs.h"

namespace TwinSystem{

    class DigitalField : public AbstractInput<int>{
    public:
        DigitalField() {};

        void Init() override{ _hasChanged = true;}
        void Read() override{
            if(!enabled) return;
            _hasChanged = false;
            lastValue = value;
            value = _reference;
            if(value != lastValue) _hasChanged = true;
        }

        void SetValue(int ref) {_reference = ref;}

    private:
        int _reference;
    };

    class FloatingField : public AbstractInput<float>{
    public:
        FloatingField() {};

        void Init() override{ _hasChanged = true;}
        void Read() override{
            if(!enabled) return;
            _hasChanged = false;
            lastValue = value;
            value = _reference;
            if(value != lastValue) _hasChanged = true;
        }

        void SetValue(float ref) {_reference = ref;}

    private:
        float _reference;
    };


    class BooleanField : public BooleanInput{
    public:
        BooleanField() {};
        void Init() override{ _hasChanged = true;}
        void Read() override{
            if(!enabled) return;
            _hasChanged = false;
            lastValue = value;
            value = _reference;
            if(value != lastValue) _hasChanged = true;
        }

        void SetValue(bool ref) {_reference = ref;}

    private:
        bool _reference;
    };



    class Fields{
    public:
        Fields();
        void Initialize();
        void Read();
        void Reset();
        bool HasChanged();
        //bool pollEvents(std::function<void(Event&)>);

    public:
        FloatingField
        x, y, z;

        DigitalField
        score;

        BooleanField
        intercom,
        probing,
        probed,
        armed,
        started;
    };

}