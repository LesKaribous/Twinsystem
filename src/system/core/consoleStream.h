#pragma once
#include "system/core/lib.h"
#include "consoleLevel.h"

#include "system/core/service.h"
#include "system/math/geometry.h"

class ConsoleStream {
public:

	ConsoleStream(Console& console, ConsoleLevel lvl, ServiceID origin);
	ConsoleStream(Console& console, ConsoleLevel lvl, const String& origin);
	
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
    Console& m_console;
	static String header(ConsoleLevel lvl);
	bool _ignored;
};
