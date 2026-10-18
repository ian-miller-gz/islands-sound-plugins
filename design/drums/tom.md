# tom — round 1

Gains the bend's time and shape as rows, two upper body modes decaying faster, velocity through a curve to the stick's tone, and chromatic play — matrix lacks 13, 27, 4, 23. Later rounds: mute and flam strokes (21), a membrane mesh (sc3 `Membrane`, high CPU), stereo (5), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | Hz | 40 | 250 | 90 | 0 | | |
| 1 | Decay | s | 0.05 | 2 | 0.4 | 0 | | |
| 2 | Bend | | 0 | 1 | 0.3 | 0 | | |
| 3 | Tone | Hz | 500 | 12000 | 4000 | 0 | | |
| 4 | Level | | 0 | 1 | 0.8 | 0 | | |
| 5 | Drop | s | 0.01 | 1 | 0.2 | 0 | | new |
| 6 | Slope | | -1 | 1 | 0 | 0 | | new |
| 7 | Ring | | 0 | 1 | 0.25 | 0 | | new |
| 8 | Curve | | -1 | 1 | 0 | 0 | | new |
| 9 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 10 | Key | | 0 | 1 | 0 | 1 | Off, On | new |

Chain: the analogue tom (a swept resonator with a noise stick, three heights by note). Drop is the bend's time (today half of Decay, so 0.2 s at rest), Slope its shape; Ring is the share of two upper membrane modes at the ideal membrane's ratios (1.59 and 2.14 — physics), each decaying faster by its ratio and swept with the body; Touch sets the stick's low-pass at Tone × 2^(Touch × (v − 1)); Key On plays every note at the mid height, Tune × 2^((note − 60)/12).

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve; the stick by Touch | NEEDS core/percussion — `struck(velocity, curve)`, `touched` (see design/drums/clap.md) |
| note → height by the key table (41–50 as today), or Key → a keyed base | `core/percussion` `keyed`, `core/phase` `ratio` (stand) |
| body and modes swept: depth Bend, time Drop, shape Slope | NEEDS core/percussion — `Sweep` slope (see design/drums/conga.md); `Tom` gains the modes in `percussion.tom.*` |
| the stick: noise through the low-pass | `core/noise` `White`, `core/filter` `Pole` (stand) |
| soft clip, Level, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

The standing `settle(tom, tune, decay, bend, tone, rate)` stays with Drop = Decay × 0.5, Slope 0, Ring 0 — kit sounds as today.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::TOM`, 4 the sheet, 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/tom/` and `core/percussion/percussion.tom.*` only.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (60, the mode ratios named).

## Gate

- After the library shift has landed `struck(velocity, curve)`, `touched` and the `Sweep` slope.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::TOM::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note; 41, 45, 48 three heights; Drop 0.02 a short thud, Drop 1 a long dive; Ring 1 a hollow shell; soft hits darker; Key On plays the keys.

## Learned

| Method | Source | Licence |
|---|---|---|
| struck modes, the higher decaying sooner | Faust `synths.lib` (`additiveDrum`), `physmodels.lib` | LGPL-2.1+ with exception (method only) |
| partials stretched, decay by a power law of frequency | RipplerX `src/dsp/Partial.cpp` | GPL-3.0 (method only) |
| a pitch envelope with a cubic bend over a time | Befaco Kickall `src/Kickall.cpp` | GPL-3.0+ (method only) |
| chromatic toms by keytrack | KICK 3 keytrack; Plaits V/oct | commercial; MIT |

Cores: core/percussion
