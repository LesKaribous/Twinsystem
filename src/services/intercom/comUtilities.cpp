#include "comUtilities.h"
#include <FastCRC.h>

FastCRC8 CRC8;

//https://codereview.stackexchange.com/questions/83170/int32-to-char4-conversion-functions
void serializeUInt32(uint8_t (&buf)[4], uint32_t uval){
    buf[0] = uval;
    buf[1] = uval >> 8;
    buf[2] = uval >> 16;
    buf[3] = uval >> 24;
}

uint32_t parseUInt32(const uint8_t (&buf)[4])
{
    // This prevents buf[i] from being promoted to a signed int.
    uint32_t u0 = buf[0], u1 = buf[1], u2 = buf[2], u3 = buf[3];
    uint32_t uval = u0 | (u1 << 8) | (u2 << 16) | (u3 << 24);
    return uval;
}

bool checkCRC(const String& str, const char crc){
    return CRC8.smbus((byte*)str.c_str(), str.length()) == crc;
}

