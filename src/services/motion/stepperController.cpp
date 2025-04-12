#include "stepperController.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>


int32_t LinStepAccelerator::prepare(int32_t currentPos, int32_t targetPos, uint32_t targetSpeed, uint32_t vStart, uint32_t vEnd, uint32_t accel) {
    vTarget = targetSpeed;
    this->vStart = vStart;
    this->vEnd = vEnd;
    this->twoA = 2 * accel;

    s0 = currentPos;
    ds = std::abs(targetPos - currentPos);

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
    const int32_t s = std::abs(currentPos - s0);
    const int32_t MIN_DECEL_STEPS = 4;

    if (s < accEnd) {
        // Still accelerating: symmetrically decelerate over same distance
        accEnd = decStart = 0;
        ds = 2 * s;
        return std::max(s, MIN_DECEL_STEPS);
    } 
    else if (s < decStart) {
        // Constant-speed segment: use acceleration to decelerate
        decStart = 0;
        ds = s + accEnd;
        return std::max(accEnd, MIN_DECEL_STEPS);
    } 
    else {
        // Already decelerating: return what's left
        int32_t remaining = ds - s;
        return (remaining > 0) ? std::max(remaining, MIN_DECEL_STEPS) : 0;
    }
}



// Constructor.
StepperController::StepperController() : 
    m_sA(Pin::Stepper::stepA, Pin::Stepper::dirA, !Settings::Stepper::DIR_A_POLARITY),
    m_sB(Pin::Stepper::stepB, Pin::Stepper::dirB, !Settings::Stepper::DIR_B_POLARITY),
    m_sC(Pin::Stepper::stepC, Pin::Stepper::dirC, !Settings::Stepper::DIR_C_POLARITY),
    targetA(0), targetB(0), targetC(0),
    m_startA(0), m_startB(0), m_startC(0),
    m_totalDistanceA(0), m_totalDistanceB(0), m_totalDistanceC(0),
    m_totalTime(0), m_startTime(0)
{
    // Initialization of steppers acceleration parameters.
    m_sA.setAcceleration(Settings::Stepper::MAX_ACCEL);
    m_sB.setAcceleration(Settings::Stepper::MAX_ACCEL);
    m_sC.setAcceleration(Settings::Stepper::MAX_ACCEL);
    
    m_sA.setDeceleration(Settings::Stepper::STOP_DECCEL);
    m_sB.setDeceleration(Settings::Stepper::STOP_DECCEL);
    m_sC.setDeceleration(Settings::Stepper::STOP_DECCEL);
}
    

void StepperController::setTarget(long posA, long posB, long posC) {
    targetA = posA;
    targetB = posB;
    targetC = posC;

    // Save original target positions for resume support
    m_finalTargetA = posA;
    m_finalTargetB = posB;
    m_finalTargetC = posC;

    Job::reset();
}

    
// start() – Enable steppers, then begin the move.
void StepperController::start() {
    Job::start();
    m_sA.enable();
    m_sB.enable();
    m_sC.enable();


    m_startA = m_sA.getPosition();
    m_startB = m_sB.getPosition();
    m_startC = m_sC.getPosition();

    m_deltaA = std::abs(targetA - m_startA);
    m_deltaB = std::abs(targetB - m_startB);
    m_deltaC = std::abs(targetC - m_startC);

    // Determine lead motor
    if (m_deltaA >= m_deltaB && m_deltaA >= m_deltaC) {
        m_leadStepper = &m_sA;
        m_leadDelta = m_deltaA;
    } else if (m_deltaB >= m_deltaA && m_deltaB >= m_deltaC) {
        m_leadStepper = &m_sB;
        m_leadDelta = m_deltaB;
    } else {
        m_leadStepper = &m_sC;
        m_leadDelta = m_deltaC;
    }

    uint32_t targetSpeed = Settings::Stepper::MAX_SPEED;
    uint32_t vStart = Settings::Stepper::PULL_IN;
    uint32_t vEnd = Settings::Stepper::PULL_OUT;
    uint32_t accel = Settings::Stepper::MAX_ACCEL;

    m_planner.prepare(m_leadStepper->getPosition(), m_leadStepper == &m_sA ? targetA : m_leadStepper == &m_sB ? targetB : targetC,
                      targetSpeed, vStart, vEnd, accel);

    m_errorB = 2 * m_deltaB - m_leadDelta;
    m_errorC = 2 * m_deltaC - m_leadDelta;

    m_stepsDoneA = m_stepsDoneB = m_stepsDoneC = 0;
    m_startTime = micros() * 1e-6;

    if (m_leadStepper == &m_sA)      m_leadStepsDone = &m_stepsDoneA;
    else if (m_leadStepper == &m_sB) m_leadStepsDone = &m_stepsDoneB;
    else if (m_leadStepper == &m_sC) m_leadStepsDone = &m_stepsDoneC;




    m_startTime = micros() * 1e-6;

    m_stepsDoneA = m_stepsDoneB = m_stepsDoneC = 0;


    if (m_leadStepper == nullptr || m_leadDelta == 0) {
        Console::error("setTarget") << "Lead stepper not assigned properly!" << Console::endl;
        complete(); // or throw an assert if critical
    }
    
}

