#include "terminal.h"
#include "os/console.h"

INSTANTIATE_SERVICE(Terminal)

Terminal::Terminal() : Service(ID_TERMINAL){}

void Terminal::onAttach(){
    Console::info() << "Terminal activated" << Console::endl;
}   

void Terminal::onUpdate(){
    if(!enabled()) return;
    
    if (Serial.available() > 0) {
        String command = Serial.readString();
        _pendingCommands.push_back(command);
        if(_pendingCommands.size() > 10) _pendingCommands.pop_front();
        
    }
}   

String Terminal::dequeCommand(){
    String command = _pendingCommands.front();
    _pendingCommands.pop_front();
    return command;
}

int Terminal::commandAvailable(){
    return _pendingCommands.size();
}
