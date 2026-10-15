# mono — round 1

Gains the saturating four-pole cascade at 2x in place of the clean ladder, velocity to level and cutoff, drift, control-rate settle — matrix lacks 39 (in part: the chain's own OTA resonance curve a later round), 6, 4, 9, 14. Later rounds: the panel's arpeggio and sequence (lack 24, `core/sequence` and TRANSPORT first).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -100 | 100 | 0 | 0 | | |
| 1 | Bend | st | -12 | 12 | 0 | 0 | | |
| 2 | Portamento | s | 0 | 3 | 0.1 | 0 | | |
| 3 | Slide | | 0 | 2 | 0 | 2 | Off, Auto, On | |
| 4 | Rate | Hz | 0.1 | 30 | 5 | 0 | | |
| 5 | Wave | | 0 | 3 | 0 | 3 | Triangle, Square, Random, Noise | |
| 6 | Vibrato | st | 0 | 12 | 0 | 0 | | |
| 7 | Range | | 0 | 3 | 1 | 3 | 16, 8, 4, 2 | |
| 8 | Width | | 0 | 1 | 0 | 0 | | |
| 9 | Source | | 0 | 2 | 1 | 2 | LFO, Manual, Envelope | |
| 10 | Pulse | | 0 | 1 | 0 | 0 | | |
| 11 | Saw | | 0 | 1 | 0.8 | 0 | | |
| 12 | Sub | | 0 | 1 | 0.4 | 0 | | |
| 13 | Down | | 0 | 2 | 0 | 2 | One, Two, Pulse | |
| 14 | Noise | | 0 | 1 | 0 | 0 | | |
| 15 | Cutoff | Hz | 20 | 20000 | 1200 | 0 | | |
| 16 | Resonance | | 0 | 1 | 0.3 | 0 | | |
| 17 | Envelope | oct | 0 | 5 | 2 | 0 | | |
| 18 | Modulation | oct | 0 | 3 | 0 | 0 | | |
| 19 | Keyboard | | 0 | 1 | 0.5 | 0 | | |
| 20 | Attack | s | 0.001 | 10 | 0.005 | 0 | | |
| 21 | Decay | s | 0.001 | 10 | 0.3 | 0 | | |
| 22 | Sustain | | 0 | 1 | 0.6 | 0 | | |
| 23 | Release | s | 0.001 | 10 | 0.2 | 0 | | |
| 24 | Trigger | | 0 | 2 | 2 | 2 | LFO, Gate, Trigger | |
| 25 | Amplifier | | 0 | 1 | 0 | 1 | Envelope, Gate | |
| 26 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 27 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 28 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 29 | Drift | ct | 0 | 20 | 3 | 0 | | new |

Chain: the little monosynth's public panel (one VCO with square, saw, sub one or two octaves down or the 25 % pulse, the LFO's four waves, envelope trigger LFO/Gate/Trigger, the amplifier's envelope or gate). Velocity, Touch and Drift are ours.

## Chain

| Stage | Rides |
|---|---|
| note: monophonic, last priority, Slide Off/Auto/On over Portamento | `core/voice` |
| VCO pulse (width by Source) + saw + sub; white noise; LFO four waves | `core/oscillator`, `core/noise`, `core/modulator` `Lfo` |
| slow pitch and cutoff wander, Drift cents deep (cutoff ±Drift/100 oct — ours) | NEEDS core/modulator — `Drift` (see dco) |
| ADSR (depth = Velocity) or gate | `core/envelope` |
| VCF: Cutoff × Keyboard × Envelope × Modulation·LFO × Touch·velocity, settled once per stride; Transistor `bass` a named constant 0, drive 1 | NEEDS core/filter — `Transistor` (see dco); NEEDS core/block — `Stride` (see acid) |
| DC blocker, Volume | `core/shaper` `Blocker` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::MONO`, 4 the sheet (rows appended to `mono.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::MONO::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a note sounds; Resonance 0.9 rings round, not glassy; a hard note is louder and brighter than a soft one; a held note wanders by a few cents.

## Learned

| Method | Source | Licence |
|---|---|---|
| nonlinear cascade: saturation per stage, 2x, half-sample feedback delay | Huovilainen DAFx-04; sst-filters `VintageLadders.h` | copyright; GPL-3.0 (method only) |
| the TPT one-pole stages with the loop solved | Zavalishin, *The Art of VA Filter Design*; stmlib `dsp/filter.h` | copyright; MIT |
| drift as a random walk | sst-basic-blocks `OscillatorDriftUnisonCharacter.h`; Odin 2 `DriftGenerator.cpp` | GPL-3.0 (method only) |
| coefficients at control rate | stmlib `dsp/parameter_interpolator.h` | MIT |

Cores: core/block, core/filter, core/modulator
