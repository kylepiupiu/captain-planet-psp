# Rights, licensing and permitted contributions

**Status: rights review in progress; the fan game is not licensed by the TV rights holders.** This document allocates the project's own licensing claims only. It is **not** a permission from external rights holders or a legal clearance opinion.

## 1. MIT scope — original source code only

The [MIT LICENSE](LICENSE) is intended solely for independently authored, copyrightable portions of:

- `src/main.c`, `src/game.c`, `src/game.h`, `src/render.c`, `src/render.h`, `src/audio.c`, `src/audio.h`
- `tests/test_game.c`, `qa/check_emulator.py`
- Original portions of `tools/*.py` and `Makefile`
- Original technical documentation and generic, non-story-specific UI logic contributed under this scope

This grant extends **only to rights the contributors are legally able to license**, and does not imply that AI-only material necessarily has copyright protection. An implementation that embeds a third party's protected material may still require separate permissions to redistribute or use as a complete game.

## 2. Not licensed under MIT

- The names, likenesses, trademarks, designs, characters, stories and other protected elements of *Captain Planet and the Planeteers*, and adaptations of them.
- `assets/**`, `ICON0.PNG`, `docs/preview.png`, `qa/*.png`, and any generated character/prop atlases (`*.png`, `*.rgba`). Provenance is documented but clearance is **not established**.
- `content.json`, `src/content.h`, `docs/EPISODE_MAP.md`, `research/tvmaze.json`: contain or derive from episode metadata, adapted summaries, character or story content. They are subject to source terms and underlying rights.
- `src/font.h`, font raster data and material under `licenses/**`: underlying typefaces have their **own** licenses.
- `downloads/**` and any bundled executable: combined artifacts include separately controlled story/art content and are **not** automatically redistributable under MIT.
- External libraries, artwork, audio or data introduced by future contributors unless specifically cleared and labelled.

**Do not describe the whole repository or game as “MIT licensed.”** Describe the **original code** as MIT licensed, and the **game IP and assets** as separately restricted/unverified.

## 3. Source-specific notices

- Font provenance and license texts: `licenses/FONT_NOTICE.txt`, `licenses/FONT_LICENSE.txt`, `licenses/DEJAVU_LICENSE.txt`.
- Public episode sources and third-party dataset terms: `docs/SOURCES.md`.
- Creative Commons rights, where applicable, apply only to the particular source material or allowable adaptation, **not** to show characters/IP belonging to other rights holders.
- The appearance of an internet image in a reference list is not permission to copy or make and distribute a derivative.

## 4. Contributor terms

Only submit code/assets you created or are authorized to share. State source, authorship, license, AI assistance if relevant, and any attribution obligations. By contributing original code, you agree to offer the parts you can lawfully license under the code MIT terms. Do not submit scraped TV footage, show soundtrack, promotional stills, other games' resources, or character-accurate fan assets without an approved rights basis.

A pull request does not transfer ownership of unrelated intellectual property. Before any public binary release, verify *every* bundled dependency and asset separately.

## 5. Rights-holder contact and removal

If you are a rights holder or representative with concerns, open a GitHub issue with the minimum needed detail so the repository maintainer can respond. Do not publish private contact details. On a credible notice, stop further distribution of the disputed material and follow applicable platform/copyright procedures.

## 6. Publication gate

**Not yet satisfied:** confirm rights ownership and secure permission for franchise adaptations **or** replace copyrighted story, character names/designs, artwork and branding with independently created elements. Recheck code, documentation, embedded screenshots, binary packages and repository history before claiming a clearance. Deleting a file from a later commit does not delete earlier public Git history or existing copies.

This is a practical engineering checklist, not legal advice. Get jurisdiction-specific professional advice before public promotion or mass distribution.
