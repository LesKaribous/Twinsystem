
enum class ProgramStatus { 


};

class AbstractProgram{

    virtual void execute() = 0;

    virtual void start();
    virtual void stop();
    virtual void restart();
    


}