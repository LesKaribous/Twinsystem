#include "ConsoleStream.h"
#include <string>


namespace TwinSystem{
    
	ConsoleStream::ConsoleStream(ConsoleLevel lvl, std::string origin) {
		_level = lvl;
        if(lvl < Console::GetLevel()) _ignored = true;

        if (!_ignored) {
			if (origin == "") Console::write(header());
			else Console::write(header() + "(" + origin + "): ");
		}
	}

	std::string ConsoleStream::header() {
		std::string str;

		switch (_level) {
		case ConsoleLevel::_TRACE:
			str = "[Trace]: ";
			break;
		case ConsoleLevel::_INFO:
			str = "[Info]: ";
			break;
		case ConsoleLevel::_WARNING:
			str = "[Warning]: ";
			break;
		case ConsoleLevel::_ERROR:
			str = "[Error]: ";
			break;
		case ConsoleLevel::_SUCCESS:
			str = "[OK]: ";
			break;
		case ConsoleLevel::_CRITICAL:
			str = "[Fatal]: ";
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

	ConsoleStream& ConsoleStream::operator<<(const std::string& i) {
		if (_ignored) return *this;
		Console::print(i);
		return *this;
	}

	ConsoleStream& ConsoleStream::operator<<(const char* i) {
		if (_ignored) return *this;
		Console::print(i);
		return *this;
	}

	ConsoleStream& ConsoleStream::operator<<(const void* i) {
		if (_ignored) return *this;
		std::string str;
		std::stringstream address;
		address << i;
		str += address.str();
		Console::write(str.c_str());
		return *this;
	}

	ConsoleStream& ConsoleStream::operator<<(std::ostream os){
		if (_ignored) return *this;
		
		std::string str;
		std::stringstream ss;
		ss << os.rdbuf();
		str += ss.str();

		Console::write(str.c_str());
		return *this;
	}
    
} // namespace TwinSystem
