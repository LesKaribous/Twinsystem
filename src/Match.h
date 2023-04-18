#pragma once

namespace TwinSystem{

    class Match{
    public:

        enum class State{
            IDLE,
            STARTED,
            FINISHED
        };

        Match();
        void Start();
        bool IsFinished();
        bool IsNearlyFinished();

        void AddToScore(int points, int multiplicateur = 1);

        int GetScore();
        int GetTimeLeftSeconds();
        unsigned long GetTimeLeft();
        unsigned long GetElapsedTime();

    private:
        void UpdateTime();

        unsigned long _elapsed;
        unsigned long _startTime;
        unsigned long _timeLeft;
        int _score;

        State _state;
    };


}