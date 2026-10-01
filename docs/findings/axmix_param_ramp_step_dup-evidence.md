# axmix_param_ramp_step_dup — evidence dossier

unit: main/game/axmix_80026EE0
verdict: blocked-evidence
attempts: 1
best_score: None
reason: no natural-C reference body; no verified runtime fact; not ready for candidate generation under current evidence

context:
FUNCTION axmix_param_ramp_step_dup  size 92 B (0x5c)  section .text
0x00  lwz r4, 0x10(r3)
0x04  lwz r0, 0xc(r3)
0x08  cmpw r4, r0
0x0C  beqlr 
0x10  lwz r0, 8(r3)
0x14  add r0, r4, r0
0x18  stw r0, 0x10(r3)
0x1C  lwz r0, 8(r3)
0x20  cmpwi r0, 0
0x24  ble 0x40
0x28  lwz r0, 0x10(r3)
0x2C  lwz r4, 0xc(r3)
0x30  cmpw r0, r4
0x34  blelr 
0x38  stw r4, 0x10(r3)
0x3C  blr 
0x40  bgelr 
0x44  lwz r0, 0x10(r3)
0x48  lwz r4, 0xc(r3)
0x4C  cmpw r0, r4
0x50  bgelr 
0x54  stw r4, 0x10(r3)
0x58  blr 

--- XREF SUMMARY ---
OBJECT  build/GFZE01/obj/game/axmix_80026EE0.o
SECTION .text  size 92 (0x5c)  value 0x1b3c  bind global
CALLERS (1): axmix_update_voice_state
CALLEES (0): 
GLOBALS (0):
RELOC HISTOGRAM: (none)

--- MINIMAL HEADER FRAGMENT (from find_xrefs --cslice) ---
(none)

--- VERIFIED RUNTIME FACTS (evidence only; static proof remains authoritative) ---
(no matching identity-verified runtime facts; rejected_bundles=0)

--- REFERENCE BODIES (Step 0) ---
(no usable natural-C bodies for this candidate)

--- M2C SEED (item 3: CFG/type hint from target object, SEED ONLY) ---
void axmix_param_ramp_step_dup(void *arg0) {
    s32 temp_r0;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 temp_r4_3;

    temp_r4 = arg0->unk10;
    if (temp_r4 != (s32) arg0->unkC) {
        arg0->unk10 = (s32) (temp_r4 + arg0->unk8);
        temp_r0 = arg0->unk8;
        if (temp_r0 > 0) {
            temp_r4_2 = arg0->unkC;
            if ((s32) arg0->unk10 > temp_r4_2) {
                arg0->unk10 = temp_r4_2;


action: record blocker and advance the unit; do not retry unchanged source.
