#pragma once
#include "consoleLevel.h"
#include "utils/geometry.h"

class ConsoleStream {
public:

	ConsoleStream(ConsoleLevel lvl, const String& origin);
	
	ConsoleStream& operator<<(short);
	ConsoleStream& operator<<(int);
	ConsoleStream& operator<<(long);
	ConsoleStream& operator<<(size_t);
	ConsoleStream& operator<<(float);
	ConsoleStream& operator<<(Vec2);
	ConsoleStream& operator<<(Vec3);
	ConsoleStream& operator<<(double);

	ConsoleStream& operator<<(char);
	ConsoleStream& operator<<(const String&);
	ConsoleStream& operator<<(const char*);

private:
	static String header(ConsoleLevel lvl);
	bool _ignored;
};
