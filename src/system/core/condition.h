#pragma once
#include "system/core/lib.h"
#include "system/core/interpreter.h"


class Command;

class Condition{
public:
    bool _result;

    std::vector<String> operators;
    std::vector<Command> commands;
    std::vector<String> outputs;

    void solve();

    inline void addCommandOuput(String str){outputs.push_back(str);}
    inline bool getResult() const {return _result;}

    Condition(String raw);
    ~Condition();
};











