#pragma once
#include <stdint.h>

// ============================================================
//  Mission — Planificateur de tâches avec gestion du temps
//  et des points pour robot de compétition
//
//  Usage :
//      static BlockResult collectA() {
//          async motion.goAlign(...);
//          if (!motion.wasSuccessful()) return BlockResult::FAILED;
//          actuators.grab(...);
//          return BlockResult::SUCCESS;
//      }
//
//      Mission mission;
//      mission
//          .setMode(Mission::SelectMode::PRIORITY)
//          .setTimeProvider([]() -> uint32_t { return (uint32_t)chrono.getTimeLeft(); })
//          .setSafetyMargin(5000)
//          .add({ "collect_A", 10, 150, 8000, collectA })
//          .add({ "collect_B",  8,  80, 6000, collectB });
//      mission.run();
// ============================================================


// ------------------------------------------------------------
//  BlockResult
// ------------------------------------------------------------

enum class BlockResult : uint8_t {
    SUCCESS = 0,  // Action terminée avec succès, points comptabilisés
    FAILED  = 1,  // Action échouée (collision, obstacle, timeout...)
};


// ------------------------------------------------------------
//  BlockStats — Statistiques d'un bloc après tentative
// ------------------------------------------------------------

struct BlockStats {
    BlockResult result      = BlockResult::FAILED;
    uint32_t    durationMs  = 0;    // Durée réelle d'exécution
    bool        attempted   = false; // Bloc effectivement tenté
    bool        skippedTime = false; // Passé faute de temps
};


// ------------------------------------------------------------
//  Block — Unité d'action atomique
//
//  Champs obligatoires (dans l'ordre pour aggregate init) :
//      name, priority, score, estimatedMs, action
//  Champs optionnels :
//      feasible  (nullptr = toujours faisable)
//
//  Exemple :
//      Block b = { "collect_A", 10, 150, 8000, collectA, isZoneAFree };
//      Block b = { "collect_B",  8,  80, 6000, collectB };  // pas de check
// ------------------------------------------------------------

struct Block {
    using ActionFn   = BlockResult (*)();  // Pointeur vers la fonction d'action
    using FeasibleFn = bool        (*)();  // Check de faisabilité (nullptr = toujours vrai)

    const char* name        = "unnamed"; // Nom du bloc pour les logs
    uint8_t     priority    = 0;         // Priorité (plus grand = plus prioritaire)
    uint16_t    score       = 0;         // Points accordés si SUCCESS
    uint32_t    estimatedMs = 0;         // Durée estimée (ms). 0 = aucune contrainte temps.
    ActionFn    action      = nullptr;   // Fonction à exécuter
    FeasibleFn  feasible    = nullptr;   // nullptr = toujours faisable

    // État interne — géré par Mission, ne pas modifier manuellement
    BlockStats  stats;
    bool        done        = false;
};


// ------------------------------------------------------------
//  Mission
// ------------------------------------------------------------

class Mission {
public:
    static constexpr uint8_t  MAX_BLOCKS            = 16;
    static constexpr uint32_t DEFAULT_SAFETY_MARGIN = 3000; // ms de marge avant fin de match

    // Mode de sélection du prochain bloc
    enum class SelectMode : uint8_t {
        PRIORITY,  // Trie par priorité décroissante (ordre fixe garanti)
        SCORE,     // Maximise le ratio score/temps parmi les blocs éligibles
    };

    Mission() = default;

    // ---- Builder API (chainable) ----

    // Ajoute un bloc à la liste
    Mission& add(Block block);

    // Mode de sélection (PRIORITY par défaut)
    Mission& setMode(SelectMode mode);

    // Marge de sécurité avant fin de match : un bloc est ignoré si son
    // estimatedMs + safetyMargin > temps restant  (3000ms par défaut)
    Mission& setSafetyMargin(uint32_t ms);

    // Fournisseur de temps restant (en ms). Si non défini, aucune contrainte temps.
    // Exemple : .setTimeProvider([]() -> uint32_t { return (uint32_t)chrono.getTimeLeft(); })
    Mission& setTimeProvider(uint32_t (*fn)());

    // ---- Exécution ----

    // Lance la mission. Bloquant — retourne quand tous les blocs éligibles ont été tentés.
    void run();

    // ---- Diagnostics post-run ----

    uint16_t totalScore()     const;  // Score cumulé des blocs SUCCESS
    uint16_t potentialScore() const;  // Score max si tout réussissait
    uint8_t  totalBlocks()    const;  // Nombre de blocs déclarés
    uint8_t  attemptedCount() const;  // Blocs effectivement exécutés
    uint8_t  successCount()   const;  // Blocs terminés SUCCESS
    uint8_t  failedCount()    const;  // Blocs tentés mais FAILED
    uint8_t  skippedCount()   const;  // Blocs non tentés (temps / infaisable)
    uint32_t elapsedMs()      const;  // Temps total depuis run()

private:
    Block*   selectNext();
    bool     canFit(const Block& b) const;
    uint32_t remainingMs()          const;
    void     logSummary()           const;

    Block      m_blocks[MAX_BLOCKS];
    uint8_t    m_count      = 0;
    uint16_t   m_score      = 0;
    uint32_t   m_startMs    = 0;
    uint32_t   m_safetyMs   = DEFAULT_SAFETY_MARGIN;
    SelectMode m_mode       = SelectMode::PRIORITY;
    uint32_t (*m_timeProvider)() = nullptr;
};
