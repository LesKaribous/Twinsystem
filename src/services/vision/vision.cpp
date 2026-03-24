#include "vision.h"
#include "os/console.h"
#include <Arduino.h>
#include <cmath>
#include <cstring>

SINGLETON_INSTANTIATE(TwinVision, vision)

TwinVision::TwinVision()
    : Service(ID_VISION), m_stream(VISION_SERIAL) {}


// ============================================================
//  Couleurs — utilitaires globaux
// ============================================================

// FLASHMEM : fonctions non time-critical → restent en flash (8MB), libèrent l'ITCM (512KB)
FLASHMEM const char* colorName(ObjectColor c) {
    switch (c) {
        case ObjectColor::RED:     return "rouge";
        case ObjectColor::GREEN:   return "vert";
        case ObjectColor::BLUE:    return "bleu";
        case ObjectColor::YELLOW:  return "jaune";
        case ObjectColor::WHITE:   return "blanc";
        case ObjectColor::BLACK:   return "noir";
        case ObjectColor::NONE:    return "aucun";
        default:                   return "inconnu";
    }
}

FLASHMEM ObjectColor colorFromChar(char c) {
    switch (c) {
        case 'R': return ObjectColor::RED;
        case 'G': return ObjectColor::GREEN;
        case 'B': return ObjectColor::BLUE;
        case 'Y': return ObjectColor::YELLOW;
        case 'W': return ObjectColor::WHITE;
        case 'K': return ObjectColor::BLACK;
        case 'N': return ObjectColor::NONE;
        default:  return ObjectColor::UNKNOWN;
    }
}

FLASHMEM char colorToChar(ObjectColor c) {
    switch (c) {
        case ObjectColor::RED:    return 'R';
        case ObjectColor::GREEN:  return 'G';
        case ObjectColor::BLUE:   return 'B';
        case ObjectColor::YELLOW: return 'Y';
        case ObjectColor::WHITE:  return 'W';
        case ObjectColor::BLACK:  return 'K';
        case ObjectColor::NONE:   return 'N';
        default:                  return '?';
    }
}


// ============================================================
//  Service lifecycle
// ============================================================

FLASHMEM void TwinVision::attach() {
    Console::info("Vision") << "TwinVision attache" << Console::endl;
    VISION_SERIAL.begin(VISION_BAUDRATE);
    while (m_stream.available()) m_stream.read(); // vider le buffer
    ping();
}

void TwinVision::run() {
    if (!enabled()) return;

    if (m_stream.available()) {
        m_lastStream = millis();
        processIncoming();
    }

    processPending();

    // Keepalive
    if (millis() - m_lastPing > PING_INTERVAL) {
        ping();
        // Connexion perdue si aucun stream depuis 3 pings
        if (m_connected && millis() - m_lastStream > PING_INTERVAL * 3) {
            m_connected = false;
            Console::warn("Vision") << "Connexion perdue" << Console::endl;
        }
    }
}


// ============================================================
//  API publique — couleur
// ============================================================

FLASHMEM ObjectColor TwinVision::queryColorSync(Vec2 pos, uint32_t timeoutMs) {
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "color:%d:%d", (int)pos.x, (int)pos.y);
    int uid = sendQuery(cmd);

    uint32_t start = millis();
    while (millis() - start < timeoutMs) {
        run(); // traite les réponses entrantes
        PendingQuery* pq = findPending(uid);
        if (pq && pq->resolved) {
            ObjectColor result = pq->colorResult;
            pq->used = false; // libère le slot
            return result;
        }
    }

    // Timeout — marquer le slot comme libéré et retourner UNKNOWN
    PendingQuery* pq = findPending(uid);
    if (pq) pq->used = false;

    Console::warn("Vision")
        << "queryColorSync timeout (" << (int)timeoutMs << "ms) @ "
        << (int)pos.x << "," << (int)pos.y
        << Console::endl;
    return ObjectColor::UNKNOWN;
}

FLASHMEM void TwinVision::queryColor(Vec2 pos, ColorCallback cb, uint32_t timeoutMs) {
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "color:%d:%d", (int)pos.x, (int)pos.y);
    int uid = sendQuery(cmd);

    PendingQuery* pq = findPending(uid);
    if (pq) {
        pq->colorCb    = cb;
        pq->timeoutMs  = timeoutMs;
        pq->pos        = pos;
    }
}

FLASHMEM void TwinVision::requestMapUpdate() {
    sendQuery("map");
    Console::info("Vision") << "Map update requested" << Console::endl;
}

FLASHMEM void TwinVision::requestAI(const char* type, const char* params,
                            AICallback cb, uint32_t timeoutMs) {
    char cmd[80];
    snprintf(cmd, sizeof(cmd), "ai:%s:%s", type, params);
    int uid = sendQuery(cmd);

    PendingQuery* pq = findPending(uid);
    if (pq) {
        pq->isAI      = true;
        pq->aiCb      = cb;
        pq->timeoutMs = timeoutMs;
    }
    Console::info("Vision")
        << "AI request: " << type << "(" << params << ")"
        << Console::endl;
}


