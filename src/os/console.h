#pragma once
#include "settings.h"
#include "services/service.h"
#include "debug/consoleStream.h"

#define HERE " [" + String(__FILE__) + " at line " + String(__LINE__) + "]"
#define THROW(x) Console::println( "Throw in " + String(__FILE__) + " at line " + String(__LINE__) + " : " + x);

class ConsoleStream;

class Console{
public: 
	friend class ConsoleStream;
	static const String endl;

	static inline ConsoleLevel getLevel() { return m_level; };
	static inline void setLevel(ConsoleLevel l) { m_level = l; };

	//Use the current log level
	static void init();
	
	static String timeStamp();
	static String microTimeStamp();
	
	static ConsoleStream info(const String& origin);
	static ConsoleStream warn(const String& origin);
	static ConsoleStream error(const String& origin);
	static ConsoleStream trace(const String& origin);
	static ConsoleStream success(const String& origin);

	//Use the current log level
	static ConsoleStream info(const ServiceID& origin = ID_NOT_A_SERVICE);
	static ConsoleStream warn(const ServiceID& origin = ID_NOT_A_SERVICE);
	static ConsoleStream error(const ServiceID& origin = ID_NOT_A_SERVICE);
	static ConsoleStream trace(const ServiceID& origin = ID_NOT_A_SERVICE);
	static ConsoleStream success(const ServiceID& origin = ID_NOT_A_SERVICE);

	static void plot(const String& name, String s);
	static void plotXY(const String& n, String x, String y);

	static void print(const String& s);
	static void println(const String& s);
	static void prettyPrint(const String& s);
	static void line();

private:
	static void write(const char* str);
 	static ConsoleLevel m_level;
};
