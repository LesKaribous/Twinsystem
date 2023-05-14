#pragma once
#include "core/lib.h"

template <class T>
class AbstractInput{
public:
    AbstractInput(){};

    virtual void init() = 0;
    virtual void read() = 0;

    virtual bool hasChanged() const{
        return(_hasChanged && enabled);
    };
    virtual T getValue() const{
        return value;
    }

    inline void enable(){enabled = true;}
    inline void disable(){enabled = false;}

protected:
    bool enabled = true;
    T value = 0;
    T lastValue = 0;
    bool _hasChanged = true;
};

class BooleanInput : public AbstractInput<bool>{
public:
    BooleanInput(){};
    inline void setInverted(bool s = true){_inverted = s;};

    virtual bool getState() const{
        return _inverted ? !value : value;
    }

private:
    bool _inverted = false;
};

class Switch : public BooleanInput{
public:

    Switch(int pin): _pin(pin){};
    void init() override{
        pinMode(_pin, INPUT_PULLUP);
        lastValue = false;
        value = digitalRead(_pin);
    }
    void read() override{
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
    void init() override{
        pinMode(_pin, INPUT_PULLUP);
        lastValue = !digitalRead(_pin);
        value = digitalRead(_pin);
    }
    void read() override{
        if(!enabled) return;
        _hasChanged = false;
        lastValue = value;
        value = digitalRead(_pin);
        if(value != lastValue) _hasChanged = true;
    }
private:
    int _pin;
};
