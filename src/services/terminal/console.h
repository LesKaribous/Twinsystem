#pragma once
#include "system/core/lib.h"
#include "consoleStream.h"

class ConsoleStream;

class Console{
public: 
	friend class ConsoleStream;
	static const String endl;

	Console(ConsoleLevel lvl = ConsoleLevel::INFO);
	~Console(){};

	inline ConsoleLevel getLevel() { return _level; };
	inline void setLevel(ConsoleLevel l) { _level = l; };

	//Use the current log level
	ConsoleStream trace(String origin = "");
	ConsoleStream info(String origin = "");
	ConsoleStream warn(String origin = "");
	ConsoleStream success(String origin = "");
	ConsoleStream error(String origin = "");
	
	void print(String s);
	void println(String s);

private:
	void write(const char* str);
 	ConsoleLevel _level;
};
