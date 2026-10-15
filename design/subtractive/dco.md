# dco — round 1

Gains voice modes and glide, velocity to level and cutoff, per-voice drift and trims, the saturating four-pole at 2x settled at control rate, and its chorus moved onto the library — matrix lacks 12, 4, 9, 39 (in part: a per-stage saturating cascade; the chain's own OTA voicing a later round), 6, 14.

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Bend | st | -12 | 12 | 0 | 0 | | |
| 2 | Range | | 0 | 2 | 1 | 2 | 16, 8, 4 | |
| 3 | Vibrato | st | 0 | 12 | 0 | 0 | | |
| 4 | Pulse | | 0 | 1 | 0 | 1 | | |
| 5 | Saw | | 0 | 1 | 1 | 1 | | |
| 6 | Width | | 0 | 1 | 0 | 0 | | |
| 7 | Source | | 0 | 2 | 0 | 2 | LFO, Manual, Envelope | |
| 8 | Sub | | 0 | 1 | 0.3 | 0 | | |
| 9 | Noise | | 0 | 1 | 0 | 0 | | |
| 10 | Highpass | | 0 | 3 | 1 | 3 | 0, 1, 2, 3 | |
| 11 | Cutoff | Hz | 20 | 20000 | 1500 | 0 | | |
| 12 | Resonance | | 0 | 1 | 0.2 | 0 | | |
| 13 | Envelope | oct | 0 | 5 | 2 | 0 | | |
| 14 | Polarity | | 0 | 1 | 0 | 1 | Positive, Negative | |
| 15 | Wobble | oct | 0 | 3 | 0 | 0 | | |
| 16 | Keyboard | | 0 | 1 | 0.5 | 0 | | |
| 17 | Amplifier | | 0 | 1 | 0 | 1 | Envelope, Gate | |
| 18 | Attack | s | 0.001 | 10 | 0.005 | 0 | | |
| 19 | Decay | s | 0.001 | 10 | 0.5 | 0 | | |
| 20 | Sustain | | 0 | 1 | 0.6 | 0 | | |
| 21 | Release | s | 0.001 | 10 | 0.4 | 0 | | |
| 22 | Rate | Hz | 0.1 | 20 | 4 | 0 | | |
| 23 | Delay | s | 0 | 3 | 0 | 0 | | |
| 24 | Chorus | | 0 | 3 | 1 | 3 | Off, I, II, I+II | |
| 25 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 26 | Assign | | 0 | 2 | 0 | 2 | Poly, Mono, Unison | new |
| 27 | Glide | s | 0 | 3 | 0 | 0 | | new |
| 28 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 29 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 30 | Drift | ct | 0 | 20 | 3 | 0 | | new |
| 31 | Trim | | 0 | 1 | 0.3 | 0 | | new |

Chain: the six-voice DCO's public panel (Range 16'/8'/4', the four-position high-pass, one ADSR with a gate mode, chorus I/II/I+II); new rows' ranges are ours. Assign: Poly = six voices, oldest stolen; Mono = one voice, last priority, legato; Unison = all six on one note, their spread from Trim and Drift alone (the chain's unison has no detune knob). Velocity is the amplifier's velocity depth; Touch the cutoff octaves at full velocity.

## Chain

| Stage | Rides |
|---|---|
| allocation by Assign; Glide as the pitch smoother | `core/voice` (`mode`, `unison`, `glide` stand) |
| per voice slot: pitch, cutoff and envelope-time trims drawn once at create, scaled by Trim (±8 ct, ±0.25 oct, ±10 % — ours) | NEEDS core/voice — `Trim`: per-slot fixed offsets `pitch`, `cutoff`, `time`, each −1..1, `draw(trims, count, seed)` once at create |
| per voice: slow pitch wander, Drift cents deep | NEEDS core/modulator — `Drift`: a seeded slow random walk in −1..1 (white noise through a leaky one-pole), `settle(drift, seconds, rate)`, `tick(drift)` |
| DCO saw + pulse (width by Source) + sub square; white noise | `core/oscillator`, `core/noise` |
| high-pass, four positions | `core/filter` `Biquad` |
| per voice VCF: Cutoff × Keyboard × Envelope (Polarity) × Wobble × Touch·velocity × trim, settled once per stride | NEEDS core/filter — `Transistor`: the four-pole low-pass with a soft saturation in each stage and the feedback, run at 2x inside `core/shaper` `Oversampler`, the feedback half a sample late so tuning holds, `bass` 0..1 restoring the passband loss; `settle(transistor, cutoff, emphasis, drive, bass, rate)`, `tick`; emphasis 1 rings. NEEDS core/block — `Stride` (see acid) |
| VCA: ADSR or gate, depth = Velocity | `core/envelope` (`depth`, `weigh` stand) |
| LFO triangle with delay | `core/modulator` `Lfo` |
| chorus: dco keeps its I/II/I+II table (rates, depths, wet) and rides the lifted core | NEEDS core/line — `Chorus`: three swept reads of one line under three phase-offset triangle LFOs, lifted from `instrument/dco/voice.chorus.cpp`; `build(chorus, rate)`, `settle(chorus, hertz, depth, wet, rate)`, `tick(chorus, in)` → left, right, whole |

`voice.chorus.cpp` leaves dco once `core/line` `Chorus` stands. Transistor `bass` is a named constant here (0: the chain shows no bass loss to restore).

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; left, right and mono sum written as today.
- voicing: `poly` (Assign Mono plays one voice of a polyphonic instrument). Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::DCO`, 4 the sheet (rows appended to `dco.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores (the chorus leaves the instrument for `core/line`; Trim, Drift, Stride, Transistor from core/).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Assign picks from a table, never a switch; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::DCO::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a chord sounds; Assign Unison thickens one key into six slightly unequal voices; soft and hard notes differ in level and brightness; Resonance 0.9 rings without hiss.

## Learned

| Method | Source | Licence |
|---|---|---|
| nonlinear ladder: saturation per stage, 2x, half-sample feedback delay | Huovilainen DAFx-04; VCV Fundamental `src/VCF.cpp`; sst-filters `VintageLadders.h` | copyright; GPL-3.0; GPL-3.0 (method only) |
| per-voice variance drawn once per slot, scaled by global rows | 2DaT Obxd `Source/Engine/ObxdVoice.h`; Repro trimmers (manual) | GPL-3.0 (method only); manual |
| drift as a random walk: noise through a leaky one-pole per voice | sst-basic-blocks `OscillatorDriftUnisonCharacter.h`; Odin 2 `DriftGenerator.cpp` | GPL-3.0 (method only) |
| mono modes and portamento | Surge `SurgeVoice.cpp`; Yarns `part.h` | GPL-3.0 (method only); MIT |
| coefficients at control rate | stmlib `dsp/parameter_interpolator.h` | MIT |
| bucket-brigade chorus as swept delay reads | Holters & Parker DAFx-18 | copyright (method only) |

Cores: core/block, core/filter, core/line, core/modulator, core/voice
