#pragma once
#include <Arduino.h>
#include <FastCRC.h>

extern FastCRC8 CRC8;
bool checkCRC(const String& str, const uint8_t crc);