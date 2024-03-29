#include "ConsoleStream.h"
#include "Console.h"
#include "core/Core.h"

namespace TwinSystem{

	ConsoleStream::ConsoleStream(ConsoleLevel lvl, const String& origin) {
		_level = lvl;
		_ignored = false;
        if(lvl < Console::GetLevel()) _ignored = true;

        if (!_ignored) {
			if (origin == "") Console::write(header().c_str());
			else Console::write((header() + "(" + origin + "): ").c_str());
		}
	}

	String ConsoleStream::header() {
		String str;

		switch (_level) {
		case ConsoleLevel::_TRACE:
			str = "[Trace]";
			break;
		case ConsoleLevel::_INFO:
			str = "[Info]";
			break;
		case ConsoleLevel::_WARNING:
			str = "[Warning]";
			break;
		case ConsoleLevel::_ERROR:
			str = "[Error]";
			break;
		case ConsoleLevel::_SUCCESS:
			str = "[OK]";
			break;
		case ConsoleLevel::_CRITICAL:
			str = "[Fatal]";
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

} // namespace TwinSystem
