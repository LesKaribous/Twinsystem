#include "chrono.h"
#include "utils/timer/timer.h"

#include "services/ihm/ihm.h"

SINGLETON_INSTANTIATE(Chrono, chrono)

void Chrono::attach(){
    setDuration(Settings::Match::DURATION);
}

void Chrono::run(){
    if(!nearlyFinished && getTimeLeft() <= Settings::Match::NEARLY_FINISH && getTimeLeft() >= Settings::Match::ENDMATCH){
        onMatchNearlyFinished();
    }

    if(!finished && getTimeLeft() <= Settings::Match::ENDMATCH){
        onMatchFinished();
    }

    ihm.setTime(getTimeLeftSeconds());
}

void Chrono::onMatchNearlyFinished(){
    nearlyFinished = true;
    if(m_matchNearEnd_callback != nullptr)m_matchNearEnd_callback();
}

void Chrono::onMatchFinished(){
    finished = true;
    if(m_matchEnd_callback != nullptr)m_matchEnd_callback();
}

void Chrono::setNearEndCallback(routine_ptr cb){
    m_matchNearEnd_callback = cb;
}

void Chrono::setEndCallback(routine_ptr cb){
    m_matchEnd_callback = cb;
}