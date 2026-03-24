#include "mission.h"
#include "os/console.h"
#include <Arduino.h>

// ============================================================
//  Builder API
// ============================================================

FLASHMEM Mission& Mission::add(Block block) {
    if (m_count >= MAX_BLOCKS) {
        Console::error("Mission") << "Max blocs atteint (" << MAX_BLOCKS << ")" << Console::endl;
        return *this;
    }
    block.done          = false;
    block.stats         = BlockStats{};
    m_blocks[m_count++] = block;
    return *this;
}

FLASHMEM Mission& Mission::setMode(SelectMode mode) {
    m_mode = mode;
    return *this;
}

FLASHMEM Mission& Mission::setSafetyMargin(uint32_t ms) {
    m_safetyMs = ms;
    return *this;
}

FLASHMEM Mission& Mission::setTimeProvider(uint32_t (*fn)()) {
    m_timeProvider = fn;
    return *this;
}


// ============================================================
//  run() — Boucle principale
// ============================================================

FLASHMEM void Mission::run() {
    m_startMs = millis();
    m_score   = 0;

    Console::info("Mission")
        << "=== DEBUT MISSION | "
        << m_count << " blocs | "
        << "objectif " << potentialScore() << " pts | "
        << "mode " << (m_mode == SelectMode::PRIORITY ? "PRIORITY" : "SCORE")
        << " ===" << Console::endl;

    while (Block* b = selectNext()) {
        uint32_t rem   = remainingMs();
        uint32_t start = millis();

        Console::info("Mission")
            << "[" << b->name << "]"
            << " prio=" << (int)b->priority
            << " score=" << (int)b->score << "pts"
            << " ~" << (int)(b->estimatedMs / 1000) << "s"
            << " | restant " << (rem == UINT32_MAX ? 9999 : (int)(rem / 1000)) << "s"
            << Console::endl;

        // --- Execution du bloc ---
        BlockResult result  = b->action();
        uint32_t    elapsed = millis() - start;

        b->done             = true;
        b->stats.attempted  = true;
        b->stats.result     = result;
        b->stats.durationMs = elapsed;

        // --- Log du résultat ---
        if (result == BlockResult::SUCCESS) {
            m_score += b->score;
            Console::info("Mission")
                << "  [OK]  " << b->name
                << " +" << (int)b->score << "pts"
                << "  reel=" << (int)(elapsed / 1000) << "s"
                << " est=" << (int)(b->estimatedMs / 1000) << "s"
                << "  TOTAL=" << (int)m_score << "pts"
                << Console::endl;
        } else {
            Console::warn("Mission")
                << "  [FAIL] " << b->name
                << "  duree=" << (int)(elapsed / 1000) << "s"
                << Console::endl;
        }
    }

    logSummary();
}


// ============================================================
//  selectNext() — Choisit le meilleur bloc éligible
// ============================================================

FLASHMEM Block* Mission::selectNext() {
    Block* best      = nullptr;
    float  bestValue = -1.0f;

    for (uint8_t i = 0; i < m_count; i++) {
        Block& b = m_blocks[i];

        if (b.done) continue;

        if (!canFit(b)) {
            b.stats.skippedTime = true;
            continue;
        }

        if (b.feasible && !b.feasible()) continue;

        float value;
        if (m_mode == SelectMode::PRIORITY) {
            value = (float)b.priority * 1000.0f + (float)b.score;
        } else {
            float ratio = (b.estimatedMs > 0)
                ? (float)b.score * 1000.0f / (float)b.estimatedMs
                : (float)b.score * 1000.0f;
            value = ratio + (float)b.priority * 0.001f;
        }

        if (value > bestValue) {
            best      = &b;
            bestValue = value;
        }
    }

    return best;
}


// ============================================================
//  canFit() / remainingMs()
// ============================================================

FLASHMEM bool Mission::canFit(const Block& b) const {
    if (b.estimatedMs == 0) return true;
    uint32_t rem = remainingMs();
    if (rem == UINT32_MAX) return true;
    return (b.estimatedMs + m_safetyMs) <= rem;
}

FLASHMEM uint32_t Mission::remainingMs() const {
    if (!m_timeProvider) return UINT32_MAX;
    return m_timeProvider();
}


// ============================================================
//  logSummary()
// ============================================================

FLASHMEM void Mission::logSummary() const {
    for (uint8_t i = 0; i < m_count; i++) {
        const Block& b = m_blocks[i];
        if (b.done) continue;

        if (b.stats.skippedTime) {
            Console::warn("Mission")
                << "  [SKIP temps]  " << b.name
                << " (~" << (int)(b.estimatedMs / 1000) << "s"
                << " pour " << (int)b.score << "pts)"
                << Console::endl;
        } else {
            Console::warn("Mission")
                << "  [SKIP infaisable] " << b.name
                << Console::endl;
        }
    }

    Console::info("Mission")
        << "=== FIN MISSION | "
        << (int)m_score << "/" << (int)potentialScore() << " pts | "
        << (int)successCount() << " OK, "
        << (int)failedCount()  << " FAIL, "
        << (int)skippedCount() << " SKIP"
        << " | " << (int)(elapsedMs() / 1000) << "s"
        << " ===" << Console::endl;
}


// ============================================================
//  Diagnostics
// ============================================================

FLASHMEM uint16_t Mission::totalScore() const { return m_score; }

FLASHMEM uint16_t Mission::potentialScore() const {
    uint16_t total = 0;
    for (uint8_t i = 0; i < m_count; i++) total += m_blocks[i].score;
    return total;
}

FLASHMEM uint8_t Mission::totalBlocks()    const { return m_count; }

FLASHMEM uint8_t Mission::attemptedCount() const {
    uint8_t n = 0;
    for (uint8_t i = 0; i < m_count; i++)
        if (m_blocks[i].done) n++;
    return n;
}

FLASHMEM uint8_t Mission::successCount() const {
    uint8_t n = 0;
    for (uint8_t i = 0; i < m_count; i++)
        if (m_blocks[i].done && m_blocks[i].stats.result == BlockResult::SUCCESS) n++;
    return n;
}

FLASHMEM uint8_t Mission::failedCount() const {
    uint8_t n = 0;
    for (uint8_t i = 0; i < m_count; i++)
        if (m_blocks[i].done && m_blocks[i].stats.result == BlockResult::FAILED) n++;
    return n;
}

FLASHMEM uint8_t Mission::skippedCount() const {
    uint8_t n = 0;
    for (uint8_t i = 0; i < m_count; i++)
        if (!m_blocks[i].done) n++;
    return n;
}

FLASHMEM uint32_t Mission::elapsedMs() const { return millis() - m_startMs; }
