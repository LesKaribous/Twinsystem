#pragma once

#include <string>
#include <sstream>

#include "ConsoleLevel.h"
#include "ConsoleStream.h"

namespace TwinSystem {

	class ConsoleStream;

	class Console{
	public: 
		friend class ConsoleStream;
		static std::string endl;

		static void Initialize();

		static inline ConsoleLevel GetLevel() { return _level; };
		static inline void SetLevel(ConsoleLevel l) { _level = l; };

         //Use the current log level
		static ConsoleStream trace(std::string origin = "");
		static ConsoleStream info(std::string origin = "");
		static ConsoleStream warn(std::string origin = "");
		static ConsoleStream error(std::string origin = "");
		static ConsoleStream success(std::string origin = "");
		static ConsoleStream critical(std::string origin = "");
		
		static void print(char c);
		static void print(const char s[]);
		static void print(uint8_t n, int base = 10);
		static void print(int n, int base = 10);
		static void print(unsigned int n, int base= 10);
		static void print(long n, int base= 10);
		static void print(unsigned long n, int base);
		static void print(int64_t n, int base= 10);
		static void print(uint64_t n, int base= 10);
		static void print(double n, int digits = 4);
        static void print(std::string content);

		static void println(char c);
		static void println(const char s[]);
		static void println(uint8_t n, int base);
		static void println(int n, int base);
		static void println(unsigned int n, int base);
		static void println(long n, int base);
		static void println(unsigned long n, int base);
		static void println(int64_t n, int base);
		static void println(uint64_t n, int base);
		static void println(double n, int digits = 4);
        static void println(std::string content);

		static bool pollEvents();

	private:
		static void write(std::string str);
	    static ConsoleLevel _level;
	};
}