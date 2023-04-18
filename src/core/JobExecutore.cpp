#include "core/JobExecutor.h"
#include "debug/Console.h"

namespace TwinSystem{


	String  JobExecutor::ToString(){
		switch(_state){
			case JobState::IDLE :
				return "IDLE";
			break;
			case JobState::RUNNING :
				return "RUNNING";
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

	bool JobExecutor::IsPaused(){
		return _state == JobState::PAUSED;
	}

    bool JobExecutor::IsBusy(){
		return _state == JobState::RUNNING || _state == JobState::PAUSED;
	}

	bool JobExecutor::IsRunning(){
        return _state == JobState::RUNNING;
    }

    bool JobExecutor::IsIdle(){
        return _state == JobState::IDLE;
    }
    
    bool JobExecutor::IsCancelled(){
        return _state == JobState::CANCELLED;
    }
    

	void  JobExecutor::Pause(){
		if(_state == JobState::RUNNING){
			_state = JobState::PAUSED;
		}
	}

    void  JobExecutor::Resume(){
		if(_state == JobState::PAUSED){
			_state = JobState::RUNNING;
		}
	}

    void  JobExecutor::Cancel(){
		if(_state != JobState::IDLE && _state != JobState::FINISHED){
			_state = JobState::CANCELLED;
		}
	}
	
    void  JobExecutor::Finish(){
		_state = JobState::FINISHED;
		_state = JobState::IDLE;
	}



} // namespace TwinSystem