// ============================================================
//  Cache
// ============================================================

ObjectColor TwinVision::getColor(Vec2 pos) const {
    for (uint8_t i = 0; i < m_cacheCount; i++) {
        const CacheEntry& e = m_cache[i];
        if (!e.valid) continue;

        float dx = e.pos.x - pos.x;
        float dy = e.pos.y - pos.y;
        if (sqrtf(dx*dx + dy*dy) > POS_TOLERANCE) continue;

        // Vérifier la fraîcheur
        if (m_cacheMaxAge > 0 && millis() - e.updatedMs > m_cacheMaxAge)
            return ObjectColor::UNKNOWN;

        return e.color;
    }
    return ObjectColor::UNKNOWN;
}

FLASHMEM void TwinVision::setCacheMaxAge(uint32_t ms) {
    m_cacheMaxAge = ms;
}

void TwinVision::updateCache(Vec2 pos, ObjectColor color) {
    CacheEntry* entry = findCacheEntry(pos);
    if (!entry) entry = allocCacheEntry(pos);
    if (!entry) return; // cache plein

    entry->pos       = pos;
    entry->color     = color;
    entry->updatedMs = millis();
    entry->valid     = true;

    Console::info("Vision")
        << "Cache: (" << (int)pos.x << "," << (int)pos.y << ") = "
        << colorName(color)
        << Console::endl;
}

TwinVision::CacheEntry* TwinVision::findCacheEntry(Vec2 pos) {
    for (uint8_t i = 0; i < m_cacheCount; i++) {
        CacheEntry& e = m_cache[i];
        if (!e.valid) continue;
        float dx = e.pos.x - pos.x;
        float dy = e.pos.y - pos.y;
        if (sqrtf(dx*dx + dy*dy) <= POS_TOLERANCE) return &e;
    }
    return nullptr;
}

TwinVision::CacheEntry* TwinVision::allocCacheEntry(Vec2 pos) {
    if (m_cacheCount < MAX_CACHE) {
        CacheEntry& e = m_cache[m_cacheCount++];
        e.pos   = pos;
        e.valid = false;
        return &e;
    }
    // Cache plein : recycler la plus ancienne
    CacheEntry* oldest = &m_cache[0];
    for (uint8_t i = 1; i < MAX_CACHE; i++) {
        if (m_cache[i].updatedMs < oldest->updatedMs)
            oldest = &m_cache[i];
    }
    Console::warn("Vision") << "Cache plein, recyclage entree ancienne" << Console::endl;
    return oldest;
}


// ============================================================
//  Connexion
// ============================================================

bool TwinVision::isConnected() const {
    return m_connected;
}

void TwinVision::ping() {
    m_lastPing = millis();
    if (m_stream.availableForWrite()) {
        m_stream.print("ping\n");
    }
}


// ============================================================
//  Pending queries — gestion des slots
// ============================================================

int TwinVision::allocPending() {
    for (uint8_t i = 0; i < MAX_PENDING; i++) {
        if (!m_pending[i].used) return i;
    }
    // Tous les slots occupés : forcer-libérer le plus ancien
    int oldest_idx  = 0;
    uint32_t oldest = m_pending[0].sentMs;
    for (uint8_t i = 1; i < MAX_PENDING; i++) {
        if (m_pending[i].sentMs < oldest) {
            oldest     = m_pending[i].sentMs;
            oldest_idx = i;
        }
    }
    Console::warn("Vision") << "Pending plein, slot " << oldest_idx << " recycle" << Console::endl;
    return oldest_idx;
}

TwinVision::PendingQuery* TwinVision::findPending(int uid) {
    for (uint8_t i = 0; i < MAX_PENDING; i++) {
        if (m_pending[i].used && m_pending[i].uid == uid)
            return &m_pending[i];
    }
    return nullptr;
}

int TwinVision::sendQuery(const char* cmd) {
    int uid = m_uidCounter++;
    int idx = allocPending();

    PendingQuery& pq = m_pending[idx];
    pq.uid         = uid;
    pq.sentMs      = millis();
    pq.resolved    = false;
    pq.used        = true;
    pq.isAI        = false;
    pq.colorCb     = nullptr;
    pq.aiCb        = nullptr;
    pq.aiResult[0] = '\0';

    // Format sur la stack — pas de heap
    char buf[96];
    snprintf(buf, sizeof(buf), "Q:%d:%s\n", uid, cmd);
    m_stream.print(buf);

    return uid;
}


// ============================================================
//  Traitement des messages entrants
// ============================================================

