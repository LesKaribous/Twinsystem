#include "chrono.h"
#include "utils/timer/timer.h"

#include "services/ihm/ihm.h"

INSTANTIATE_SERVICE(Chrono, chrono)

void Chrono::onAttach(){
    setDuration(Settings::Match::DURATION);
}

void Chrono::onUpdate(){
    if(!nearlyFinished && getTimeLeft() <= Settings::Match::NEARLY_FINISH && getTimeLeft() >= Settings::Match::ENDMATCH){
        nearlyFinished = true;
        onMatchNearlyFinished();
    }

    if(!finished && getTimeLeft() <= Settings::Match::ENDMATCH){
        finished = true;
        onMatchFinished();
    }

    ihm.setTime(getTimeLeftSeconds());
}

void Chrono::onMatchNearlyFinished(){
    if(m_matchNearEnd_callback != nullptr)m_matchNearEnd_callback();
}

void Chrono::onMatchFinished(){
    if(m_matchEnd_callback != nullptr)m_matchEnd_callback();
}

void Chrono::setNearEndCallback(routine_ptr cb){
    m_matchNearEnd_callback = cb;
}

void Chrono::setEndCallback(routine_ptr cb){
    m_matchEnd_callback = cb;
}