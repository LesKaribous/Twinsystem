#include "console.h"
#include "os.h"
#include <Arduino.h>

const String Console::endl = "\n";

ConsoleStream Console::trace(const ServiceID& origin) {
	return ConsoleStream(ConsoleLevel::VERBOSE, origin);
}

ConsoleStream Console::info(const ServiceID& origin) {
	return ConsoleStream(ConsoleLevel::INFO, origin);
}

ConsoleStream Console::warn(const ServiceID& origin) {
	return ConsoleStream(ConsoleLevel::WARNING, origin);
}

ConsoleStream Console::error(const ServiceID& origin) {
	return ConsoleStream(ConsoleLevel::CRITICAL, origin);
}

ConsoleStream Console::success(const ServiceID& origin) {
	return ConsoleStream(ConsoleLevel::SUCCESS, origin);
}


ConsoleStream Console::info(const String& origin) {
	return ConsoleStream(ConsoleLevel::INFO, origin);
}

ConsoleStream Console::warn(const String& origin) {
	return ConsoleStream(ConsoleLevel::WARNING, origin);
}

ConsoleStream Console::error(const String& origin) {
	return ConsoleStream(ConsoleLevel::CRITICAL, origin);
}

ConsoleStream Console::trace(const String& origin) {
	return ConsoleStream(ConsoleLevel::VERBOSE, origin);
}

ConsoleStream Console::success(const String& origin) {
	return ConsoleStream(ConsoleLevel::SUCCESS, origin);
}


void header(){
	Serial.println("");
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

Console::Console(ConsoleLevel lvl){
	Serial.begin(115200);
	setLevel(lvl);
	header();
}

void Console::write(const char* str) {
	Serial.write(str);
}

void Console::plot(const String& n, String s){
	os.console.print(">" + n + ":"); os.console.println(s);
}

void Console::print(const String& s){
	Serial.print(s);
}

void Console::println(const String& s){
	Serial.println(s);
}

void Console::prettyPrint(const String& s){
	int l = 0;
	Serial.print(l);
	Serial.print(":\t");
	for (size_t i = 0; i <= s.length(); i++){
		if(s[i] == '\n'){
			Serial.println();
			Serial.print(++l);
			Serial.print(":\t");
		}else{
			Serial.print(s[i]);
		}
	}
	Serial.print(s[s.length() - 1]);
}