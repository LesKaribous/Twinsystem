#include "comUtilities.h"
#include <FastCRC.h>
#include "os/console.h"

FastCRC8 CRC8;

bool checkCRC(const String& str, const uint8_t crc){
    uint8_t mcrc = CRC8.smbus((uint8_t*)str.c_str(), str.length());
    // Console::print(mcrc);
    // Console::print("|");
    // Console::println(crc);
    return mcrc == crc;
}
