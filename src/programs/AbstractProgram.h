
enum class ProgramStatus { 


};

class Program{

    virtual void execute() = 0;

    virtual void start();
    virtual void stop();
    virtual void restart();
    


}