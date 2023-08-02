#pragma once

template <class T>
class AbstractField{
public:
    AbstractField(){};

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

class DigitalField : public AbstractField<int>{
public:
    DigitalField() {};

    void init() override{ _hasChanged = true;}
    void read() override{
        if(!enabled) return;
        _hasChanged = _reference != value;
        lastValue = value;
        value = _reference;
    }

    void SetValue(int ref) {_reference = ref;}

private:
    int _reference;
};

class FloatingField : public AbstractField<float>{
public:
    FloatingField() {};

    void init() override{ _hasChanged = true;}
    void read() override{
        if(!enabled) return;
        _hasChanged = _reference != value;
        lastValue = value;
        value = _reference;
    }

    void SetValue(float ref) {_reference = ref;}

private:
    float _reference;
};


class BooleanField : public AbstractField<bool>{
public:
    BooleanField() {};
    void init() override{ _hasChanged = true;}
    void read() override{
        if(!enabled) return;
        _hasChanged = _reference != value;
        lastValue = value;
        value = _reference;
    }

    void SetValue(bool ref) {_reference = ref;}

    virtual bool getState() const{
        return _inverted ? !value : value;
    }
    inline void setInverted(bool s = true){_inverted = s;}
private:
    bool _inverted = false;
    bool _reference;
};

