#include "core/Program.h"

namespace TwinSystem{

    Program::Program(){}

    void Program::Execute(){
        for (auto& b : _blocks) {
            b->Execute();
        }
    }

    void Program::Add(Shared<Instruction> i){
        if(_currentBlock == nullptr){   
            Begin();
        }
        _currentBlock->AddInstruction(i);
    }

    void Program::Begin(){
        Shared<Block> b = CreateShared<Block>();
    }

    void Program::End(){
        _blocks.push_back(_currentBlock);
    }
}