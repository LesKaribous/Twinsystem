#include "chrono/chrono.h"
#include "settings.h"

namespace TwinSystem{
        
    Chronometer::Chronometer(){
        _elapsed = 0;
        _timeLeft = Settings::Match::DURATION;
        _startTime = 0;
        _score = 0;
        _state = State::IDLE;
    }

    void Chronometer::Start(){
        if(_state == State::IDLE){
            _startTime = millis();
            _state = State::STARTED;
        }
    }

    bool Chronometer::IsFinished(){
        UpdateTime();
        return _state == State::FINISHED;
    }

    bool Chronometer::IsNearlyFinished(){
        UpdateTime();
        return _timeLeft <= 5000; // <= 5s
    }

    void Chronometer::AddToScore(int points, int multiplicateur){
        _score = _score + (points * multiplicateur);
    }

    int  Chronometer::GetScore(){
        return _score;
    }

    void Chronometer::UpdateTime(){
        if(_state == State::STARTED){
            _elapsed = millis() - _startTime;
            _timeLeft = Settings::Match::DURATION - _elapsed;
            if(_timeLeft <= 0) _state = State::FINISHED;
        }
    }

    int Chronometer::GetTimeLeftSeconds(){
        UpdateTime();
        return _timeLeft/1000;
    }

    unsigned long Chronometer::GetTimeLeft(){
        UpdateTime();
        return _timeLeft;
    }

    unsigned long Chronometer::GetElapsedTime(){
        UpdateTime();
        return _elapsed;
    }

} // namespace TwinSsytem