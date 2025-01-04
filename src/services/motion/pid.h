#pragma once
#include "utils/geometry.h"
#include "settings.h"

template <typename T>
class PID {
public:
   
    PID(const T& kp, const T& ki, const T& kd)
        : m_kP(kp)
        , m_kI(ki)
        , m_kD(kd)
        , m_integral(T{})
        , m_previousError(T{})
    {}

   
    inline T compute(const T& error)
    {
        static long lastControlTime = 0;
        float dt = micros() - lastControlTime;
        dt = max(dt, Settings::Motion::MAX_PID_DELAY);
        dt /= 1000000.0;
        lastControlTime = micros();
        
        T derivative = (error - m_previousError) / dt;

        m_integral += error * dt;

        T output = (m_kP * error) 
                 + (m_kI * m_integral)
                 + (m_kD * derivative);

        m_previousError = error;

        return output;
    }

    inline void reset()
    {
        m_integral = T{};
        m_previousError = T{};
    }


    inline void setKp(const T& kp) { m_kP = kp; }
    inline void setKi(const T& ki) { m_kI = ki; }
    inline void setKd(const T& kd) { m_kD = kd; }

    inline const T& getKp() const { return m_kP; }
    inline const T& getKi() const { return m_kI; }
    inline const T& getKd() const { return m_kD; }

private:
    T m_kP;
    T m_kI;
    T m_kD;

    T m_integral;
    T m_previousError;
};
