#include "terminal.h"
#include "os.h"

Terminal::Terminal() : Service(TERMINAL){
    os.screen.addBootProgress(10);
	os.screen.drawBootProgress("Loading Terminal...");
}

void Terminal::update(){
    if(!m_enabled) return;

    
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');  // Read the serial command
        os.console.trace("Terminal") << "Command received at " << int(millis()) << "ms" << os.console.endl;
        _pendingCommands.push_back(command);  // Assuming _pendingCommands holds a struct or class that can take these 3 parameters
        if(_pendingCommands.size() > 50) _pendingCommands.pop_front();
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
