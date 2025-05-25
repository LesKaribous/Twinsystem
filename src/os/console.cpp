#include "console.h"
#include <Arduino.h>

const String Console::endl = "\n";
ConsoleLevel Console::m_level = ConsoleLevel::INFO;

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
	return ConsoleStream( ConsoleLevel::SUCCESS, origin);
}

String Console::timeStamp(){
	String time = String(millis());
	return String("[t=" + time + "ms]");
}

String Console::microTimeStamp(){
	String time = String(micros());
	return String("[t=" + time + "us]");
}


ConsoleStream Console::info(const String &origin)
{
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
	CONSOLE_SERIAL.println("");
	CONSOLE_SERIAL.println("  _______       _                     _"                        );
	CONSOLE_SERIAL.println(" |__   __|     (_)                   | |"                       );
	CONSOLE_SERIAL.println("    | |_      ___ _ __  ___ _   _ ___| |_ ___ _ __ ___  "       );
	CONSOLE_SERIAL.println("    | \\ \\ /\\ / / | '_ \\/ __| | | / __| __/ _ \\ '_ ` _ \\"  );
	CONSOLE_SERIAL.println("    | |\\ V  V /| | | | \\__ \\ |_| \\__ \\ ||  __/ | | | | |"  );
	CONSOLE_SERIAL.println("    |_| \\_/\\_/ |_|_| |_|___/\\__, |___/\\__\\___|_| |_| |_|"  );
	CONSOLE_SERIAL.println("                             __/ |"                             );
	CONSOLE_SERIAL.println("                            |___/"                              );
	CONSOLE_SERIAL.println();
	CONSOLE_SERIAL.println("Author  : Nadarbreicq, JulesTopart ");
	CONSOLE_SERIAL.println();
	CONSOLE_SERIAL.print("Twinsystem... compiled  ");
	CONSOLE_SERIAL.print(__DATE__);
	CONSOLE_SERIAL.print(" at ");
	CONSOLE_SERIAL.println(__TIME__);

}

void Console::init(){
	CONSOLE_SERIAL.begin(CONSOLE_BAUDRATE);
	header();
}

void Console::write(const char* str) {
	CONSOLE_SERIAL.write(str);
}

void Console::plot(const String& n, String s){
	print(">" + n + ":"); println(s);
}

void Console::plotXY(const String& n, String x, String y){
	println(">" + n + ":" + x + ":" + y + "|xy"); 
}

void Console::print(const String& s){
	CONSOLE_SERIAL.print(s);
}

void Console::println(const String& s){
	CONSOLE_SERIAL.println(s);
}

void Console::prettyPrint(const String& s){
	int l = 0;
	line();
	CONSOLE_SERIAL.print(l);
	CONSOLE_SERIAL.print(":\t");
	for (size_t i = 0; i <= s.length(); i++){
		if(s[i] == '\n'){
			CONSOLE_SERIAL.println();
			CONSOLE_SERIAL.print(++l);
			CONSOLE_SERIAL.print(":\t");
		}else{
			CONSOLE_SERIAL.print(s[i]);
		}
	}
	CONSOLE_SERIAL.println(s[s.length() - 1]);
	line();
}

void Console::line(){
	println("_________________________________________");
}