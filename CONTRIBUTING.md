# Contributing to the PSP prototype

Thanks for considering a contribution. **Our current goal is a technically solid Episode 1 vertical slice**, not simply adding more episode entries. Read [rights & licensing](RIGHTS_AND_LICENSING.md) before contributing.

## First, check rights

This repository is an unlicensed fan prototype. Contributions to original engine code, unit tests and generic environmental effects are welcome. Do not upload TV footage, audio, screenshots, unlicensed character likenesses or copyrighted artwork. The MIT grant concerns only original code that contributors can legally license; it is **not** a franchise license.

For new art/data, document where it came from, who owns it, its license, and how it was made. Do not assume AI-generated assets that resemble a protected character are automatically clear.

## How to help

1. Search issues and pick **one** scoped change, ideally from [Episode 1 criteria](docs/EPISODE01_VERTICAL_SLICE.md).
2. Fork and create a branch from `main`; submit a small PR with a clear description.
3. Explain the expected change, affected files, how to reproduce/test, and what was **not** tested.
4. Include before/after screenshots only if you have rights to share them; do not falsify PSP hardware results.

For PSP or PPSSPP bugs: report hardware model/firmware (or PPSSPP version), episode/stage, sequence of inputs, expected/actual behavior, crash log if any, and reproducible steps. Never include private save files or personal data.

## Technical guardrails

- Keep 480×272 and PSP offline compatibility; conserve memory and CPU.
- Preserve the current save format unless migration and regression tests are included.
- Prefer minimal changes to `src/game.c` and `src/render.c` over rewrites.
- Test objective reachability, collisions, save integrity, controls and FPS separately.
- Use `docs/BUILD_REPORT.md` language carefully: host tests, emulator tests and real-device tests are different evidence levels.
- First-level art polish should not conceal missing mechanics.

## Development commands

See [build instructions](docs/INSTALL_AND_BUILD.md) and the build/test snippet in the [README](README.md).

## Collaboration

We welcome issues, code reviews, test data and small pull requests. Maintainers review functionality, license provenance and scope before merging. Submitting a PR does not guarantee a merge or a public release.
