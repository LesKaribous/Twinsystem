#include "lidar.h"
#include "os/console.h"
#include "services/intercom/intercom.h"
//#include "services/navigation/navigation.h"
#include "services/motion/motion.h"

INSTANTIATE_SERVICE(Lidar, lidar)

Lidar::Lidar() : Service(ID_LIDAR){}

void Lidar::onAttach(){
    if(!intercom.enabled()){
        Console::error("Lidar") << "is not enabled" << Console::endl;
    }
}

void Lidar::onUpdate(){
    static Vec3 pos;
    if(enabled()){
        //if((millis() - m_lastPosUpdate > 100 && Vec3::distanceBetween(pos, nav.getPosition()) > 10) || millis() - m_lastPosUpdate > 1000){
        if(/*(*/millis() - m_lastPosUpdate > 100 /*&& Vec3::distanceBetween(pos, motion.estimatedPosition()) > 10)*/ || millis() - m_lastPosUpdate > 1000){
            m_lastPosUpdate = millis();
            //pos = nav.getPosition();
            pos = motion.estimatedPosition();
            intercom.sendRequest("pos(" + String(pos.x)  + "," + String(pos.y) + "," + String(pos.z*RAD_TO_DEG) + ")", 100);
        }
        /*
        if(millis() - m_lastOccupancyRequest > 150){
            m_lastOccupancyRequest = millis();
            intercom.sendRequest("getOccupancyMap()", 1000, onOccupancyResponse, onOccupancyTimeout);
        }*/
    }
}

void Lidar::enable(){
    Service::enable();
}

void Lidar::disable(){
    Service::disable();
}

void Lidar::showRadarLED(){
    intercom.sendRequest("on",100);
    Console::println("displayLidar");
}

void Lidar::showStatusLED(){
    intercom.sendRequest("off",100);
    Console::println("displayIntercom");
}

void Lidar::decompressOccupancyMap(const String& encoded) {
    if (encoded.length() < (GRID_BYTES * 2 + 3)) // hex chars + dash + CRC
        return;

    uint8_t map[GRID_WIDTH][GRID_HEIGHT] = {0};

    int sep = encoded.lastIndexOf('-');
    if (sep == -1 || sep + 2 > encoded.length()) return;

    String dataHex = encoded.substring(0, sep);
    String crcHex = encoded.substring(sep + 1);

    // Decode hex into bytes
    uint8_t data[GRID_BYTES] = {0};
    for (int i = 0; i < GRID_BYTES; ++i) {
        String byteStr = dataHex.substring(i * 2, i * 2 + 2);
        data[i] = strtoul(byteStr.c_str(), nullptr, 16);
    }

    // CRC check (XOR-based)
    uint8_t crc = 0;
    for (int i = 0; i < GRID_BYTES; ++i)
        crc ^= data[i];

    uint8_t receivedCRC = strtoul(crcHex.c_str(), nullptr, 16);
    if (crc != receivedCRC)
        return;

    // ✅ Unpack in the same order as packing: y outer, x inner
    int bitIndex = 0;
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            int byteIndex = bitIndex / 8;
            int bitInByte = bitIndex % 8;
            map[x][y] = (data[byteIndex] >> bitInByte) & 0x01;
            ++bitIndex;
        }
    }

    // Copy to internal buffer
    for (int x = 0; x < GRID_WIDTH; ++x)
        for (int y = 0; y < GRID_HEIGHT; ++y)
            m_map[x][y] = map[x][y];
}


bool Lidar::isOccupied(int x, int y)
{
    int gx = (x * GRID_WIDTH) / 3000;
    int gy = (y * GRID_HEIGHT) / 2000;

    if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT)
        return false;

    return m_map[gx][gy] == 1;
}
