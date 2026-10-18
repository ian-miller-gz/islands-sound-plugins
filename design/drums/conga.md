# conga — round 1

Gains a mute stroke by note, the bend's time and shape, a Tone row over the slap's crack, two upper body modes, a velocity curve, and chromatic play — matrix lacks 21, 13, 22, 27, 4, 23. Later rounds: a membrane mesh (sc3 `Membrane`, high CPU), stereo (5), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | Hz | 100 | 400 | 190 | 0 | | |
| 1 | Decay | s | 0.05 | 1.5 | 0.3 | 0 | | |
| 2 | Bend | | 0 | 1 | 0.2 | 0 | | |
| 3 | Level | | 0 | 1 | 0.8 | 0 | | |
| 4 | Drop | s | 0.005 | 0.3 | 0.03 | 0 | | new |
| 5 | Slope | | -1 | 1 | 0 | 0 | | new |
| 6 | Tone | Hz | 500 | 6000 | 1500 | 0 | | new |
| 7 | Ring | | 0 | 1 | 0.25 | 0 | | new |
| 8 | Curve | | -1 | 1 | 0 | 0 | | new |
| 9 | Key | | 0 | 1 | 0 | 1 | Off, On | new |

Chain: the swept bridged-T conga with the hand damp (slap above velocity 0.75 as today). Drop is the bend's time (today's constant 30 ms), Slope its shape; Tone is the slap noise's high-pass (today's 1500 Hz); Ring is the share of two upper membrane modes at the ideal membrane's ratios (1.59 and 2.14 — Bessel zeros, physics, not a tree's table), each decaying faster by its ratio. The note map follows the general MIDI map: 62 high muted (the damp at any velocity), 63 high open, 64 low open; any other note mid open. Key On plays every note open at Tune × 2^((note − 60)/12).

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve | NEEDS core/percussion — `struck(velocity, curve)` (see design/drums/clap.md) |
| note → height and stroke by a key table (muted a column of it); Key → a keyed base | `core/percussion` `keyed` (stands; the table gains the stroke) |
| the body sweep: depth Bend × 0.5, time Drop, shape Slope | NEEDS core/percussion — `Sweep` slope: `settle(sweep, decay, seconds, depth, slope, rate)`; the pitch offset is depth × drop^(4^slope) (0 today's exponential fall, +1 a snap, −1 a lingering bend); the standing five-argument `settle` is slope 0. Riders: conga, tom, kick, snare |
| two upper modes beside the body, damped with it on a mute | `core/percussion` `Resonator` (stands); `Conga` gains them in `percussion.conga.*`, the standing `settle`/`strike` keep today's sound for kit |
| slap noise through the high-pass at Tone | `core/filter` `Pole` HIGH (stands) |
| soft clip, Level, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::CONGA`, 4 the sheet, 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/conga/` and `core/percussion/percussion.conga.*` only.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (60, the two mode ratios named).

## Gate

- After the library shift has landed `struck(velocity, curve)` and the `Sweep` slope.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::CONGA::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note; 62 a short muted tone, 63 the same pitch ringing, 64 lower; a longer Drop audibly bends; Ring 1 hollow; Key On plays a tune.

## Learned

| Method | Source | Licence |
|---|---|---|
| a mode bank whose higher modes decay sooner; the djembe as struck modes | Faust `physmodels.lib` (`djembeModel`), `synths.lib` (`additiveDrum`) | LGPL-2.1+ with exception (method only) |
| partials stretched, decay by a power law of frequency | RipplerX `src/dsp/Partial.cpp` | GPL-3.0 (method only) |
| a pitch envelope with a cubic bend over a decay time | Befaco Kickall `src/Kickall.cpp` | GPL-3.0+ (method only) |
| mute and open strokes by note | Roland Cloud TR-808 conga switch; Battery 4 articulations | commercial manuals |

Cores: core/percussion
