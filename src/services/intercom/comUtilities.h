#pragma once
#include <Arduino.h>
#include <FastCRC.h>

extern FastCRC8 CRC8;
void serializeUInt32(byte (&buf)[4], uint32_t uval);
uint32_t parseUInt32(const byte (&buf)[4]);
bool checkCRC(const String& str, const char crc);