void StepperController::run() {
    if (!isPending()) return;

    int32_t speed = m_planner.updateSpeed(m_leadStepper->getPosition());

    if (speed > 0) {
        if (m_deltaA > 0) {
            // Instead of computing µs delay:
            float velA = speed * (m_deltaA / (float)m_leadDelta);
            float final_vel = (targetA >= m_startA ? 1 : -1) * velA;
            m_sA.setVelocity(final_vel);
            //Console::info("StepperA") << final_vel << Console::endl;
        }

        if (m_deltaB > 0) {
            float velB = speed * (m_deltaB / (float)m_leadDelta);
            float final_vel = (targetB >= m_startB ? 1 : -1) * velB;
            m_sB.setVelocity(final_vel);
            //Console::info("StepperB") << final_vel << Console::endl;
        }

        if (m_deltaC > 0) {
            float velC = speed * (m_deltaC / (float)m_leadDelta);
            float final_vel = (targetC >= m_startC ? 1 : -1) * velC;
            m_sC.setVelocity(final_vel);
            //Console::info("StepperC") << final_vel << Console::endl;
        }

    } else {
        Console::success() << Console::microTimeStamp() << " : Motion complete (speed = 0)" << Console::endl;
        complete();
    }
}




void StepperController::control() {
    if (!isPending()) return;

    // --- Always step the motors (high-frequency) ---
    m_sA.step();
    m_sB.step();
    m_sC.step();

    // --- Run control logic less frequently ---
    static unsigned long last_compute = micros();
    if (micros() - last_compute < Settings::Stepper::STEPPER_COMPUTE_DELAY) return;
    last_compute = micros();

    bool steppedLead = false;

    // --- Step lead stepper ---
    if (*m_leadStepsDone < m_leadDelta && m_leadStepper->shouldStep()) {
        m_leadStepper->step();
        (*m_leadStepsDone)++;
        steppedLead = true;
        // Console::info("Lead stepper stepped. Total: ") << *m_leadStepsDone << Console::endl;
    }

    // --- Step B if not lead ---
    if (&m_sB != m_leadStepper && m_stepsDoneB < m_deltaB && steppedLead) {
        m_errorB += 2 * m_deltaB;
        if (m_errorB >= m_leadDelta) {
            m_sB.step();
            m_stepsDoneB++;
            m_errorB -= 2 * m_leadDelta;
        }
    }

    // --- Step C if not lead ---
    if (&m_sC != m_leadStepper && m_stepsDoneC < m_deltaC && steppedLead) {
        m_errorC += 2 * m_deltaC;
        if (m_errorC >= m_leadDelta) {
            m_sC.step();
            m_stepsDoneC++;
            m_errorC -= 2 * m_leadDelta;
        }
    }

    // --- Step A if not lead ---
    if (&m_sA != m_leadStepper && m_stepsDoneA < m_deltaA && steppedLead) {
        m_errorA += 2 * m_deltaA;
        if (m_errorA >= m_leadDelta) {
            m_sA.step();
            m_stepsDoneA++;
            m_errorA -= 2 * m_leadDelta;
        }
    }

    // --- Check if all steppers are done ---
    if (m_stepsDoneA >= m_deltaA &&
        m_stepsDoneB >= m_deltaB &&
        m_stepsDoneC >= m_deltaC)
    {
        Console::success() << Console::microTimeStamp() << " : All axes complete, calling complete()" << Console::endl;
        complete();
    }
}




    
// pause() – Set target velocities to zero and change state.
void StepperController::pause() {
    if (!isRunning()) return;

    Job::pause();

    // --- Step 1: Ask planner for remaining steps to stop ---
    int32_t stepsRemaining = m_planner.initiateStop(m_leadStepper->getPosition());

    if (stepsRemaining <= 0) {
        Console::error("pause") << "Planner returned zero steps. Completing immediately." << Console::endl;
        complete();
        return;
    }

    // --- Step 2: Calculate new stop position along original motion ---
    long currentLeadPos = m_leadStepper->getPosition();
    int direction = (m_finalTargetA >= m_startA) ? 1 : -1;

    long newLeadTarget = currentLeadPos + direction * stepsRemaining;
    float leadProgress = (float)(newLeadTarget - m_startA) / (float)m_leadDelta;

    long posA = m_sA.getPosition();
    long posB = m_sB.getPosition();
    long posC = m_sC.getPosition();

    long tmpTargetA = posA + direction * round(m_deltaA * leadProgress);
    long tmpTargetB = posB + direction * round(m_deltaB * leadProgress);
    long tmpTargetC = posC + direction * round(m_deltaC * leadProgress);

    Console::trace("pause") << "Remaining=" << stepsRemaining
                            << ", New Targets: A=" << tmpTargetA
                            << ", B=" << tmpTargetB
                            << ", C=" << tmpTargetC << Console::endl;

    // --- Step 3: Plan the short deceleration segment ---
    setTarget(tmpTargetA, tmpTargetB, tmpTargetC);

    // --- Step 4: Validate lead was set correctly ---
    if (!m_leadStepper || !m_leadStepsDone) {
        Console::error("pause") << "Lead stepper not set after replan." << Console::endl;
        complete();
        return;
    }

    start();  // continue execution with new short move
}



