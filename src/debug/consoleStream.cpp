#include "consoleStream.h"
#include "os.h"
#include "console.h"

ConsoleStream::ConsoleStream(ConsoleLevel lvl, const String& origin) {
	_ignored = false;
	if(lvl < Console::getLevel()) _ignored = true;

	if (!_ignored) {
		if (origin == ID_NOT_A_SERVICE) Console::write(header(lvl).c_str());
		else Console::write((header(lvl) + "(" + origin + "): ").c_str());
	}
}

String ConsoleStream::header(ConsoleLevel lvl) {
	String str;

	switch (lvl) {
	case ConsoleLevel::VERBOSE:
		str = "[Trace]";
		break;
	case ConsoleLevel::INFO:
		str = "[Info]";
		break;
	case ConsoleLevel::WARNING:
		str = "[Warning]";
		break;
	case ConsoleLevel::CRITICAL:
		str = "[Error]";
		break;
	case ConsoleLevel::SUCCESS:
		str = "[OK]";
		break;
	default:
		str = "";
		break;
	}
	return(str);
}


ConsoleStream& ConsoleStream::operator<<(short i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(int i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(long i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(size_t i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(float i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(Vec2 i) {
	if (_ignored) return *this;
	Console::print("{");
	Console::print(i.a);
	Console::print(",");
	Console::print(i.b);
	Console::print("}");
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(Vec3 i) {
	if (_ignored) return *this;
	Console::print("{");
	Console::print(i.a);
	Console::print(",");
	Console::print(i.b);
	Console::print(",");
	Console::print(i.c);
	Console::print("}");
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(double i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(char i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(const String& i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(const char* i) {
	if (_ignored) return *this;
	Console::print(i);
	return *this;
}

