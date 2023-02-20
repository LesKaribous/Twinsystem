#pragma once
#include "core/Core.h"
#include <vector>

namespace TwinSystem{
    class Instruction;
    class Block;

    class Program{
    public:
        Program();
        void Execute();

        //Add an instruction to the current block
        void Add(Shared<Instruction> i);

        //Add new block and listen to incoming instructions
        void Begin();
        void End();


    private:
        std::vector<Shared<Block>> _blocks;
        Shared<Block> _currentBlock;
    };


    enum class InstructionStatus {
        NotExecuted,
        Success,
        Failed
    };

    class Instruction {
    public:
        Instruction();

        inline void operator()() { fc(error); }
        inline InstructionStatus getStatus() const{return status;};

        inline bool isExecuted() const { return executed;}
        inline bool isSuccessful() const { return successful;}

    protected:
        bool executed;
        bool successful;
        InstructionStatus status;
        std::string error;

    private:
        std::function<bool(std::string &)> fc;
    };

    class Block : Instruction {
    public:
        Block();    

        void AddInstruction(Shared<Instruction> i);
        void Execute();

    private:
        std::vector<Shared<Instruction>> instructions;
    };

} // namespace TwinSystem