// resume() – When in Paused state, simply restart the move using new start time.
void StepperController::resume() {
    if (!isPaused()) return;

    Console::trace("resume") << "Resuming to final targets: A=" << m_finalTargetA
                             << ", B=" << m_finalTargetB
                             << ", C=" << m_finalTargetC << Console::endl;

    // Current position becomes new start
    long posA = m_sA.getPosition();
    long posB = m_sB.getPosition();
    long posC = m_sC.getPosition();

    m_startA = posA;
    m_startB = posB;
    m_startC = posC;

    setTarget(m_finalTargetA, m_finalTargetB, m_finalTargetC);
    start();

    Job::resume();
}


// complete() – Immediately stop and reset the trajectory data, then disable the steppers.
void StepperController::complete() {
    m_sA.setVelocity(0);
    m_sB.setVelocity(0);
    m_sC.setVelocity(0);
    m_sA.disable();
    m_sB.disable();
    m_sC.disable();
    // Reset internal move parameters.
    m_startA = m_startB = m_startC = 0;
    targetA = targetB = targetC = 0;
    m_totalDistanceA = m_totalDistanceB = m_totalDistanceC = 0;
    m_totalTime = 0;
    m_startTime = 0;
    Job::complete();
}

// cancel() – Decelerate (using pause) then complete the move.
void StepperController::cancel() {
    if (!isPending()) return;

    Console::trace("cancel") << "Cancelling with deceleration." << Console::endl;

    // Smooth deceleration to stop
    Job::cancel();

    int32_t stepsRemaining = m_planner.initiateStop(m_leadStepper->getPosition());
    if (stepsRemaining <= 0) {
        Console::info("cancel") << "Nothing to decelerate — stopping immediately." << Console::endl;
        complete();
        return;
    }

    long currentLeadPos = m_leadStepper->getPosition();
    int direction = (m_finalTargetA >= m_startA) ? 1 : -1;

    long newLeadTarget = currentLeadPos + direction * stepsRemaining;
    float leadProgress = (float)(newLeadTarget - m_startA) / (float)m_leadDelta;

    long posA = m_sA.getPosition();
    long posB = m_sB.getPosition();
    long posC = m_sC.getPosition();

    long tmpTargetA = posA + direction * round(m_deltaA * leadProgress);
    long tmpTargetB = posB + direction * round(m_deltaB * leadProgress);
    long tmpTargetC = posC + direction * round(m_deltaC * leadProgress);

    setTarget(tmpTargetA, tmpTargetB, tmpTargetC);

    if (!m_leadStepper || !m_leadStepsDone) {
        Console::error("cancel") << "Invalid lead stepper state — forcing complete." << Console::endl;
        complete();
        return;
    }

    start();
}


// reset() – If a move is running, cancel it; then reset all internal parameters.
void StepperController::reset() {
    if (isPending()) {
        cancel();
    }
    Job::reset();
    m_startA = m_startB = m_startC = 0;
    m_finalTargetA = m_finalTargetB = m_finalTargetC = 0;
    targetA = targetB = targetC = 0;
    m_totalDistanceA = m_totalDistanceB = m_totalDistanceC = 0;
    m_totalTime = 0;
    m_startTime = 0;

    m_sA.setPosition(0);
    m_sB.setPosition(0);
    m_sC.setPosition(0);
}


