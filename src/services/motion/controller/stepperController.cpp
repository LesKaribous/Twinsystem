#include "stepperController.h"
#include "os/console.h"
#include "services/motion/kinematics.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

int32_t LinStepAccelerator::prepare(int32_t _currentPos, int32_t _targetPos, uint32_t _targetSpeed, uint32_t _vStart, uint32_t _vEnd, uint32_t _accel) {
    vTarget = _targetSpeed;
    this->vStart = _vStart;
    this->vEnd = _vEnd;
    this->twoA = 2 * _accel;

    s0 = _currentPos;
    ds = std::abs(_targetPos - _currentPos);

    vStart2 = (int64_t)vStart * vStart;
    vEnd2   = (int64_t)vEnd   * vEnd;
    vTarget2 = (int64_t)vTarget * vTarget;

    int32_t sm = ((vEnd2 - vStart2) / twoA + ds) / 2;

    if (sm >= 0 && sm <= ds) {
        int32_t sa = (vTarget2 - vStart2) / twoA;
        if (sa < sm) {
            accEnd = sa;
            decStart = sm + (sm - sa);
        } else {
            accEnd = decStart = sm;
        }
    } else {
        // Bad input
        Console::error("Planner") << "Planner return bad input" << Console::endl;
        accEnd = decStart = ds;
    }
    return vStart;
}

int32_t LinStepAccelerator::updateSpeed(int32_t currentPos) {
    int32_t s = std::abs(currentPos - s0);

    if (s < accEnd)
        return std::sqrt((float)(twoA * s + vStart2));
    if (s < decStart)
        return vTarget;
    if (s < ds)
        return std::sqrt((float)(twoA * (ds - s - 1) + vEnd2));
    
    return 0;
}

uint32_t LinStepAccelerator::initiateStop(int32_t currentPos) {
    const int32_t stepsDone = std::abs(s0 - currentPos);

    if (stepsDone < accEnd) {// Still accelerating: symmetrically decelerate over same distance
        accEnd = decStart = 0;
        ds = 2 * stepsDone;
        return stepsDone;
    } 
    else if (stepsDone < decStart) {// Constant-speed segment: use acceleration to decelerate
        decStart = 0;
        ds = stepsDone + accEnd;
        return accEnd;
    } 
    else {
        return ds - stepsDone;// Already decelerating: return what's leff
    }
}


StepperController::StepperController() : 
    m_sA(nullptr), 
    m_sB(nullptr),  
    m_sC(nullptr),
    m_totalTime(0), m_startTime(0)
{// Initialization of steppers acceleration parameters.
}
    
void StepperController::setFeedrate(float feed){
    m_feedrate = feed;
}


void StepperController::setTarget(long posA, long posB, long posC) {
    m_sA->m_target = posA;
    m_sB->m_target = posB;
    m_sC->m_target = posC;

    m_sA->m_finalTarget = posA;
    m_sB->m_finalTarget = posB;
    m_sC->m_finalTarget = posC;

    // Clear all errors — prevent reuse of old sync state
    m_sA->m_error = m_sB->m_error = m_sC->m_error = 0;

    m_sA->m_start = m_sA->m_position;
    m_sB->m_start = m_sB->m_position;
    m_sC->m_start = m_sC->m_position;

    m_sA->m_delta = std::abs(m_sA->m_target - m_sA->m_start);
    m_sB->m_delta = std::abs(m_sB->m_target - m_sB->m_start);
    m_sC->m_delta = std::abs(m_sC->m_target - m_sC->m_start);

    // Determine lead motor
    if (m_sA->m_delta >= m_sB->m_delta && m_sA->m_delta >= m_sC->m_delta) {
        m_leadStepper = m_sA;
        m_leadDelta = m_sA->m_delta;
    } else if (m_sB->m_delta >= m_sA->m_delta && m_sB->m_delta >= m_sC->m_delta) {
        m_leadStepper = m_sB;
        m_leadDelta = m_sB->m_delta;
    } else {
        m_leadStepper = m_sC;
        m_leadDelta = m_sC->m_delta;
    }

    if (m_leadStepper == nullptr || m_leadDelta == 0) {
        Console::error("StepperController") << "Lead stepper not assigned properly!" << Console::endl;
        reset();
        m_state = JobState::CANCELED;
        return;
    }

    uint32_t targetSpeed = Settings::Stepper::MAX_SPEED* m_feedrate;
    uint32_t vStart = Settings::Stepper::PULL_IN;
    uint32_t vEnd   = Settings::Stepper::PULL_OUT;
    uint32_t accel  = Settings::Stepper::MAX_ACCEL * m_feedrate;

    long leadTarget =
        (m_leadStepper == m_sA) ? m_sA->m_target :
        (m_leadStepper == m_sB) ? m_sB->m_target :
                                    m_sC->m_target;

    m_planner.prepare(m_leadStepper->m_position, leadTarget, targetSpeed, vStart, vEnd, accel);

    // Bresenham sync errors for all
    if (m_leadStepper != m_sA) m_sA->m_error = 2 * m_sA->m_delta - m_leadDelta;
    if (m_leadStepper != m_sB) m_sB->m_error = 2 * m_sB->m_delta - m_leadDelta;
    if (m_leadStepper != m_sC) m_sC->m_error = 2 * m_sC->m_delta - m_leadDelta;

    m_sA->m_stepsDone = m_sB->m_stepsDone = m_sC->m_stepsDone = 0;

    if      (m_leadStepper == m_sA) m_leadStepsDone = &m_sA->m_stepsDone;
    else if (m_leadStepper == m_sB) m_leadStepsDone = &m_sB->m_stepsDone;
    else if (m_leadStepper == m_sC) m_leadStepsDone = &m_sC->m_stepsDone;

    
}

