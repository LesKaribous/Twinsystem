#pragma once
#include "core/lib.h"
#include "consoleStream.h"

#define THROW(x) Console::println( "Throw in " + String(__FILE__) + " at line " + String(__LINE__) + " : " + x);

class ConsoleStream;

class Console{
public: 
	friend class ConsoleStream;
	static String endl;

	static void initialize(ConsoleLevel lvl = ConsoleLevel::_INFO);

	static inline ConsoleLevel getLevel() { return _level; };
	static inline void setLevel(ConsoleLevel l) { _level = l; };

	//Use the current log level
	static ConsoleStream trace(String origin = "");
	static ConsoleStream info(String origin = "");
	static ConsoleStream warn(String origin = "");
	static ConsoleStream error(String origin = "");
	static ConsoleStream success(String origin = "");
	static ConsoleStream critical(String origin = "");
	
	static void print(String s);
	static void println(String s);

private:
	static void write(const char* str);
	static ConsoleLevel _level;
};
