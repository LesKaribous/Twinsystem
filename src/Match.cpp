#include "Match.h"
#include "Settings.h"

namespace TwinSystem{
        
    Match::Match(){
        _elapsed = 0;
        _timeLeft = Settings::Match::MATCH_DURATION;
        _startTime = 0;
        _score = 0;
    }

    void Match::Start(){
        if(_state == State::IDLE){
            _startTime = millis();
            _state = State::STARTED;
        }
    }

    bool Match::IsFinished(){
        UpdateTime();
        return _state == State::FINISHED;
    }

    bool Match::IsNearlyFinished(){
        UpdateTime();
        return _timeLeft <= 5000; // <= 5s
    }

    void Match::AddToScore(int points, int multiplicateur){
        _score = _score + (points * multiplicateur);
    }

    int  Match::GetScore(){
        return _score;
    }

    void Match::UpdateTime(){
        if(_state == State::STARTED){
            _elapsed = millis() - _startTime;
            _timeLeft = Settings::Match::MATCH_DURATION - _elapsed;
            if(_timeLeft <= 0) _state = State::FINISHED;
        }
    }

    int Match::GetTimeLeftSeconds(){
        UpdateTime();
        return _timeLeft/1000;
    }

    unsigned long Match::GetTimeLeft(){
        UpdateTime();
        return _timeLeft;
    }

    unsigned long Match::GetElapsedTime(){
        UpdateTime();
        return _elapsed;
    }

} // namespace TwinSsytem