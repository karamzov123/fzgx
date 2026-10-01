# Finding 43 — M3 mode-machine scout: main.dol contains no scene/mode dispatch tables

Scout (sa-0-54aee6af) mined fzero-gx-online findings/67-71 + mode-scene-tables
against our symbols.txt. Result:

- ALL live-verified mode/scene machinery sits OUTSIDE main.dol:
  modeTable 0x803283C0, sceneTable 0x80328720, MD_GAME/SMD_* handlers
  0x801BB360-0x801F94AC, modeManager 0x801BB908, loadColiCourse 0x801CE5D4,
  raceSceneInit 0x801EAA90. These are overlay/REL addresses — consistent with
  findings/02 (62/63 online seeds out-of-dol). Reserved for M4.
- In-dol candidates checked and resolved: fn_800702E4 calls only OSPanic;
  8000B628 inside DCInvalidateRange; heap wrappers already named. No
  HIGH-confidence in-dol mode-machine names from cross-pollination this pass.

Conclusion: M3 mode-machine naming cannot advance from main.dol evidence
alone; the win lives in M4 REL work post-deadline. Redirect M3 effort to
gamehead/model/lightctrl/sound clusters (findings/39-42 studies).
