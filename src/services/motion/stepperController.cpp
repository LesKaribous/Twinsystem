#include "stepperController.h"

StepperController::StepperController(){
    for(int i = 0; i < STEPPER_COUNT; i++){
        steppers[i].acceleration = Settings::Stepper::MAX_ACCEL;
        steppers[i].velocity = Settings::Stepper::MAX_SPEED;
        steppers[i].timeAccel = 0;
        steppers[i].timeFlat = 0;
        steppers[i].totalTime = 0;
    }
}

void StepperController::computeSynchronizedProfiles() {
    std::vector<double> durations;
    for (auto& stepper : steppers) {
        durations.push_back(calcMinDuration(stepper.distanceToGo, Settings::Stepper::MAX_SPEED, Settings::Stepper::MAX_ACCEL));
    }
    double maxDuration = *std::max_element(durations.begin(), durations.end());

    for (auto& stepper : steppers) {
        computeProfile(stepper, maxDuration);
    }
}




// Run: Called from the main loop to update the stepping profiles.
// This function simulates one update interval where positions are recalculated
// based on the motion profile. In a real system, this might update step delay values
// or compute new positions using an incremental algorithm like Bresenham.
void StepperController::run() {
    // Static variable to simulate elapsed time in seconds.
    static double elapsedTime = 0.0;
    double dt = 0.01; // simulation time step (10ms) — for real implementation use precise timers
    elapsedTime += dt;

    for (int i = 0; i < STEPPER_COUNT; i++) {
        StepperProfile &stepper = steppers[i];
        if (elapsedTime >= stepper.totalTime) {
            // If the motion time has elapsed, set the current position to the target.
            stepper.currentPosition = stepper.distanceToGo;
        } else {
            double pos = 0.0;
            if (elapsedTime < stepper.timeAccel) {
                // Accelerating phase: s = 0.5 * a * t^2.
                pos = 0.5 * stepper.acceleration * elapsedTime * elapsedTime;
            } else if (elapsedTime < (stepper.timeAccel + stepper.timeFlat)) {
                // Constant speed phase: s = s_accel + velocity * t.
                double t = elapsedTime - stepper.timeAccel;
                pos = 0.5 * stepper.acceleration * stepper.timeAccel * stepper.timeAccel + stepper.velocity * t;
            } else {
                // Deceleration phase: symmetric to acceleration.
                double t = elapsedTime - stepper.timeAccel - stepper.timeFlat;
                pos = stepper.distanceToGo - 0.5 * stepper.acceleration * (stepper.totalTime - elapsedTime) * (stepper.totalTime - elapsedTime);
            }
            stepper.currentPosition = pos;
        }
        // In an actual implementation, here you would use the difference between
        // calculated position and the last step position to generate step pulses.
    }
}

// Reset the controller and all steppers to their initial states.
void StepperController::reset() {
    Job::reset();
    for (int i = 0; i < STEPPER_COUNT; i++) {
        steppers[i].distanceToGo = 0;
        steppers[i].currentPosition = 0;
        steppers[i].timeAccel = 0;
        steppers[i].timeFlat = 0;
        steppers[i].totalTime = 0;
        steppers[i].lastStepTime = 0;
    }
}

// Start the job and initialize any additional parameters.
void StepperController::start() {
    Job::start();
    for (int i = 0; i < STEPPER_COUNT; i++) {
        // Reset the current position and last step time.
        steppers[i].currentPosition = 0;
        steppers[i].lastStepTime = 0;
    }
}

// Pause the motion (for example, by disabling step pulse generation).
void StepperController::pause() {
    Job::pause();
    // Here, disable any timers or motors if required.
}

// Resume motion from a paused state.
void StepperController::resume() {
    Job::resume();
    // Re-enable timers or reinitialize step counters as needed.
}

// Cancel the current motion: immediately stop the steppers.
void StepperController::cancel() {
    Job::cancel();
    // For each stepper, stop motion and optionally reset the position.
    for (int i = 0; i < STEPPER_COUNT; i++) {
        // In hardware, you might disable the drivers; here we simply reset the position.
        steppers[i].currentPosition = 0;
    }
}

// Complete the job: finish motion, perform cleanup and reset state.
void StepperController::complete() {
    // Ensure each stepper has reached its target.
    for (int i = 0; i < STEPPER_COUNT; i++) {
        steppers[i].currentPosition = steppers[i].distanceToGo;
        // Additional cleanup logic can be placed here.
    }
    // Optionally reset the controller state.
}

// The control function is meant to be called every ~10 microseconds (via a timer interrupt)
// to generate actual step pulses for the motors. Here, we use a placeholder implementation.
void StepperController::control() {
    // In a real implementation, use a hardware timer to get the current time in microseconds.
    static unsigned long lastControlTime = 0;
    unsigned long currentTime = 0;  // Replace with actual hardware timer reading
    unsigned long dt = currentTime - lastControlTime; // elapsed time in microseconds
    if (dt < 10)
        return;  // Ensure at least 10 microseconds have passed
    lastControlTime = currentTime;

    // Use a simple algorithm (or a Bresenham variant) to decide when to issue step pulses.
    // Here, we use an error accumulator for each stepper.
    static double error[STEPPER_COUNT] = {0.0, 0.0, 0.0};

    for (int i = 0; i < STEPPER_COUNT; i++) {
        StepperProfile &stepper = steppers[i];
        // In a full implementation, targetPosition could be a step count computed from currentPosition.
        double targetPosition = stepper.currentPosition;
        // The following simulates an accumulator that increments to trigger a step.
        error[i] += targetPosition - error[i];  // Update the error using a simplified model
        if (error[i] >= 1.0) {
            // Issue a step pulse here.
            // In a real system, set the appropriate GPIO pin high then low.
            error[i] -= 1.0;
            // Optionally update a step counter or current hardware position.
        }
    }
}

void StepperController::setTarget(Vec3 target){
    steppers[0].distanceToGo = target.a;
    steppers[1].distanceToGo = target.b;
    steppers[2].distanceToGo = target.c;
    computeSynchronizedProfiles();
}

void StepperController::setTarget(float x, float y, float z ){
    setTarget(Vec3(x,y,z));
}

double StepperController::calcMinDuration(double distance, double maxVel, double maxAcc) const {
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

void StepperController::computeProfile(StepperProfile &stepper, double targetDuration) const {
    double acc = Settings::Stepper::MAX_ACCEL;
    double vel = Settings::Stepper::MAX_SPEED;
    double distance = stepper.distanceToGo;

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