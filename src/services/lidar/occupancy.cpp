#include "occupancy.h"

SINGLETON_INSTANTIATE(OccupancyMap, occupancy);

OccupancyMap::OccupancyMap() {
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            m_map[i][j] = 0;
        }
    }
}

bool OccupancyMap::isInBounds(int x, int y) const {
    return (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT);
}

float OccupancyMap::distanceToNearestObstacle(const Vec2& pos) const {
    int gx = static_cast<int>((pos.x + TABLE_SIZE_X) * GRID_WIDTH / (2 * TABLE_SIZE_X));
    int gy = static_cast<int>((pos.y + TABLE_SIZE_Y) * GRID_HEIGHT / (2 * TABLE_SIZE_Y));

    if(!isInBounds(gx, gy))
        return false;
    
    float minDistance = std::numeric_limits<float>::max();
    
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            if (m_map[i][j] > 0) { // If occupied
                float distance = Vec2(i * 2 * TABLE_SIZE_X / GRID_WIDTH - TABLE_SIZE_X, 
                                      j * 2 * TABLE_SIZE_Y / GRID_HEIGHT - TABLE_SIZE_Y).dist(pos);
                if (distance < minDistance) {
                    minDistance = distance;
                }
            }
        }
    }
    
    return minDistance;
}

Vec2 OccupancyMap::repulsiveGradient(const Vec2 &pos) const{
    const int RANGE = 5; // Neighborhood size (5x5)
    Vec2 force(0.0f, 0.0f);

    int cx = static_cast<int>((pos.x + TABLE_SIZE_X) * GRID_WIDTH / (2 * TABLE_SIZE_X));
    int cy = static_cast<int>((pos.y + TABLE_SIZE_Y) * GRID_HEIGHT / (2 * TABLE_SIZE_Y));

    for (int dx = -RANGE; dx <= RANGE; ++dx) {
        for (int dy = -RANGE; dy <= RANGE; ++dy) {
            int nx = cx + dx;
            int ny = cy + dy;

            if (!isInBounds(nx, ny))
                continue;

            if (isCellOccupied(nx, ny)) {
                Vec2 obs(static_cast<float>(nx), static_cast<float>(ny));
                Vec2 diff = pos - obs;
                float distSq = diff.x * diff.x + diff.y * diff.y + 1e-5f;

                // Inverse square law for repulsion
                force = force + (diff / distSq);
            }
        }
    }

    return force;
}

void OccupancyMap::decompress(const String& encoded) {
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

bool OccupancyMap::isOccupied(int x, int y) const {
    // Convert world coordinates to grid coordinates
    int gx = static_cast<int>((x + TABLE_SIZE_X) * GRID_WIDTH / (2 * TABLE_SIZE_X));
    int gy = static_cast<int>((y + TABLE_SIZE_Y) * GRID_HEIGHT / (2 * TABLE_SIZE_Y));

    return isCellOccupied(gx, gy);
}

bool OccupancyMap::isCellOccupied(int gx, int gy) const {
    if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT) {
        return false;
    }
    return m_map[gx][gy] > 0;
}
