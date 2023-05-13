#pragma once
#include "core/lib.h"
#include "consoleStream.h"

namespace TwinSystem {

	class ConsoleStream;

	class Console{
	public: 
		friend class ConsoleStream;
		static String endl;

		static void Initialize();

		static inline ConsoleLevel GetLevel() { return _level; };
		static inline void SetLevel(ConsoleLevel l) { _level = l; };

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
}