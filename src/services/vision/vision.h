#pragma once
#include "services/service.h"
#include "utils/geometry.h"
#include "config/settings.h"

// ============================================================
//  TwinVision — Liaison avec la caméra centrale
//
//  La caméra observe la table depuis le dessus et fournit :
//    - La couleur des objets aux positions demandées
//    - Une vue d'ensemble de la table (map complète)
//    - La position de l'adversaire (broadcast proactif)
//    - Du calcul AI déporté (optimisation, inférence ML)
//
//  Communication : XBee sur VISION_SERIAL (Serial2 par défaut)
//
//  Protocole (ASCII, terminé par \n) :
//    Robot → Vision  :  Q:uid:cmd\n          (requête)
//    Vision → Robot  :  R:uid:payload\n      (réponse)
//    Vision → Robot  :  B:type:payload\n     (broadcast proactif)
//    Keepalive       :  ping\n  /  pong\n
//
//  Commandes :
//    Q:uid:color:x:y          → R:uid:C        (C = couleur)
//    Q:uid:map                → R:uid:x1,y1,C1;x2,y2,C2;...
//    Q:uid:ai:type:params     → R:uid:result_string
//
//  Broadcasts :
//    B:obj:x,y,C              → mise à jour d'un objet
//    B:adv:x,y,heading        → position adversaire
//
//  Couleurs (char) :
//    R=rouge  G=vert  B=bleu  Y=jaune  W=blanc  K=noir
//    N=aucun objet   ?=inconnu
//
//  Usage typique dans un bloc Mission :
//    ObjectColor col = vision.queryColorSync(POI::testA);
//    if (col == ObjectColor::NONE) return BlockResult::FAILED;
//    // ... collecter selon la couleur
// ============================================================


// ------------------------------------------------------------
//  ObjectColor
// ------------------------------------------------------------

enum class ObjectColor : uint8_t {
    UNKNOWN = 0, // Jamais interrogé ou réponse non reçue
    NONE,        // Interrogé : aucun objet à cette position
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE,
    BLACK,
};

// Nom lisible d'une couleur (pour les logs)
const char* colorName(ObjectColor c);

// Encode / décode le char de protocole
ObjectColor colorFromChar(char c);
char        colorToChar(ObjectColor c);


// ------------------------------------------------------------
//  Callbacks
// ------------------------------------------------------------

using ColorCallback = void (*)(Vec2 pos, ObjectColor color);
using AICallback    = void (*)(const char* result);


// ------------------------------------------------------------
//  TwinVision
// ------------------------------------------------------------

class TwinVision : public Service {
public:
    TwinVision();

    void attach() override;
    void run() override;

    // ================================================================
    //  Requêtes couleur
    // ================================================================

    // Sync — bloque jusqu'à réponse ou timeout.
    // Safe depuis les blocs Mission (utilise run() en interne).
    ObjectColor queryColorSync(Vec2 pos, uint32_t timeoutMs = 500);

    // Async — retourne immédiatement, cb appelé quand la réponse arrive.
    // cb reçoit ObjectColor::UNKNOWN en cas de timeout.
    void queryColor(Vec2 pos, ColorCallback cb, uint32_t timeoutMs = 500);

    // ================================================================
    //  Map complète
    // ================================================================

    // Demande une mise à jour complète de la table (async).
    // Met à jour le cache pour tous les objets visibles.
    void requestMapUpdate();

    // ================================================================
    //  Cache — accès immédiat, sans serial
    // ================================================================

    // Dernière couleur connue à cette position.
    // Retourne UNKNOWN si jamais interrogé, ou si l'entrée est périmée.
    ObjectColor getColor(Vec2 pos) const;

    // Durée max avant qu'une entrée cache soit considérée périmée (ms).
    // 0 = jamais périmée. Défaut : 30000 ms.
    void setCacheMaxAge(uint32_t ms);

    // ================================================================
    //  AI offload
    // ================================================================

    // Envoie une requête arbitraire au moteur AI de TwinVision.
    // type   : type de calcul  (ex: "prioritize", "path", "predict")
    // params : paramètres CSV  (ex: "500,500,1000,500")
    // cb     : callback appelé avec la réponse string (nullptr = fire-and-forget)
    void requestAI(const char* type, const char* params,
                   AICallback cb = nullptr, uint32_t timeoutMs = 2000);

    // ================================================================
    //  Connexion
    // ================================================================

    bool isConnected() const;

    SINGLETON(TwinVision);

private:
    // ---- Capacités ----
    static constexpr uint8_t  MAX_CACHE      = 16;  // 16 POIs max en cache (~256 bytes)
    static constexpr uint8_t  MAX_PENDING    = 4;   // 4 requêtes simultanées max
    static constexpr uint8_t  AI_RESULT_SIZE = 48;  // chars max pour une réponse AI
    static constexpr uint32_t PING_INTERVAL  = 1000;    // ms
    static constexpr float    POS_TOLERANCE  = 80.0f;   // mm — seuil de correspondance cache

    // ---- Cache de la table (digital twin) ----
    struct CacheEntry {
        Vec2        pos;
        ObjectColor color      = ObjectColor::UNKNOWN;
        uint32_t    updatedMs  = 0;
        bool        valid      = false;
    };

    // ---- Requêtes en attente ----
    struct PendingQuery {
        int          uid        = -1;
        Vec2         pos;
        uint32_t     sentMs     = 0;
        uint32_t     timeoutMs  = 500;
        bool         isAI       = false;
        bool         resolved   = false;
        bool         used       = false;
        ObjectColor  colorResult = ObjectColor::UNKNOWN;
        char         aiResult[AI_RESULT_SIZE] = {};  // Buffer fixe, pas de heap
        ColorCallback colorCb   = nullptr;
        AICallback    aiCb      = nullptr;
    };

    // ---- État interne ----
    CacheEntry   m_cache[MAX_CACHE];
    uint8_t      m_cacheCount  = 0;
    uint32_t     m_cacheMaxAge = 30000;

    PendingQuery m_pending[MAX_PENDING];
    int          m_uidCounter  = 0;

    Stream&      m_stream;
    bool         m_connected   = false;
    uint32_t     m_lastStream  = 0;
    uint32_t     m_lastPing    = 0;

    // ---- Sérialisation ----
    int   sendQuery(const char* cmd);
    void  ping();

    // ---- Parsing ----
    void  processIncoming();
    void  processPending();
    void  parseResponse(int uid, const char* payload);
    void  parseBroadcast(const char* type, const char* payload);

    // ---- Cache ----
    void        updateCache(Vec2 pos, ObjectColor color);
    CacheEntry* findCacheEntry(Vec2 pos);
    CacheEntry* allocCacheEntry(Vec2 pos);

    // ---- Pending slots ----
    int           allocPending();
    PendingQuery* findPending(int uid);
};

SINGLETON_EXTERN(TwinVision, vision)
