# polymod — round 1

Gains per-voice trimmers and drift, velocity to level and cutoff, a stereo chorus in the instrument — matrix lacks 9 (the polymod's own trimmers line), 4, 10. Later rounds: filter flavours and drive (lack 20), the modulation slots (lack 11), oversampling and control rate (lacks 6, 14).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Bend | st | -12 | 12 | 0 | 0 | | |
| 2 | Glide | s | 0 | 3 | 0 | 0 | | |
| 3 | FrequencyA | st | -24 | 24 | 0 | 48 | | |
| 4 | SawA | | 0 | 1 | 1 | 1 | | |
| 5 | PulseA | | 0 | 1 | 0 | 1 | | |
| 6 | WidthA | | 0.05 | 0.5 | 0.5 | 0 | | |
| 7 | Sync | | 0 | 1 | 0 | 1 | | |
| 8 | FrequencyB | st | -24 | 24 | 0 | 48 | | |
| 9 | Fine | ct | -50 | 50 | 7 | 0 | | |
| 10 | SawB | | 0 | 1 | 1 | 1 | | |
| 11 | TriangleB | | 0 | 1 | 0 | 1 | | |
| 12 | PulseB | | 0 | 1 | 0 | 1 | | |
| 13 | WidthB | | 0.05 | 0.5 | 0.5 | 0 | | |
| 14 | Low | | 0 | 1 | 0 | 1 | | |
| 15 | Keyboard | | 0 | 1 | 1 | 1 | | |
| 16 | LevelA | | 0 | 1 | 0.7 | 0 | | |
| 17 | LevelB | | 0 | 1 | 0.5 | 0 | | |
| 18 | Noise | | 0 | 1 | 0 | 0 | | |
| 19 | Cutoff | Hz | 20 | 20000 | 1200 | 0 | | |
| 20 | Resonance | | 0 | 1 | 0.2 | 0 | | |
| 21 | Amount | oct | 0 | 5 | 2 | 0 | | |
| 22 | Tracking | | 0 | 1 | 1 | 2 | Off, Half, Full | |
| 23 | Attack1 | s | 0.001 | 10 | 0.005 | 0 | | |
| 24 | Decay1 | s | 0.001 | 10 | 0.5 | 0 | | |
| 25 | Sustain1 | | 0 | 1 | 0.3 | 0 | | |
| 26 | Release1 | s | 0.001 | 10 | 0.4 | 0 | | |
| 27 | Attack2 | s | 0.001 | 10 | 0.005 | 0 | | |
| 28 | Decay2 | s | 0.001 | 10 | 0.3 | 0 | | |
| 29 | Sustain2 | | 0 | 1 | 0.8 | 0 | | |
| 30 | Release2 | s | 0.001 | 10 | 0.4 | 0 | | |
| 31 | Contour | | 0 | 1 | 0 | 0 | | |
| 32 | Modulator | | 0 | 1 | 0 | 0 | | |
| 33 | Pitch | | 0 | 1 | 0 | 1 | | |
| 34 | Duty | | 0 | 1 | 0 | 1 | | |
| 35 | Filter | | 0 | 1 | 0 | 1 | | |
| 36 | Rate | Hz | 0.05 | 20 | 5 | 0 | | |
| 37 | Wave | | 0 | 2 | 0 | 2 | Triangle, Saw, Square | |
| 38 | Vibrato | st | 0 | 12 | 0 | 0 | | |
| 39 | Wobble | oct | 0 | 3 | 0 | 0 | | |
| 40 | Unison | | 0 | 1 | 0 | 1 | | |
| 41 | Detune | ct | 0 | 50 | 10 | 0 | | |
| 42 | Release | | 0 | 1 | 1 | 1 | | |
| 43 | Hold | | 0 | 1 | 0 | 1 | | |
| 44 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 45 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 46 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 47 | Drift | ct | 0 | 20 | 3 | 0 | | new |
| 48 | Trim | | 0 | 1 | 0.3 | 0 | | new |
| 49 | Chorus | | 0 | 1 | 0.3 | 0 | | new |

Chain: the five-voice poly-modulation chain (A and B per voice, poly-mod from the filter envelope and B into A's pitch, width and the cutoff, the four-pole, unison, release and hold). Trim scales the slot trimmers (±8 ct, ±0.25 oct, ±10 % envelope time — ours); Chorus is the wet of a fixed stereo chorus (0.5 Hz, 2.5 ms — ours).

## Chain

| Stage | Rides |
|---|---|
| allocation, five voices, oldest stolen; Unison, Detune, Glide; Hold via the core's flag | `core/voice` |
| per voice slot: trims of pitch, cutoff and envelope times, drawn once at create; a slow pitch wander, Drift cents | NEEDS core/voice — `Trim` (see dco); NEEDS core/modulator — `Drift` (see dco) |
| B (saw/triangle/pulse, Low), A (saw/pulse) synced to B, poly-mod sums | `core/oscillator` (`Sync`) |
| VCF: Cutoff × Tracking × Amount × Wobble × poly-mod × Touch·velocity × trim | `core/filter` `Ladder` (stands this round) |
| filter and loudness ADSRs, trimmed times; loudness depth = Velocity | `core/envelope` |
| the voice sum into a stereo chorus | NEEDS core/line — `Chorus` (see dco) |
| DC blocker, Volume | `core/shaper` `Blocker` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in. The out becomes a pair: left and right from the chorus, the mono sum on a single lane (the dco's `side` rule).
- voicing: `poly`. Meter: `CORE::BLOCK::Meter` over the loudest lane.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::POLYMOD`, 4 the sheet (rows appended to `polymod.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores (no chorus written in the instrument).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::POLYMOD::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a chord sounds; the chord's voices differ a little in pitch and brightness (Trim 1 plainly); Chorus widens it across left and right; hard notes louder and brighter.

## Learned

| Method | Source | Licence |
|---|---|---|
| per-voice variance drawn once per slot, scaled by global rows | 2DaT Obxd `ObxdVoice.h`; Repro-5 trimmers (manual 1.1.3) | GPL-3.0 (method only); manual |
| drift as a random walk | sst-basic-blocks `OscillatorDriftUnisonCharacter.h`; Odin 2 `DriftGenerator.cpp` | GPL-3.0 (method only) |
| bucket-brigade chorus as swept delay reads | Holters & Parker DAFx-18 | copyright (method only) |
| effects inside the poly instrument | Repro-5 (manual: five effects) | manual |

Cores: core/line, core/modulator, core/voice
