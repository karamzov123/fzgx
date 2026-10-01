# MTX-region naming study (0x8006E200-0x8007A060)

Method: capstone dump + custom Gekko paired-single decoder (opcode 4) over all 211 fns.
Finding: only ~30 fns (0x8006E250-0x8006FC00) are actual Mtx/Vec math;
the remainder are game-side state/cache managers that share this unit range.
All math fns use scalar fmul/fmadds or psq_l/psq_st paired singles; no AltiVec.

| VA | fn_ sym | proposed | conf | evidence |
|---|---|---|---|---|
| 0x8006E250 | fn_8006E250 | C_MTXMultVec | HIGH | scalar, 3x3 rot apply, no w |
| 0x8006E294 | fn_8006E294 | C_MTXMultVecSS | MED | rot-only 9 loads |
| 0x8006E2B0 | fn_8006E2B0 | PSMTXMultVecSS | HIGH | psq_l pairs + fmadds + sign branch |
| 0x8006E324 | fn_8006E324 | PSMTXRotTrig | MED | calls sin lookup 0x8006D1C4; psq col writes |
| 0x8006E398 | fn_8006E398 | PSMTXRotTrig variant (transpose store) | LOW | sin call + swapped stores |
| 0x8006E424 | fn_8006E424 | PSMTXRotTrig + PSMTXConcat group | MED | sin call; ps_mul chains x3 fns |
| 0x8006E5FC | fn_8006E5FC | C_MTXQuat-ish scalar quat/mtx | LOW | stack frame, epsilon fcmpo |
| 0x8006E7E4 | fn_8006E7E4 | PSMTXQuat prep (normalize path) | MED | fmuls/fmadds sumsq + epsilon cmp |
| 0x8006E8DC | fn_8006E8DC | vec normalize scalar (4-elem) | MED | sumsq + rsqrt helper 0x8006D0E8 |
| 0x8006E978 | fn_8006E978 | PSMTXQuat (Mtx->Quat) | MED | big fp save set; pairwise products |
| 0x8006EB4C | fn_8006EB4C | PSMTXQuat (Quat->Mtx) | HIGH | largest save set, 16 psq_st |
| 0x8006ED0C | fn_8006ED0C | MTXLookAt dist/cross prep | MED | fsubs diffs + sqrt helper 0x8006D0B4 |
| 0x8006EDBC | fn_8006EDBC | PSMTXLookAt variant | MED | extra arg r6, same diff/sqrt pattern |
| 0x8006EE70 | fn_8006EE70 | PSVECNormalize | HIGH | negate + frsqrte helper 0x8006D24C |
| 0x8006EF10 | fn_8006EF10 | PSVECScale+normalize combo | MED | same helper call pattern |
| 0x8006EFB4 | fn_8006EFB4 | PSMTXRotAxisRad-ish | MED | lha angle, sin call, axis*sign stores |
| 0x8006F038 | fn_8006F038 | PSMTXLookAt | HIGH | full diff/sqrt/cross pattern |
| 0x8006F120 | fn_8006F120 | PSMTXLookAt variant | MED | no up-out param |
| 0x8006F1F0 | fn_8006F1F0 | PSMTXReflect? | LOW | diff vector, identity fallback |
| 0x8006F394 | fn_8006F394 | C_MTXRotAxis helper | LOW | const*angle, atan2 helper |
| 0x8006F3E4 | fn_8006F3E4 | C_MTXRotAxisRad | MED | quat build then concat chain |
| 0x8006F4E0 | fn_8006F4E0 | C_MTXRotAxisRad variant (in-place) | MED | calls 0x8006E1C0 |
| 0x8006F5C4 | fn_8006F5C4 | C_MTXLookAt | MED | two col builds + cross |
| 0x8006F6A8 | fn_8006F6A8 | C_MTXLookAt in-place variant | MED | same shape |
| 0x8006F774 | fn_8006F774 | scale-const setter | LOW | 2 consts to smalls |
| 0x8006F78C | fn_8006F78C | PSQUATSlerp step A | MED | fabs vs eps, fdivs |
| 0x8006F828 | fn_8006F828 | PSQUATSlerp variant B | MED | same shape, extra const |
| 0x8006F900 | fn_8006F900 | PSQUATSlerp variant C | MED | double-fdiv path |
| 0x8006F9A4 | fn_8006F9A4 | composite mtx-from-pos/scale | LOW | calls 0x8006DBAC/0x8006E5FC/0x8006EB4C |
| 0x8006FA24 | fn_8006FA24 | PSMTXConcat | HIGH | two 12-float multiply halves |
| 0x8006FB20 | fn_8006FB20 | PSMTXConcat variant | HIGH | scratch small -0x76c0 |
| 0x80071ED4 | fn_80071ED4 | compare f1/f2 vs stored vec (dirty check) | MED | fcmpu chain on globals |
| 0x80072270 | fn_80072270 | delta/ratio compute | MED | fdivs + fabs |
| 0x8007245C | fn_8007245C | mtx34-array upload loop | MED | 36 psq ops, 26 iters |
| 0x800737E4 | fn_800737E4 | int->float convert + copy | MED | 0x4330 magic, fctiwz-style inverse |
| 0x80074A8C | fn_80074A8C | color clamp/scale f1..f4 -> u8 RGBA | MED | fcmpu vs consts + fctiwz |
| 0x80074B40 | fn_80074B40 | rgba bytes -> u32 with 0xFF check | MED | byte loads + pack |
| 0x80074BC4 | fn_80074BC4 | color scale variant | MED | same shape as 0x80074A8C |
| 0x80074C74 | fn_80074C74 | rgba zeros check variant | MED | mirror of 0x80074B40 |
| 0x80074D28 | fn_80074D28 | copy vec3 + project/dot helper | MED | calls 0x8006D668 |
| 0x800749B0 | fn_800749B0 | mtx copy + dirty flag | MED | calls 0x8006DD14 |
| 0x80076790 | fn_80076790 | matrix/param blend setup | MED | const matrix load + lfs blend |

## Not MTX (game logic) — key examples

- `0x8006FCB4` — init wrapper
- `0x80070CF8` — aligned alloc
- `0x8007174C` — assert/vsprintf
- `0x80071D30` — global struct init
- `0x80072EDC` — huge dispatch
- `0x80073E8C` — size-dispatch loader
- `0x80074D88` — render-state apply
- `0x80075240` — scene builder
- `0x80075908` — draw fn
- `0x80076CD4` — material dispatch
- `0x80078538` — DMA/prefetch mtspr
- `0x800791E8` — chunked loader