void TwinVision::processIncoming() {
    // Buffer de ligne statique — pas de heap
    static char line[128];

    while (m_stream.available()) {
        // Lire jusqu'au \n dans le buffer statique
        uint8_t len = 0;
        while (m_stream.available() && len < sizeof(line) - 1) {
            char c = (char)m_stream.read();
            if (c == '\n') break;
            if (c != '\r') line[len++] = c;
        }
        line[len] = '\0';
        if (len == 0) continue;

        if (strcmp(line, "pong") == 0) {
            if (!m_connected) {
                m_connected = true;
                Console::info("Vision") << "TwinVision connecte" << Console::endl;
            }
            continue;
        }

        if (strcmp(line, "ping") == 0) {
            m_stream.print("pong\n");
            continue;
        }

        // R:uid:payload  — réponse à une requête
        if (line[0] == 'R' && line[1] == ':') {
            char* p = line + 2;
            char* sep = strchr(p, ':');
            if (!sep) continue;
            *sep = '\0';
            int uid = atoi(p);
            parseResponse(uid, sep + 1);
            continue;
        }

        // B:type:payload  — broadcast proactif
        if (line[0] == 'B' && line[1] == ':') {
            char* p = line + 2;
            char* sep = strchr(p, ':');
            if (!sep) continue;
            *sep = '\0';
            parseBroadcast(p, sep + 1);
            continue;
        }

        Console::trace("Vision") << "msg inconnu: " << line << Console::endl;
    }
}

FLASHMEM void TwinVision::parseResponse(int uid, const char* payload) {
    PendingQuery* pq = findPending(uid);

    // ---- Réponse couleur : char unique
    if (!pq || !pq->isAI) {
        if (payload[0] != '\0' && payload[1] == '\0') {
            ObjectColor color = colorFromChar(payload[0]);
            if (pq) {
                updateCache(pq->pos, color);
                pq->colorResult = color;
                pq->resolved    = true;
                if (pq->colorCb) {
                    pq->colorCb(pq->pos, color);
                    pq->used = false;
                }
            }
            return;
        }

        // ---- Réponse map : "x1,y1,C1;x2,y2,C2;..."
        if (strchr(payload, ';') || strchr(payload, ',')) {
            // Copie de travail sur la stack pour strtok
            char buf[128];
            strncpy(buf, payload, sizeof(buf) - 1);
            buf[sizeof(buf) - 1] = '\0';

            char* token = strtok(buf, ";");
            while (token) {
                // token = "x,y,C"
                char* c1 = strchr(token, ',');
                if (c1) {
                    char* c2 = strchr(c1 + 1, ',');
                    if (c2) {
                        float x  = atof(token);
                        float y  = atof(c1 + 1);
                        char  ch = *(c2 + 1);
                        updateCache(Vec2(x, y), colorFromChar(ch));
                    }
                }
                token = strtok(nullptr, ";");
            }
            if (pq) { pq->resolved = true; pq->used = false; }
            return;
        }
    }

    // ---- Réponse AI : chaîne arbitraire ----
    if (pq && pq->isAI) {
        strncpy(pq->aiResult, payload, AI_RESULT_SIZE - 1);
        pq->aiResult[AI_RESULT_SIZE - 1] = '\0';
        pq->resolved = true;

        Console::info("Vision") << "AI [" << uid << "]: " << pq->aiResult << Console::endl;

        if (pq->aiCb) {
            pq->aiCb(pq->aiResult);
            pq->used = false;
        }
    }
}

FLASHMEM void TwinVision::parseBroadcast(const char* type, const char* payload) {
    // Parseur CSV commun : x,y,third
    auto parseXYC = [](const char* s, float& x, float& y, char& third) -> bool {
        char buf[48];
        strncpy(buf, s, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        char* c1 = strchr(buf, ',');
        if (!c1) return false;
        char* c2 = strchr(c1 + 1, ',');
        if (!c2) return false;
        x     = atof(buf);
        y     = atof(c1 + 1);
        third = *(c2 + 1);
        return true;
    };

    // B:obj:x,y,C  — mise à jour d'un objet
    if (strcmp(type, "obj") == 0) {
        float x, y; char ch;
        if (parseXYC(payload, x, y, ch))
            updateCache(Vec2(x, y), colorFromChar(ch));
        return;
    }

    // B:adv:x,y,heading  — position adversaire
    if (strcmp(type, "adv") == 0) {
        float x, y; char dummy;
        if (parseXYC(payload, x, y, dummy)) {
            // TODO: occupancy.setAdversaryPosition(Vec2(x, y));
            Console::info("Vision")
                << "Adv @ (" << (int)x << "," << (int)y << ")"
                << Console::endl;
        }
        return;
    }

    Console::trace("Vision") << "broadcast inconnu: " << type << Console::endl;
}


// ============================================================
//  processPending — timeout des requêtes non répondues
// ============================================================

void TwinVision::processPending() {
    for (uint8_t i = 0; i < MAX_PENDING; i++) {
        PendingQuery& pq = m_pending[i];
        if (!pq.used || pq.resolved) continue;

        if (millis() - pq.sentMs > pq.timeoutMs) {
            // Timeout
            if (pq.colorCb) pq.colorCb(pq.pos, ObjectColor::UNKNOWN);
            if (pq.aiCb)    pq.aiCb("timeout");
            pq.used = false;
        }
    }
}
