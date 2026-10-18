# machine — round 1

Gains the second machine's cymbals and rimshot, per-drum Decay rows, Tone rows and the kick's Attack, a pan per drum and a stereo out, and velocity through a curve — matrix lacks 89, 28, 5, 4. Later rounds: the hat loop's colour and length (its loop is drawn once at create), choke groups with a fade (19), programs (3).

## Rows

The 11 standing rows keep their indices (`place(drum, knob)` for the first five drums, Accent 10); the two new drums' Tune and Level and every new block join at the tail, a table mapping each drum to its rows.

| # | Row | Unit | Least | Most | Rest | |
|---|---|---|---|---|---|---|
| 0–9 | Kick, Snare, Hat, Clap, Tom Tune, Level | st, — | -12, 0 | 12, 1 | 0; 0.9, 0.8, 0.7, 0.8, 0.8 | |
| 10 | Accent | | 0 | 1 | 0.5 | |
| 11 | Cymbal Tune | st | -12 | 12 | 0 | new |
| 12 | Cymbal Level | | 0 | 1 | 0.6 | new |
| 13 | Rim Tune | st | -12 | 12 | 0 | new |
| 14 | Rim Level | | 0 | 1 | 0.7 | new |
| 15 | Kick Decay | s | 0.05 | 3 | 0.45 | new |
| 16 | Snare Decay | s | 0.05 | 1.5 | 0.25 | new |
| 17 | Hat Decay | s | 0.05 | 2 | 0.35 | new |
| 18 | Clap Decay | s | 0.05 | 2 | 0.35 | new |
| 19 | Tom Decay | s | 0.05 | 2 | 0.45 | new |
| 20 | Cymbal Decay | s | 0.2 | 5 | 1.5 | new |
| 21 | Rim Decay | s | 0.01 | 0.5 | 0.05 | new |
| 22 | Kick Attack | | 0 | 1 | 0.5 | new |
| 23 | Snare Tone | Hz | 1000 | 12000 | 7000 | new |
| 24 | Clap Tone | Hz | 400 | 5000 | 1300 | new |
| 25 | Tom Tone | Hz | 500 | 12000 | 5000 | new |
| 26–32 | Kick, Snare, Hat, Clap, Tom, Cymbal, Rim Pan | | -1 | 1 | 0, 0.1, -0.35, -0.1, 0.3, 0.45, 0.2 | new |
| 33 | Curve | | -1 | 1 | 0 | new |

No choice rows. Chain: the second machine (swept-sine kick with knee and click, noise-tone snare, the ring-bank hats as short synthetic loops, clap, toms) with its cymbals as longer loops of the same bank and its rimshot. Rests are today's voicings; Hat Decay is the open hat's (the closed stays 0.05 s); Tom Tone is today's constant 5 kHz. Notes: 37 rim, 49, 52, 55, 57 crash, 51, 53, 59 ride, the rest as today. Velocity as the kit's: struck through Curve, × (1 − Accent × 0.6) below 0.75.

## Chain

| Stage | Rides |
|---|---|
| note → drum by the pad table; velocity through Curve and the accent | NEEDS core/percussion — `struck(velocity, curve)` (see design/drums/clap.md) |
| kick, snare, hat, tom on the hybrid circuits; the tom's tone from its row | `core/percussion` `HYBRID` (stands; `HYBRID::Tom` gains a tone argument in `percussion.hybrid.tom.cpp`, the standing `settle` keeps 5 kHz) |
| the cymbal: a loop drawn once at create from the ring bank and noise, longer (a named 0.6 s) and darker (a band at a named 5 kHz) than the hat's, read at Tune; crash and ride as two envelopes | `core/percussion` — `HYBRID::Cymbal` written in `percussion.hybrid.cymbal.*`, the hat's loop drawing shared with it inside `HYBRID` (machine's own circuits) |
| the clap; the rim on the analogue clave's rim stroke | `core/percussion` `Clap`, `Clave` (stand) |
| each drum ticked only while sounding | NEEDS core/percussion — `sounding` (see design/drums/kit.md) |
| each drum placed by its Pan, summed per side, soft-clipped per side | NEEDS core/stereo (see design/subtractive/pads.md) |
| lanes and peak | NEEDS core/percussion — `play` for a side pair (see design/drums/kit.md) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; now stereo.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter` over the loudest side.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::MACHINE`, 4 the sheet (the `Drum` enum grows `CYMBAL`, `RIM`; `place` serves the standing five only, a row table the rest; rows to `machine.rows.hpp` if `machine.hpp` passes 100), 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in, 6 (bipolar Pan rows).
- Edits `instrument/machine/` and `core/percussion/percussion.hybrid*` only (`percussion.hybrid.hat.cpp` stands at 94 lines: the loop drawing moves to a shared hybrid unit).
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings but the row names; no switch; no magic literals (0.6 s, 5 kHz named).

## Gate

- After the library shift has landed `struck(velocity, curve)`, `sounding`, `play` for a side pair and core/stereo.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::MACHINE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a hit; a crash on 49 and a ride on 51 that are not the hat, a rim on 37; the cymbals right, the hats left; Kick Attack 1 a hard click; a velocity ramp rises smoothly.

## Learned

| Method | Source | Licence |
|---|---|---|
| the hit rendered whole into a buffer, a note only reads it | Geonkick `src/dsp/src/synthesizer.c` | GPL-3.0+ (method only) |
| ring-pair hats and a tracking band | Plaits `plaits/dsp/drums/hi_hat.h` | MIT |
| per-voice Attack, Decay, Tone, Snappy knobs and the cymbal and rim voices | Roland Cloud TR-909 (owner's manual) | commercial manual |
| velocity squared, then a curve exponent | drumkv1 `src/drumkv1.cpp` | GPL-2.0+ (method only) |

Cores: core/percussion, core/stereo
