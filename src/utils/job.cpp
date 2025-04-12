#include "job.h"

Job::Job(){
	m_state = JobState::IDLE;
	startTime = 0;
	pauseTime = 0;
}

Job::~Job(){

}

String  Job::toString() const {
	switch(m_state){
		case JobState::IDLE :
			return "IDLE";
		break;
		case JobState::RUNNNING :
			return "RUNNNING";
		break;
		case JobState::PAUSED :
			return "PAUSED";
		break;
		case JobState::CANCELLED :
			return "CANCELLED";
		break;
		case JobState::COMPLETED :
			return "COMPLETED";
		break;
		default:
			return "NONE";
		break;
	}
	//Console::log("Collision :", Intercom::collision(), INFO);
}

bool Job::isPaused() const {
	return m_state == JobState::PAUSED;
}

bool Job::isIdle() const {
	return m_state == JobState::IDLE;
}

bool Job::isRunning() const{
	return m_state == JobState::RUNNNING;
}

bool Job::isPending() const{
	return m_state == JobState::RUNNNING || m_state == JobState::PAUSED ;
}

bool Job::isCancelled() const{
	return m_state == JobState::CANCELLED;
}


bool Job::isCompleted() const{
	return m_state == JobState::COMPLETED;
}

long Job::elaspedTime() const{
	if(startTime == 0) return 0;
    else return millis() - startTime;
}

long Job::pauseDuration() const
{
	if(pauseTime == 0) return 0;
    else return millis() - pauseTime;
}

void Job::reset(){
	startTime = 0;
	pauseTime = 0;
    m_state = JobState::IDLE;
}

void  Job::start(){
	if(m_state == JobState::IDLE){
		startTime = millis();
		m_state = JobState::RUNNNING;
	}
}

void  Job::pause(){
	if(m_state == JobState::RUNNNING){
		pauseTime = millis();
		m_state = JobState::PAUSED;
	}
}

void  Job::resume(){
	if(m_state == JobState::PAUSED){
		m_state = JobState::RUNNNING;
	}
}

void  Job::cancel(){
	m_state = JobState::CANCELLED;
}

void  Job::complete(){
	m_state = JobState::COMPLETED;
}