// start() – Enable steppers, then begin the move.
void StepperController::start() {
    Job::start();
    m_sA->enable();
    m_sB->enable();
    m_sC->enable();
    
    if (m_leadStepper == nullptr || m_leadDelta == 0) {
        Console::error("StepperController") << "Lead stepper not assigned properly!" << Console::endl;
        reset();
    }

    m_startTime = micros() * 1e-6;
}

void StepperController::run(){
    long now = micros();
    if (now - m_last_compute >= Settings::Stepper::STEPPER_COMPUTE_DELAY) {
        m_last_compute = now;
    }else return;

    if(isCanceling()){
        onCanceling();
    }else if(isRunning()) onUpdate();
}


void StepperController::onUpdate() {
    if (!isBusy()) return;

    int32_t leadSpeed = m_planner.updateSpeed(m_leadStepper->m_position);

    if (leadSpeed > 0) {
        for (Stepper* s : { m_sA, m_sB, m_sC }) {
            if (s->m_delta > 0) {
                float ratio = s->m_delta / static_cast<float>(m_leadDelta);
                float velocity = leadSpeed * ratio;
                float direction = (s->m_target >= s->m_start) ? 1.0f : -1.0f;
                s->setVelocity(direction * velocity);
            }
        }
    } else if(isRunning()){
        Console::success("StepperController") << Console::microTimeStamp() << " : Motion complete (speed = 0)" << Console::endl;
        complete();
    } else if(isCanceling()){
        onCanceled();
    }
}



void StepperController::control() {
    if (!isBusy()) return;

    unsigned long now = micros();

    if (now - m_last_control >= Settings::Stepper::STEPPER_DELAY) {
        m_last_control = now;
    }else return;

    // more stable like this...
    
    m_sA->step();
    m_sB->step();
    m_sC->step();
    

    // --- Throttle logic updates ---
    static unsigned long m_last_compute = micros();
    if (micros() - m_last_compute < Settings::Stepper::STEPPER_COMPUTE_DELAY) return;
    m_last_compute = micros();

    bool steppedLead = false;

    // --- Step lead stepper ---
    if (*m_leadStepsDone < m_leadDelta && m_leadStepper->shouldStep()) {
        m_leadStepper->step();
        (*m_leadStepsDone)++;
        steppedLead = true;
        // Console::trace("Lead stepper stepped. Total: ") << *m_leadStepsDone << Console::endl;
    }

    // --- Step the non-lead steppers using Bresenham sync ---
    for (Stepper* s : { m_sA, m_sB, m_sC }) {
        if (s == m_leadStepper) continue;
        if (s->m_stepsDone < s->m_delta && steppedLead) {
            s->m_error += 2 * s->m_delta;
            if (s->m_error >= m_leadDelta) {
                s->step();
                s->m_stepsDone++;
                s->m_error -= 2 * m_leadDelta;
            }
        }
    }

    // --- Check if all axes are complete ---
    if (m_sA->m_stepsDone >= m_sA->m_delta &&
        m_sB->m_stepsDone >= m_sB->m_delta &&
        m_sC->m_stepsDone >= m_sC->m_delta)
    {
        Console::success("StepperController") << Console::microTimeStamp() << " : All axes complete, calling complete()" << Console::endl;
        complete();
    }
}

