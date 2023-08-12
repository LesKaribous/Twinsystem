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

	bool isIgnored(ServiceID);
	inline ConsoleLevel getLevel() { return m_level; };
	inline void setLevel(ConsoleLevel l) { m_level = l; };


	//Use the current log level
	ConsoleStream info(const String& origin);
	ConsoleStream warn(const String& origin);
	ConsoleStream error(const String& origin);
	ConsoleStream trace(const String& origin);
	ConsoleStream success(const String& origin);

	//Use the current log level
	ConsoleStream info(const ServiceID& origin = NOT_A_SERVICE);
	ConsoleStream warn(const ServiceID& origin = NOT_A_SERVICE);
	ConsoleStream error(const ServiceID& origin = NOT_A_SERVICE);
	ConsoleStream trace(const ServiceID& origin = NOT_A_SERVICE);
	ConsoleStream success(const ServiceID& origin = NOT_A_SERVICE);




	void plot(const String& name, String s);

	void print(const String& s);
	void println(const String& s);
	void prettyPrint(const String& s);

private:
	void write(const char* str);
 	ConsoleLevel m_level;
	std::vector<ServiceID> m_activeServices;
};
