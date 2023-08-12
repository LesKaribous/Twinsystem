#include "consoleStream.h"
#include "os.h"


ConsoleStream::ConsoleStream(ConsoleLevel lvl, ServiceID origin) {
	_ignored = false;
	if((lvl < ConsoleLevel::WARNING && !os.debug(origin))) _ignored = true;

	if (!_ignored) {
		if (origin == NOT_A_SERVICE) os.console.write(header(lvl).c_str());
		else os.console.write((header(lvl) + "(" + Service::toString(origin) + "): ").c_str());
	}
}

ConsoleStream::ConsoleStream(ConsoleLevel lvl, const String& origin) {
	_ignored = false;
	if((lvl < ConsoleLevel::WARNING && !os.debug(Service::toID(origin)))) _ignored = true;

	if (!_ignored) {
		if (origin == NOT_A_SERVICE) os.console.write(header(lvl).c_str());
		else os.console.write((header(lvl) + "(" + origin + "): ").c_str());
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
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(int i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(long i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(size_t i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(float i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(Vec2 i) {
	if (_ignored) return *this;
			os.console.print("{");
	os.console.print(i.a);
	os.console.print(",");
	os.console.print(i.b);
	os.console.print("}");
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(Vec3 i) {
	if (_ignored) return *this;
	os.console.print("{");
	os.console.print(i.a);
	os.console.print(",");
	os.console.print(i.b);
	os.console.print(",");
	os.console.print(i.c);
	os.console.print("}");
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(double i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(char i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(const String& i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

ConsoleStream& ConsoleStream::operator<<(const char* i) {
	if (_ignored) return *this;
	os.console.print(i);
	return *this;
}

