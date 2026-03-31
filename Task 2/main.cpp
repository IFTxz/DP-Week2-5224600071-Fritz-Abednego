#include <iostream>
#include <string>

// ============================================================
//  Task 2 — Core Loop Skeleton
//  Card-Run Game | Design Pattern Course
//  This file demonstrates the STRUCTURAL loop of the game.
//  Full gameplay logic lives in the pretest implementation.
// ============================================================

// Forward declarations — each phase has its own responsibility
class HandSelector;
class ScoringSystem;
class ShopSystem;
class RunSession;

// ------------------------------------------------------------
// HandSelector — Phase 1: Player selects cards to play
// Invariant: must happen BEFORE scoring
// ------------------------------------------------------------
class HandSelector {
public:
    void dealCards() {
        std::cout << "[Phase 1a] Dealing 8 cards to player...\n";
        // placeholder: generate random hand
    }

    void selectHand() {
        std::cout << "[Phase 1b] Player selects up to 5 cards to play...\n";
        // placeholder: player picks cards from hand
    }
};

// ------------------------------------------------------------
// ScoringSystem — Phase 2: System evaluates the played hand
// Invariant: must happen AFTER hand selection, BEFORE state update
// ------------------------------------------------------------
class ScoringSystem {
public:
    int evaluateHand() {
        std::cout << "[Phase 2a] Detecting hand rank (Pair, Flush, etc.)...\n";
        // placeholder: detect hand ranking
        int baseScore = 100; // placeholder

        std::cout << "[Phase 2b] Applying active modifiers...\n";
        // placeholder: apply modifier chain

        std::cout << "[Phase 2c] Final score calculated: " << baseScore << "\n";
        return baseScore;
    }

    bool checkTarget(int score, int target) {
        std::cout << "[Phase 2d] Checking score " << score
                  << " against target " << target << "...\n";
        return score >= target;
    }
};

// ------------------------------------------------------------
// ShopSystem — Phase 3: Reward phase (buy modifier)
// Mutable: which modifiers appear, price, availability
// Invariant: must happen AFTER scoring, BEFORE next round
// ------------------------------------------------------------
class ShopSystem {
public:
    void openShop() {
        std::cout << "[Phase 3] Shop opened — player may buy a modifier.\n";
        // placeholder: display modifiers, accept input
    }
};

// ------------------------------------------------------------
// RunSession — Master controller that enforces loop order
// The loop order is the INVARIANT — it must never change
// ------------------------------------------------------------
class RunSession {
private:
    HandSelector handSelector;
    ScoringSystem scorer;
    ShopSystem shop;

    int currentRound = 1;
    int totalRounds  = 3;
    int lives        = 3;       // mutable: could be 1, 3, or infinite
    int targetScore  = 300;     // mutable: scales per round
    int totalScore   = 0;
    bool gameOver    = false;

    bool isGameOver() const {
        return (currentRound > totalRounds) || (lives <= 0);
    }

    // INVARIANT PHASE ORDER:
    // 1. PlayerAction  → player selects hand
    // 2. ResolveSystem → system scores the hand
    // 3. UpdateState   → apply result, open shop, advance round
    // Breaking this order would cause undefined game state.

    void playerAction() {
        std::cout << "\n--- [PLAYER ACTION] Round " << currentRound << " ---\n";
        handSelector.dealCards();
        handSelector.selectHand();
    }

    void resolveSystem() {
        std::cout << "\n--- [RESOLVE SYSTEM] ---\n";
        int score = scorer.evaluateHand();
        bool hit  = scorer.checkTarget(score, targetScore);

        totalScore += score;

        if (!hit) {
            lives--;
            std::cout << "[Result] Missed target! Lives remaining: " << lives << "\n";
        } else {
            std::cout << "[Result] Target reached! Well done.\n";
        }
    }

    void updateState() {
        std::cout << "\n--- [UPDATE STATE] ---\n";
        if (lives <= 0) {
            std::cout << "[State] No lives left. Game over.\n";
            gameOver = true;
            return;
        }

        if (currentRound < totalRounds) {
            shop.openShop();    // reward phase — mutable content
            targetScore += 100; // mutable: difficulty scaling
        }

        currentRound++;
        std::cout << "[State] Advancing to round " << currentRound << "\n";
    }

public:
    // StartGame enforces the invariant loop order
    void startGame() {
        std::cout << "========================================\n";
        std::cout << "   CARD-RUN GAME — Core Loop Skeleton   \n";
        std::cout << "========================================\n";

        while (!isGameOver()) {
            playerAction();   // Phase 1 — must be first
            resolveSystem();  // Phase 2 — must follow action
            updateState();    // Phase 3 — must be last
        }

        endGame();
    }

    void endGame() {
        std::cout << "\n========================================\n";
        std::cout << "              END RUN\n";
        std::cout << "  Total Score : " << totalScore << "\n";
        std::cout << "  Lives Left  : " << lives << "\n";
        std::cout << (lives > 0 ? "  Result: WIN!\n" : "  Result: GAME OVER\n");
        std::cout << "========================================\n";
    }
};

// ------------------------------------------------------------
// main — hanya entry point, tidak mengandung game logic
// ------------------------------------------------------------
int main() {
    RunSession session;
    session.startGame();
    return 0;
}
