# High-fan-in unnamed functions — rename targets (session 18)

Method: R_PPC_REL24 call-graph from build objs; fan-in = # of distinct caller fns.

| fan-in | VA | symbol | unit | strings |
|---|---|---|---|---|
| 54 | 8002A8F4 | fn_8002A8F4 | dolphin/card/CARDMount |  |
| 43 | 80035828 | fn_80035828 | dolphin/gx/GXGeometry |  |
| 36 | 80088578 | fn_80088578 | dolphin/msl/msl_80083E84 |  |
| 33 | 800474E4 | fn_800474E4 | game/criadx_80047464 |  |
| 28 | 8008023C | fn_8008023C | (not in obj set) |  |
| 28 | 8005FBDC | fn_8005FBDC | game/gamehead_8005C120 |  |
| 27 | 80072CC4 | fn_80072CC4 | game/lightctrl_8007264C |  |
| 27 | 80072C24 | fn_80072C24 | game/lightctrl_8007264C |  |
| 27 | 80059AB4 | fn_80059AB4 | game/adxt_800589BC |  |
| 26 | 80072E20 | fn_80072E20 | game/lightctrl_8007264C |  |
| 26 | 800565FC | fn_800565FC | game/adxt_80055708 |  |
| 25 | 80072D64 | fn_80072D64 | game/lightctrl_8007264C |  |
| 25 | 8006D24C | fn_8006D24C | dolphin/mtx/MTXFused |  |
| 25 | 800501F4 | fn_800501F4 | game/adxt_80050180 |  |
| 24 | 80046718 | fn_80046718 | game/criadx_800462F8 |  |
| 24 | 800284A0 | fn_800284A0 | game/axmix_80026EE0 |  |
| 23 | 80059B44 | fn_80059B44 | game/adxt_800589BC |  |
| 23 | 800095A4 | fn_800095A4 | dolphin/os/OSAllocCtx |  |
| 22 | 8008D398 | fn_8008D398 | dolphin/metrotrk/main |  |
| 22 | 80046738 | fn_80046738 | game/criadx_800462F8 |  |
| 21 | 800734A8 | fn_800734A8 | game/model_80072EDC |  |
| 19 | 80073C6C | fn_80073C6C | game/model_80072EDC |  |
| 18 | 800745A4 | fn_800745A4 | game/model_80072EDC |  |
| 18 | 80038D34 | fn_80038D34 | (not in obj set) |  |
| 18 | 80026D70 | fn_80026D70 | game/axmix_80025504 |  |
| 17 | 80072AB0 | fn_80072AB0 | game/lightctrl_8007264C |  |
| 17 | 8006D0B4 | fn_8006D0B4 | dolphin/mtx/MTXFused |  |
| 17 | 8002C4BC | fn_8002C4BC | (not in obj set) |  |
| 17 | 8000C49C | fn_8000C49C | dolphin/os/OSError |  |
| 16 | 80057728 | fn_80057728 | game/adxt_800570DC |  |
| 16 | 800576DC | fn_800576DC | game/adxt_800570DC |  |
| 16 | 8002A83C | fn_8002A83C | dolphin/card/CARDMount |  |
| 16 | 800288B4 | fn_800288B4 | (not in obj set) |  |
| 16 | 80023168 | fn_80023168 | dolphin/ax/AXVPB |  |
| 15 | 8008FC3C | fn_8008FC3C | (not in obj set) |  |
| 15 | 8008E9B4 | fn_8008E9B4 | dolphin/metrotrk/custom |  |
| 15 | 8008E770 | fn_8008E770 | dolphin/metrotrk/custom |  |
| 15 | 8006DB30 | fn_8006DB30 | (not in obj set) |  |
| 15 | 8006DAEC | fn_8006DAEC | (not in obj set) |  |
| 15 | 8006D668 | fn_8006D668 | (not in obj set) |  |
| 15 | 8005A648 | fn_8005A648 | game/adxt_8005A24C |  |
| 15 | 8005A628 | fn_8005A628 | game/adxt_8005A24C |  |
| 15 | 800589BC | fn_800589BC | (not in obj set) |  |
| 14 | 80064A30 | fn_80064A30 | game/gamehead_8005C120 |  |
| 14 | 800643F4 | fn_800643F4 | game/gamehead_8005C120 |  |
| 13 | 800891B4 | fn_800891B4 | dolphin/metrotrk/trk_80088B00 |  |
| 13 | 80083D6C | fn_80083D6C | (not in obj set) |  |
| 13 | 800736C0 | fn_800736C0 | game/model_80072EDC |  |
| 13 | 8005A5BC | fn_8005A5BC | game/adxt_8005A24C |  |
| 13 | 800595FC | fn_800595FC | game/adxt_800589BC |  |
| 13 | 8004EDC4 | fn_8004EDC4 | game/adxt_8004E098 |  |
| 13 | 800175C0 | fn_800175C0 | dolphin/dvd/dvdfs |  |
| 12 | 8008AFF0 | fn_8008AFF0 | (not in obj set) |  |
| 12 | 800747D0 | fn_800747D0 | game/model_80072EDC |  |
| 12 | 80064D4C | fn_80064D4C | game/gamehead_8005C120 |  |
| 12 | 80053A30 | fn_80053A30 | (not in obj set) |  |
| 12 | 8003D588 | fn_8003D588 | dolphin/gx/GXFog |  |
| 12 | 8003D42C | fn_8003D42C | (not in obj set) |  |
| 11 | 80088EB0 | fn_80088EB0 | (not in obj set) |  |
| 11 | 80083DB0 | fn_80083DB0 | (not in obj set) |  |
| 11 | 8006D758 | fn_8006D758 | (not in obj set) |  |
| 11 | 8004E270 | fn_8004E270 | (not in obj set) |  |
| 11 | 80028424 | fn_80028424 | game/axmix_80026EE0 |  |
| 10 | 80083B8C | fn_80083B8C | (not in obj set) |  |
| 10 | 800735C8 | fn_800735C8 | game/model_80072EDC |  |
| 10 | 800728A8 | fn_800728A8 | game/lightctrl_8007264C |  |
| 10 | 80060C54 | fn_80060C54 | game/gamehead_8005C120 |  |
| 10 | 80012678 | fn_80012678 | dolphin/os/SIBios |  |
| 10 | 8000B8AC | fn_8000B8AC | (not in obj set) |  |
| 10 | 80006BDC | fn_80006BDC | (not in obj set) |  |
| 9 | 8008069C | fn_8008069C | dolphin/msl/stdio_tail_8008067C |  |
| 9 | 80060BDC | fn_80060BDC | game/gamehead_8005C120 |  |
| 8 | 8008A754 | fn_8008A754 | dolphin/metrotrk/trk_80088B00 |  |
| 8 | 80088598 | fn_80088598 | dolphin/msl/msl_80083E84 |  |
| 8 | 80074788 | fn_80074788 | game/model_80072EDC |  |
| 8 | 80074660 | fn_80074660 | game/model_80072EDC |  |
| 8 | 80073678 | fn_80073678 | game/model_80072EDC |  |
| 8 | 8007245C | fn_8007245C | game/fn_80071CC0 |  |
| 8 | 80041684 | fn_80041684 | game/criadx_80041460 |  |
| 8 | 800371F8 | fn_800371F8 | dolphin/gx/GXTexture |  |
