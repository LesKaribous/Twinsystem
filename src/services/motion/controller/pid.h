
#pragma once
class PIDController {
public:
    PIDController(float kP, float kI, float kD)
        : kP(kP), kI(kI), kD(kD), integral(0.0f), prevError(0.0f) {}

    float compute(float error, float dt) {
        integral += error * dt;
        float derivative = (error - prevError) / dt;
        prevError = error;

        return kP * error + kI * integral + kD * derivative;
    }

    void reset() {
        integral = 0.0f;
        prevError = 0.0f;
    }

private:
    float kP, kI, kD;
    float integral;
    float prevError;
};
