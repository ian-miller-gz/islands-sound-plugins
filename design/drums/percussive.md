# percussive — round 1

Gains its re-founding on the cores (first, before any gain), per-slot Tune, Bend, Tone and Noise rows, a choke that fades, and velocity through a curve — matrix lacks 2, 28, 19, 4. Later rounds: a pan per slot and a stereo out (5), separate outputs (seam S5), programs (3).

## Rows

The 12 standing rows keep their indices, names, ranges and rests (slot × (Level, Decay), then Gain, Choke); their names become sheet literals (`Kick Level` … `Tom Decay`), no longer composed at run time. The new blocks join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | |
|---|---|---|---|---|---|---|---|
| 0–9 | Kick, Snare, Closed, Open, Tom Level; Decay | —; s | 0; 0.01 | 1; 2 | 0.9, 0.75, 0.6, 0.55, 0.7; 0.36, 0.17, 0.05, 0.38, 0.31 | 0 | |
| 10 | Gain | | 0 | 1 | 0.8 | 0 | |
| 11 | Choke | | 0 | 1 | 1 | 1 | |
| 12–16 | Kick … Tom Tune | Hz | 20 | 1000 | 42, 172, 400, 400, 84 | 0 | new |
| 17–21 | Kick … Tom Bend | | 0 | 1 | 0.21, 0.05, 0, 0, 0.17 | 0 | new |
| 22–26 | Kick … Tom Tone | Hz | 200 | 16000 | 2000, 6000, 12000, 12000, 3000 | 0 | new |
| 27–31 | Kick … Tom Noise | | 0 | 1 | 0, 0.55, 1, 1, 0 | 0 | new |
| 32 | Curve | | -1 | 1 | 0 | 0 | new |

Chain: the founding five-slot drum box (a swept sine and a coloured noise per slot, one note each: 36, 38, 42, 46, 45). Each slot becomes one `Pad` circuit: its recipe's end pitch is Tune, its start-to-end sweep is Bend (the pad's sweep reaches Tune × (1 + 2.25 × Bend); 62→42 Hz gives 0.21), its noise share is Noise (one minus the recipe's tone), its noise colour Tone. The recipes' numbers move into the rests above; the sound moves from the hand-rolled sine table to the pad circuit's. Choke On: a closed hat fades the open one over 5 ms instead of zeroing it.

## Chain

| Stage | Rides |
|---|---|
| the sheet, the rows held as `Float rows[PARAMETERS]`, the five parameter hooks | `core/table` `Surface` (stands; `params.cpp`, `params.rows.cpp`, `voice.params.cpp` leave) |
| denormal flush, event cursor, meter | `core/block` (stands; the own `publish`, `denormals`, `level[2]`/`face` leave) |
| note → slot by a key table; velocity through Curve | `core/percussion` `keyed` (stands); NEEDS core/percussion — `struck(velocity, curve)` (see design/drums/clap.md) |
| five pads, each settled from its Tune, Bend, Decay, Tone, Noise rows | `core/percussion` `Pad`, `Voicing` (stand) |
| the open slot under a choke fade | NEEDS core/percussion — `Choke` (see design/drums/cymbal.md) |
| sum × Level per slot × Gain, every lane | `core/percussion` `play` (stands) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::PERCUSSIVE`, 4 the sheet — this is the re-founding: `percussive.hpp` states `ROWS` and `SHEET`, `create` fills by `CORE::TABLE::rest`; the `SURFACE` context, `Recipe`, the `<xmmintrin.h>` and `<threads.hpp>` includes and the `snprintf` reading leave; 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Edits `instrument/percussive/` only.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings but the row names; no switch; no magic literals (the 0.5 rounding and the 24-char buffer go with `params.rows.cpp`).

## Gate

- After the library shift has landed `struck(velocity, curve)` and `Choke`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::PERCUSSIVE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a hit, each of 36, 38, 42, 46, 45 sounds; Kick Bend 1 a long drop; Snare Noise 0 a pitched tom-like snare; a 42 after a 46 closes it without a click.

## Learned

| Method | Source | Licence |
|---|---|---|
| a sine swept from a start to an end pitch, noise mixed by a share | LMMS Kicker `plugins/Kicker/KickerOsc.h` | GPL-2.0+ (method only) |
| the exclusive class: a choked voice gets a very short release | FluidSynth `src/synth/fluid_voice.c`; sfizz `src/sfizz/Voice.cpp` | LGPL-2.1+ (method only); BSD-2-Clause |
| velocity through an adjustable curve | drumkv1 `src/drumkv1.cpp` | GPL-2.0+ (method only) |

Cores: core/percussion
