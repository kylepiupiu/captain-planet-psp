# Building and testing the PSP prototype (English)

> **Copyright and release hold:** These instructions concern technical reproducibility only. Building successfully does **not** grant a license to distribute the bundled character art or the resulting game. See [rights & licensing](../RIGHTS_AND_LICENSING.md).

## Prerequisites

- Sony PSP development toolchain / [PSPSDK](https://github.com/pspdev/pspsdk)
- `make` and a supported compiler in the toolchain
- `PSPDEV` environment variable set for your PSPSDK install; include `$PSPDEV/bin` on `PATH`
- For host-based logic tests, a C99 compiler (`gcc`) and standard development libraries

## Build

From the repository root:

```sh
make
```

The makefile generates `EBOOT.PBP` with a PSP 6.60 target configuration. The checked-in generated files `src/content.h` and `src/font.h` mean a normal game build does not require network access or a Python asset rebuild.

## Host regression test

```sh
gcc -std=c99 -O1 -g -fsanitize=address,undefined -Wno-unused-variable \
  tests/test_game.c src/game.c src/render.c -lm -o tests/test_game
ASAN_OPTIONS=detect_leaks=0 ./tests/test_game
```

This exercises all 113 catalog entries, objective reachability and stage behavior, combat primitives, pause, save/backup integrity and randomized rendering. Episode 1 has extra tests for authored terrain, required team abilities, objective order and the final evacuation gate.

**Limitations:** tests clear ordinary enemies for large campaign navigation checks; they do not demonstrate full combat balancing. A host render test cannot validate frame rate or hardware compatibility on a real PSP.

## Historical demo on PSP / PPSSPP

The October 2026 v0.1 package is a technical baseline only, and **its distribution rights have not been cleared**. Do not mirror or promote bundled game builds until copyright issues are resolved.

For a legally cleared build, package `EBOOT.PBP` and runtime assets in `PSP/GAME/PLANET/`. The game requires the entire runtime `assets` folder, not just the executable. The save is `save.dat` with backup `save.dat.bak`; startup diagnostics are written to `PLANET_BOOT.LOG`.

## Rebuilding generated data

- Update `content.json` then run `python3 tools/build_content.py` (subject to its own dataset and narrative rights).
- `tools/build_assets.py` regenerates atlases and fonts, but **does not create new character artwork**. It needs Pillow and fonts indicated in the Chinese build guide. Do not regenerate or submit art without rights clearance.

## Current evidence and acceptance gates

- [CI host regression](https://github.com/kylepiupiu/captain-planet-psp/actions): host compilation and sanitizer regression results
- [Historical v0.1 verification](BUILD_REPORT.md): original PSP toolchain build and emulator validation only
- [Episode 1 quality specification](EPISODE01_VERTICAL_SLICE.md): pending real-PSP, experience, visual and rights checks

Do not call the Episode 1 changes “real-PSP tested” unless a new build was actually loaded and evaluated on hardware.
