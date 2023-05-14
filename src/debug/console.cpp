#include "console.h"

#include <Arduino.h>


String Console::endl = "\n";
ConsoleLevel Console::_level = ConsoleLevel::_INFO;

ConsoleStream Console::trace(String origin) {
	return ConsoleStream(ConsoleLevel::_TRACE, origin);
}

ConsoleStream Console::info(String origin) {
	return ConsoleStream(ConsoleLevel::_INFO, origin);
}

ConsoleStream Console::warn(String origin) {
	return ConsoleStream(ConsoleLevel::_WARNING, origin);
}

ConsoleStream Console::error(String origin) {
	return ConsoleStream(ConsoleLevel::_ERROR, origin);
}

ConsoleStream Console::success(String origin) {
	return ConsoleStream(ConsoleLevel::_SUCCESS, origin);
}

ConsoleStream Console::critical(String origin) {
	return ConsoleStream(ConsoleLevel::_CRITICAL, origin);
}

void header(){
	Serial.println("  _______       _                     _"                        );
	Serial.println(" |__   __|     (_)                   | |"                       );
	Serial.println("    | |_      ___ _ __  ___ _   _ ___| |_ ___ _ __ ___  "       );
	Serial.println("    | \\ \\ /\\ / / | '_ \\/ __| | | / __| __/ _ \\ '_ ` _ \\"  );
	Serial.println("    | |\\ V  V /| | | | \\__ \\ |_| \\__ \\ ||  __/ | | | | |"  );
	Serial.println("    |_| \\_/\\_/ |_|_| |_|___/\\__, |___/\\__\\___|_| |_| |_|"  );
	Serial.println("                             __/ |"                             );
	Serial.println("                            |___/"                              );
	Serial.println();
	Serial.println("Author  : Nadarbreicq, JulesTopart ");
	Serial.println();
	Serial.print("Twinsystem... compiled  ");
	Serial.print(__DATE__);
	Serial.print(" at ");
	Serial.println(__TIME__);

}


void Console::initialize(){
	Serial.begin(115200);
	header();
	
}

void Console::write(const char* str) {
	Serial.write(str);
}

void Console::print(String s){
	Serial.print(s);
}

void Console::println(String s){
	Serial.println(s);
}


