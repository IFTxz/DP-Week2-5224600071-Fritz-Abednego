# Card-Run Game — Task 2: Core Loop Analysis
**Course: Design Pattern**

---

## Step 1 — Core Loop

The core loop of the Card-Run Game runs as follows:

1. Player is dealt 8 cards and selects up to 5 cards to play as a hand
2. System detects the hand rank (e.g. Pair, Flush, Straight) and calculates base score
3. Active modifiers are applied in chain order to produce the final score
4. Final score is compared against the round target
5. If target is met → round cleared; if missed → player loses a life
6. Shop opens: player may purchase a modifier to strengthen future rounds
7. Target score increases for next round (difficulty scaling)
8. Repeat until all rounds are completed or lives run out

---

## Step 2 — Invariants

### Which steps must NEVER change order?
The following sequence is invariant and must always occur in this exact order:

> **PlayerAction → ResolveSystem → UpdateState**

Specifically:
- **Card dealing** must happen before **hand selection**
- **Hand selection** must happen before **scoring**
- **Scoring** must happen before **target check**
- **Target check** must happen before **state update and shop**

### Which components must always exist?
| Component | Reason |
|---|---|
| `RunSession` | Controls the loop; without it there is no game flow |
| `HandSelector` | Without card selection there is no player input |
| `ScoringSystem` | Without scoring, win/lose condition cannot be evaluated |
| `IModifier` interface | Modifier chain depends on this contract |

### What breaks if the order changes?
- If **shop opens before scoring**: player buys modifier that affects a score already calculated — modifier has no effect
- If **scoring happens before card selection**: system scores an empty or undefined hand
- If **state update happens before target check**: lives and round counter advance before result is known, making the game unwinnable or logic broken

---

## Step 3 — Mutable Elements

| # | Mutable Element | Why Mutable |
|---|---|---|
| 1 | **Target score per round** | Scales with difficulty; can be adjusted without changing loop structure |
| 2 | **Active modifiers** | Player chooses modifiers in shop; content changes every run |
| 3 | **Number of lives** | Game mode can set this to 1 (hardcore) or unlimited (casual) |
| 4 | **Hand rank bonus values** | Score multipliers for Pair, Flush, etc. can be tuned for balance |
| 5 | **Shop modifier pool** | Which modifiers appear in shop is randomized and expandable |
| 6 | **Cards dealt per round** | Could be 5, 7, or 8 cards without breaking the loop |

These are mutable because they are **parameters of the loop, not the loop itself**. Changing them affects difficulty, balance, or content — but the phase sequence remains intact.

---

## Step 4 — C++ Core Loop Skeleton

See `Task2/main.cpp`.

The skeleton demonstrates:
- `RunSession::startGame()` enforces the invariant phase order via `while (!isGameOver())`
- Three clearly separated phases: `playerAction()`, `resolveSystem()`, `updateState()`
- Each phase is delegated to its own class (`HandSelector`, `ScoringSystem`, `ShopSystem`)
- `main()` only creates a `RunSession` — no game logic lives there

---

## Reflection (300–400 words)

### What is the invariant structure of your game?

The invariant structure of the Card-Run Game is the three-phase loop: **PlayerAction → ResolveSystem → UpdateState**. This sequence must always occur in this exact order every round. Within PlayerAction, cards must be dealt before the player can select them. Within ResolveSystem, the hand must be evaluated before checking it against the target. These are not just conventions — they represent causal dependencies. The output of one phase is the required input of the next. No valid game state can exist if these dependencies are violated.

The other structural invariant is the existence of core classes. `RunSession` must always exist to own the loop. `ScoringSystem` must always exist because without evaluation there is no win or lose condition. `IModifier` must always exist as a contract, because the entire modifier chain depends on polymorphism through that interface. Removing any of these collapses the architecture.

### What parts are mutable?

Everything that is a parameter rather than a structure is mutable. The target score, the number of lives, the modifier pool in the shop, the score bonuses per hand rank, and the number of cards dealt are all mutable. These can change between game modes, difficulty settings, or future updates. A hardcore mode might reduce lives to one. A balanced mode might cap the SquareBoost modifier. A new update might add a new hand rank. None of these changes touch the loop order — they only change the values flowing through it.

### If you wanted to add a new feature, which class would change?

Adding a new modifier (like a modifier that multiplies score based on remaining lives) only requires creating a new class implementing `IModifier` and registering it in `ModifierFactory`. The loop, `RunSession`, and `ScoringSystem` do not change. Adding a new hand rank bonus only changes `ScoringSystem::evaluateHand()`. Adding a discard mechanic adds a phase inside `playerAction()` without affecting `resolveSystem()` or `updateState()`.

### If you changed the loop order, what would break?

If `updateState()` ran before `resolveSystem()`, the round counter and lives would update before the score is checked — making it impossible to correctly respond to a missed target. If `resolveSystem()` ran before `playerAction()`, the scoring system would operate on an undefined hand. If the shop opened before scoring, modifiers purchased that round would not affect the current round's score, breaking player expectation and game logic simultaneously. The loop order is the skeleton of the game — bending it breaks the game.



