#include "planner/job.h"
#include "debug/console.h"


String  Job::toString(){
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
		case JobState::FINISHED :
			return "FINISHED";
		break;
		default:
			return "NONE";
		break;
	}
	//Console::log("Collision :", Intercom::collision(), INFO);
}

bool Job::isPaused(){
	return m_state == JobState::PAUSED;
}

bool Job::isPending(){
	return m_state == JobState::PENDING;
}

bool Job::isCancelled(){
	return m_state == JobState::CANCELLED;
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
	if(m_state != JobState::IDLE && m_state != JobState::FINISHED){
		m_state = JobState::CANCELLED;
	}
}

void  Job::finish(){
	m_state = JobState::FINISHED;
	m_state = JobState::IDLE;
}

