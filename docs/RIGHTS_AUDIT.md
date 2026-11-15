# Intellectual-property audit · 2026-11-16

**Result: RED — public game distribution is not rights-cleared.**

This repository is already public. The present audit is a **file-level desk review**, not proof of asset origin or permissions. No franchise license was found in the reviewed repository. Absence of a license document does not prove no permission exists elsewhere, but no permission can be assumed.

| Material | Examples | Status | Required action |
| --- | --- | --- | --- |
| Franchise names, five characters, summon character, villain references | README, `src/content.h`, `src/render.c` | **RED** | Verify rights holder and obtain derivative-work permission or independently rebrand/rewrite |
| AI-generated character likenesses / prop sprites | `assets/characters_master.png`, `assets/props_master.png`, atlases | **RED** | Provenance and resemblance review; separate approvals or independent replacement |
| Game executable with bundled assets | `downloads/CaptainPlanet_PSP_v0.1.zip` | **RED** | Hold promotion/redistribution; no assumption of authorization from noncommercial status |
| Episode list and adapted story text | `content.json`, `src/content.h`, `docs/EPISODE_MAP.md` | **AMBER/RED** | Track exact provenance, CC BY-SA attribution/share-alike where required, plus franchise adaptation rights |
| TV reference screenshots | `docs/SOURCES.md` URLs | **RED if embedded/reproduced** | Do not add stills to distributed art/screens; links alone are not asset grants |
| Original engine/gameplay/test source | `src/*.c`, `src/game.h`, `tools/*.py`, `tests/*.c` | **AMBER** | MIT scope only to independent, protectable code; scan embedded IP before standalone reuse |
| Bundled font rasters | `src/font.h`; `licenses/*` | **AMBER** | Validate OFL/DejaVu terms, attribution and any rename/modification obligations for redistribution |
| Generated preview and QA screenshots | `docs/preview.png`, `qa/*.png` | **RED/AMBER** | Screens depict potentially protected fan characters; do not assume sharing rights |
| Third-party episode metadata snapshot | `research/tvmaze.json` | **AMBER** | Audit attribution, redistribution conditions and API-specific terms |

## Scope of audit

Reviewed file tree, README, contribution guide, source provenance, build report, character/prop atlas file names, release archive and game/content integration. **Pixel-by-pixel asset examination, embedded ZIP content audit and rights-holder chain of title were not completed.**

## Publication decision

- No claim that *Captain Planet PSP* as a whole is open source / MIT.
- No “safe because free/AI/noncommercial” assertion.
- Do not promote the binary or sprite assets for public downloading until cleared.
- Public collaboration can focus first on rights-safe generic PSP engine, controls, tests, and original environment designs.
- Preferred paths: **(A)** documented franchise rights-holder permission, **or (B)** convert to a genuinely original eco-adventure IP and independently created artwork/story.
- If reducing exposure, consider suspending the binary distribution and high-risk artwork in the current public repository and addressing earlier Git history; **this requires an explicit repository-publication decision**.

## Before a new public post / release

- [ ] Identify owner(s) of relevant copyrighted franchise elements and trademarks
- [ ] Obtain written permission covering game adaptations, naming/art, source sharing and downloads, **or** replace all protected elements
- [ ] Audit all bundled binaries/sprites plus historical commits for copyrighted content
- [ ] Complete third-party data and font license notice review
- [ ] Confirm license scope and contributor provenance; complete technical smoke tests
- [ ] Document and approve release assets and outreach screenshots

References: [U.S. Copyright Office on derivatives](https://www.copyright.gov/eco/help-limitation.html), [GitHub licensing documentation](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/licensing-a-repository), [project sources](SOURCES.md).
