
#include "os/asyncExecutor.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>


/*
struct StepperProfile {
    double distance;
    double maxVelocity;
    double maxAcceleration;

    double acceleration;
    double velocity;
    double timeAccel;
    double timeFlat;
    double totalTime;
};

class StepperController : public Job{
public:
    void addStepper(double maxVelocity, double maxAcceleration) {
        steppers.push_back({0, maxVelocity, maxAcceleration, 0, 0, 0, 0, 0});
    }

    void computeSynchronizedProfiles() {
        std::vector<double> durations;
        for (auto& stepper : steppers) {
            durations.push_back(calcMinDuration(stepper.distance, stepper.maxVelocity, stepper.maxAcceleration));
        }
        double maxDuration = *std::max_element(durations.begin(), durations.end());

        for (auto& stepper : steppers) {
            computeProfile(stepper, maxDuration);
        }
    }

    const std::vector<StepperProfile>& getProfiles() const {
        return steppers;
    }

    void run() override; 
    void reset() override;  //Set to IDLE
    void start() override;   //Set to PENDING
    void pause() override;   //Set to PAUSED
    void resume() override;  //Set to PENDING
    void cancel() override;  //Set to CANCELLED
    void complete() override;//Set to COMPLETED

    void control();

    void setTarget();
    
    
    void printProfiles() const {
        for (size_t i = 0; i < steppers.size(); ++i) {
            const auto& s = steppers[i];
            std::cout << "Stepper " << i + 1 << ":\n"
                      << " - Steps: " << s.distance << "\n"
                      << " - Velocity: " << s.velocity << " steps/s\n"
                      << " - Acceleration: " << s.acceleration << " steps/s²\n"
                      << " - Time (Accel/Flat/Total): " << s.timeAccel << " / " << s.timeFlat
                      << " / " << s.totalTime << " s\n\n";
        }
    }

private:
    std::vector<StepperProfile> steppers;

    double calcMinDuration(double distance, double maxVel, double maxAcc) const {
        double t_accel = maxVel / maxAcc;
        double dist_accel = 0.5 * maxAcc * t_accel * t_accel;

        if (2 * dist_accel >= distance) {
            return 2 * sqrt(distance / maxAcc);
        } else {
            double dist_flat = distance - 2 * dist_accel;
            double t_flat = dist_flat / maxVel;
            return 2 * t_accel + t_flat;
        }
    }

    void computeProfile(StepperProfile &stepper, double targetDuration) const {
        double acc = stepper.maxAcceleration;
        double vel = stepper.maxVelocity;
        double distance = stepper.distance;

        if (distance < acc * targetDuration * targetDuration / 4) {
            stepper.timeAccel = targetDuration / 2;
            stepper.velocity = distance / stepper.timeAccel;
            stepper.acceleration = stepper.velocity / stepper.timeAccel;
            stepper.timeFlat = 0;
        } else {
            stepper.timeAccel = (acc * targetDuration - sqrt(acc * (acc * targetDuration * targetDuration - 4 * distance))) / (2 * acc);
            stepper.velocity = acc * stepper.timeAccel;
            stepper.acceleration = acc;
            stepper.timeFlat = targetDuration - 2 * stepper.timeAccel;
        }
        stepper.totalTime = targetDuration;
    }
};
*/

//Usage : 
/*
int main() {
    StepperController controller;

    // Example steppers (distance, max velocity, max acceleration)
    controller.addStepper(1200, 800, 2000);
    controller.addStepper(500, 600, 1500);
    controller.addStepper(300, 500, 1000);

    controller.computeSynchronizedProfiles();
    controller.printProfiles();

    return 0;
}
*/