void StepperController::onCanceling(){
    onUpdate();
}

void StepperController::onCanceled(){
    Job::onCanceled();
    Console::success("StepperController") << Console::microTimeStamp() << " : Motion cancelled succesfully (speed = 0)" << Console::endl;
    complete();
}

// complete() – Immediately stop and reset the trajectory data, then disable the steppers.
void StepperController::complete() {
    m_sA->setVelocity(0);
    m_sB->setVelocity(0);
    m_sC->setVelocity(0);
    m_sA->disable();
    m_sB->disable();
    m_sC->disable();
    Job::complete();
}

void StepperController::pause(){
    Console::error("StepperController") << "Pause is not supported yet (cancelling instead)" << Console::endl;
    cancel();
}

void StepperController::resume(){
    Console::error("StepperController") << "Resume is not supported yet" << Console::endl;
}

// cancel() – Decelerate (using pause) then complete the move.
void StepperController::cancel() {
    if (!isRunning()) return;

    Console::info("cancel") << "Cancelling with controlled deceleration..." << Console::endl;

    // Estimate current speed to create a short deceleration move
    int32_t vStart = std::max(1, static_cast<int>(fabs(m_leadStepper->getVelocity())));
    uint32_t vEnd = Settings::Stepper::PULL_OUT;
    uint32_t accel = Settings::Stepper::STOP_DECCEL* m_feedrate;
    uint32_t speed = Settings::Stepper::MAX_SPEED* m_feedrate;

    // Estimate how far we need to stop (s = (v² - ve²) / 2a)
    int32_t stopDistance = (vStart * vStart - vEnd * vEnd) / (2 * accel);
    if (stopDistance <= Settings::Stepper::MIN_STEPS) {
        Console::warn("cancel") << "Already nearly stopped. Forcing complete." << Console::endl;
        complete();
        return;
    }

    // Plan temporary motion to stop
    int dir = (m_leadStepper->getVelocity() >= 0) ? 1 : -1;
    long stopTarget = m_leadStepper->m_position + dir * stopDistance;

    m_planner.prepare(m_leadStepper->m_position, stopTarget, speed, vStart, vEnd, accel);
    Job::cancel();
    
}





// reset() – If a move is running, cancel it; then reset all internal parameters.
void StepperController::reset() {
    if (isPending()) {
        cancel();
    }
    Job::reset();
    m_sA->setVelocity(0);
    m_sB->setVelocity(0);
    m_sC->setVelocity(0);
    m_sA->disable();
    m_sB->disable();
    m_sC->disable();
    // Reset internal move parameters.

    m_totalTime = 0;
    m_startTime = 0;

    m_sA->reset();
    m_sB->reset();
    m_sC->reset();

}

Vec3 StepperController::getDisplacement(){
    if(isIdle()) return Vec3(0);
    return fk(Vec3(m_sA->m_position, m_sB->m_position, m_sC->m_position));
}

void StepperController::setSteppers(Stepper* a, Stepper* b, Stepper* c){
    m_sA = a;
    m_sB = b;
    m_sC = c;

    m_sA->m_accel = Settings::Stepper::MAX_ACCEL* m_feedrate;
    m_sB->m_accel = Settings::Stepper::MAX_ACCEL* m_feedrate;
    m_sC->m_accel = Settings::Stepper::MAX_ACCEL* m_feedrate;
    
    m_sA->m_deccel = Settings::Stepper::STOP_DECCEL* m_feedrate;
    m_sB->m_deccel = Settings::Stepper::STOP_DECCEL* m_feedrate;
    m_sC->m_deccel = Settings::Stepper::STOP_DECCEL* m_feedrate;
}