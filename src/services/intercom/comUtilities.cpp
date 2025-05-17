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

String compressData(const BinaryPayload& payload) {
    if (!payload.data || payload.size == 0)
        return "";

    const uint8_t* data = payload.data;
    size_t size = payload.size;

    // CRC-8 (simple XOR, or replace with table-based CRC if needed)
    uint8_t crc = 0;
    for (size_t i = 0; i < size; ++i)
        crc ^= data[i];

    // Hex encode
    String result;
    result.reserve(size * 2 + 3); // preallocate for performance

    for (size_t i = 0; i < size; ++i) {
        if (data[i] < 16) result += '0';
        result += String(data[i], HEX);
    }

    // Append CRC
    result += '-';
    if (crc < 16) result += '0';
    result += String(crc, HEX);

    return result;
}
