#pragma once
class PIDController {
public:
    PIDController(float kP, float kI, float kD)
        : kP(kP), kI(kI), kD(kD), integral(0.0f), prevError(0.0f) {}

    float compute(float error,  float dt, bool saturated = false) {
        if(!saturated) integral += error * dt;
        
        float derivative = 0;
        if (init){ //avoid derivative kick
           init = false;
        }else{
            derivative = (error - prevError) / dt; 
        }
        prevError = error;

        return kP * error + kI * integral + kD * derivative;
    }

    // float compute(float error, float vel_error, float dt, bool saturated = false) {
    //     if(!saturated) integral += error * dt;
    //     float derivative = vel_error;
    //     prevError = error; //in case we use it later
    //     return kP * error + kI * integral + kD * derivative;
    // }

    void reset() {
        integral = 0.0f;
        prevError = 0.0f;
    }

private:
    float kP, kI, kD;
    float integral;
    float prevError;
    bool init = true;
};
