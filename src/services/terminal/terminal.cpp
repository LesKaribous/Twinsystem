#include "terminal.h"
#include "os.h"

Terminal::Terminal() : Service(TERMINAL){
    os.screen.addBootProgress(10);
	os.screen.drawBootProgress("Loading Terminal...");
}

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
