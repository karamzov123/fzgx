# `main/game/adxt_8004B7F4`: reference blocker

- Worker: `hard2`
- Lease: live on `main/game/adxt_8004B7F4`
- Selected symbol: `fn_8004B7F4`
- Selector attempts: `0/12`
- Selector state: `blocked-evidence`
- Selector reason: no natural-C reference and no verified runtime fact

## Evidence action

The reference index was rebuilt with:

```sh
python3 tools/natc_refs.py --unit main/game/adxt_8004B7F4 --build
```

Verified index result:

```text
dolsdk2001: 1764 definitions
melee: 46278 definitions
melee-src-tmpcopy: 1894 definitions
mkdd: 1396 definitions
sms: 1207 definitions
indexed 23374 distinct symbols across 5 trees
```

A fresh exact-symbol dump was then performed for `fn_8004B7F4`. It reported:

```text
20 functions still in asm, 0 with a natural-C reference body (0%)
fn_8004B7F4 -
0 reference bodies written
```

The unit context identifies the retail function as 384 bytes with calls to
`memset` and `svm_ringbuf_read`, but provides no accepted natural-C twin and no
verified runtime fact. Therefore no candidate was generated or compiled. The
The next valid action requires a new verified runtime trace or another usable exact
reference body; guessing a candidate would violate the blocked-evidence gate.

## Fresh readiness verification

The live readiness check for `fn_8004B7F4` remains `blocked-evidence`: attempts 0/12,
best score 0.000, `reference_backed=false`, `reference_present=false`, and
`runtime_traceable=false`. No candidate or compiler attempt was made.
