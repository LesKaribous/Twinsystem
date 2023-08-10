#include "system/core/job.h"
#include "os.h"

Job::Job(){
	m_state = JobState::IDLE;
}

Job::~Job(){

}

String  Job::toString() const {
	switch(m_state){
		case JobState::IDLE :
			return "IDLE";
		break;
		case JobState::PENDING :
			return "PENDING";
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
	//os.console.log("Collision :", Intercom::collision(), INFO);
}

bool Job::isPaused() {
	return m_state == JobState::PAUSED;
}

bool Job::isIdle() {
	return m_state == JobState::IDLE;
}

bool Job::isPending(){
	return m_state == JobState::PENDING;
}

bool Job::isCancelled(){
	return m_state == JobState::CANCELLED;
}


bool Job::isCompleted(){
	return m_state == JobState::COMPLETED;
}

void  Job::reset(){
	m_state = JobState::IDLE;
}

void  Job::start(){
	if(m_state == JobState::IDLE){
		m_state = JobState::PENDING;
	}
}

void  Job::pause(){
	if(m_state == JobState::PENDING){
		m_state = JobState::PAUSED;
	}
}

void  Job::resume(){
	if(m_state == JobState::PAUSED){
		m_state = JobState::PENDING;
	}
}

void  Job::cancel(){
	if(m_state != JobState::COMPLETED){
		m_state = JobState::CANCELLED;
	}
}

void  Job::complete(){
	m_state = JobState::COMPLETED;
}

