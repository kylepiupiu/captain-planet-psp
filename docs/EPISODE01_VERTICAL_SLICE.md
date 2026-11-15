# Episode 1 vertical slice: “A Hero for Earth”

**Purpose:** create one convincing, hand-authored prototype level to establish quality and contributor confidence. The goal is not 113 generic missions.

> The episode story, characters and names are unlicensed derivative subject matter. This is an internal design reference only; do not treat it as distribution clearance.

## Baseline

v0.1: shared map generator; three general objectives; simple combat; generic enemies; limited animation; no PSP hardware performance evidence.

## Gameplay target (first playable engineering pass)

1. **Coastline arrival:** clear start area and bounded, recognizable shoreline with a compact map.
2. **Stabilize valve:** first interactive objective; clear elemental requirement and completion feedback.
3. **Contain contaminated pipe:** second objective, uses different team ability; environmental hazards visibly guide the player.
4. **Disable drilling controls:** final device, team switching is meaningful, objectives have a deterministic order.
5. **Final encounter & evacuation:** short boss/guard confrontation; energy/summon tutorial; green exit.
6. **Completion card:** goals cleared, elapsed time, remaining health, clear controls, retained save behavior.

Scenes should have author-placed geometry, connected paths and environmental silhouettes; procedural textures may remain but **must not** determine critical paths. The initial engineering pass may reuse existing art until independent art is rights-cleared; a rights-safe replacement art pass is required before public promotion.

## Release acceptance

- [ ] Source update and documented exact scope of code changes
- [ ] Host compiler and regression tests, including first-level interactions and pathing
- [ ] PSP toolchain build and actual PPSSPP boot/render/input verification
- [ ] Real PSP hardware check: startup, 30/60 FPS behavior, memory, audio, save/restore, long play
- [ ] UI legibility on 480×272, controls and no unintentional soft locks
- [ ] First-level music/art/character IP review and full release rights gate

**A screenshot mockup or passing host test is not a shipped PSP game.** Track test evidence separately.
