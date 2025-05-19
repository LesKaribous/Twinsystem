#include "job_owner.h"

JobOwner::JobOwner(){
    m_job = nullptr;
}

bool JobOwner::isIdle()const{
    if(m_job == nullptr) return false;
    return m_job->isIdle();
}

bool JobOwner::isPausing()const{
    if(m_job == nullptr) return false;
    return m_job->isPausing();
}

bool JobOwner::isPaused() const{
    if(m_job == nullptr) return false;
    return m_job->isPaused();
}

bool JobOwner::isRunning()const{
    if(m_job == nullptr) return false;
    return m_job->isRunning();
}

bool JobOwner::isPending()const{
    if(m_job == nullptr) return false;
    return m_job->isPending();
}

bool JobOwner::isBusy()const{
    if(m_job == nullptr) return false;
    return m_job->isBusy();
}

bool JobOwner::isCompleted()const{
    if(m_job == nullptr) return false;
    return m_job->isCompleted();
}

bool JobOwner::isCanceling()const{
    if(m_job == nullptr) return false;
    return m_job->isCanceling();
}

bool JobOwner::isCanceled()const{
    if(m_job == nullptr) return true;
    return m_job->isCanceled();
}

void JobOwner::run(){
    if(m_job == nullptr) return;
    return m_job->run();
}

void JobOwner::reset(){
    if(m_job == nullptr) return;
    return m_job->reset();
}

void JobOwner::start(){
    if(m_job == nullptr) return;
    return m_job->start();
}

void JobOwner::pause(){
    if(m_job == nullptr) return;
    return m_job->pause();
}

void JobOwner::resume(){
    if(m_job == nullptr) return;
    return m_job->resume();
}

void JobOwner::cancel(){
    if(m_job == nullptr) return;
    return m_job->cancel();
}

void JobOwner::forceCancel(){
    if(m_job == nullptr) return;
    return m_job->forceCancel();
}

void JobOwner::complete(){
    if(m_job == nullptr) return;
    return m_job->complete();
}


void JobOwner::onPausing(){
    if(m_job == nullptr) return;
    return m_job->onPausing();
}

void JobOwner::onCanceling(){
    if(m_job == nullptr) return;
    return m_job->onCanceling();
}

void JobOwner::onPaused(){
    if(m_job == nullptr) return;
    return m_job->onPaused();
}

void JobOwner::onCanceled(){
    if(m_job == nullptr) return;
    return m_job->onCanceled();
}
