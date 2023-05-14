#pragma once
#include "core/lib.h"
#include "consoleLevel.h"

#include "math/geometry.h"

class ConsoleStream {
public:

	ConsoleStream(ConsoleLevel lvl, const String& origin = "");
	
	ConsoleStream& operator<<(short);
	ConsoleStream& operator<<(int);
	ConsoleStream& operator<<(long);
	ConsoleStream& operator<<(float);
	ConsoleStream& operator<<(Vec2);
	ConsoleStream& operator<<(Vec3);
	ConsoleStream& operator<<(double);

	ConsoleStream& operator<<(char);
	ConsoleStream& operator<<(const String&);
	ConsoleStream& operator<<(const char*);

private:
	String header();
	
	ConsoleLevel _level;
	bool _ignored;
};
