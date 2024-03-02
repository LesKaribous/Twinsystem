#pragma once
#define HERE " [" + String(__FILE__) + " at line " + String(__LINE__) + "]"
#define THROW(x) Console::println( "Throw in " + String(__FILE__) + " at line " + String(__LINE__) + " : " + x);