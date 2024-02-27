#include "terminal.h"
#include "system/core/os.h"
#include "system/core/console.h"

Terminal::Terminal() : Service(TERMINAL){}

void Terminal::update(){
    if(!m_enabled) return;

    
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
