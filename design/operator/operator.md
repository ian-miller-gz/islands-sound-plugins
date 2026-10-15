# operator — round 1

Gains per-operator velocity sensitivity, keyboard level and rate scaling, a fixed-frequency switch per operator, a pitch envelope, and the voice-mode, glide and hold rows — matrix lacks 4 (per-operator sensitivity), 55, 56, 13, 12, 38. Later rounds: unison and a stereo out (lacks 26, 5: core/stereo first), waveforms past the sine (18), a filter after the operators (16), effects (10), LFO sensitivity and the matrix (11), macros (8), anti-aliasing (6).

## Rows

New rows join at the tail; no standing row moves. Per-operator rows repeat for n = 1..6 at the index shown.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Algorithm | | 1 | 32 | 1 | 31 | 1 … 32 | |
| 1 | Feedback | | 0 | 7 | 0 | 7 | 0 … 7 | |
| 2+11(n−1) | Ratio*n* | | 0.5 | 32 | 1 | 0 | | |
| 3+11(n−1) | Detune*n* | | -7 | 7 | 0 | 14 | -7 … +7 | |
| 4+11(n−1) | Output*n* | | 0 | 99 | 99, 75, 0, 0, 0, 0 | 99 | | |
| 5..8+11(n−1) | Rate*n*.1 … Rate*n*.4 | | 0 | 99 | 99, 30, 20, 50 | 99 | | |
| 9..12+11(n−1) | Level*n*.1 … Level*n*.4 | | 0 | 99 | 99, 80, 60, 0 | 99 | | |
| 68 | Speed | Hz | 0.05 | 50 | 5 | 0 | | |
| 69 | Delay | s | 0 | 5 | 0 | 0 | | |
| 70 | Wave | | 0 | 5 | 0 | 5 | Triangle, Down, Up, Square, Sine, Hold | |
| 71 | Pitch | ct | 0 | 1200 | 0 | 0 | | |
| 72 | Amplitude | | 0 | 1 | 0 | 0 | | |
| 73 | Sync | | 0 | 1 | 1 | 1 | | |
| 74 | Velocity | | 0 | 1 | 0.5 | 0 | | |
| 75 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 76 | Bend | st | -12 | 12 | 0 | 0 | | |
| 77 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 78+7(n−1) | Sense*n* | | 0 | 7 | 7 | 7 | | new |
| 79+7(n−1) | Fixed*n* | | 0 | 1 | 0 | 1 | | new |
| 80+7(n−1) | Break*n* | | 21 | 120 | 60 | 99 | | new |
| 81+7(n−1) | Lower*n* | | -99 | 99 | 0 | 198 | | new |
| 82+7(n−1) | Upper*n* | | -99 | 99 | 0, -30, 0, 0, 0, 0 | 198 | | new |
| 83+7(n−1) | Curve*n* | | 0 | 1 | 0 | 1 | Linear, Exponential | new |
| 84+7(n−1) | Tracking*n* | | 0 | 7 | 2 | 7 | | new |
| 120 | Sweep | st | 0 | 48 | 12 | 0 | | new |
| 121..124 | Pace1 … Pace4 | | 0 | 99 | 99, 55, 99, 99 | 99 | | new |
| 125..128 | Height1 … Height4 | | 0 | 99 | 58, 50, 50, 50 | 99 | | new |
| 129 | Mode | | 0 | 2 | 0 | 2 | Poly, Mono, Legato | new |
| 130 | Glide | s | 0 | 3 | 0 | 0 | | new |
| 131 | Hold | | 0 | 1 | 0 | 1 | | new |

Chain: the six-operator phase-modulation chain's public panel — per operator key velocity sensitivity 0–7, oscillator mode ratio/fixed, breakpoint A-1…C8 (keys 21–120), left and right depth 0–99 with four curves per side (−lin, −exp, +exp, +lin), rate scaling 0–7; a pitch envelope of four rates and four levels 0–99 with 50 the key's own pitch; poly/mono and portamento. Ours, folded: Lower/Upper are signed depths (the curve's sign), one Curve per operator serves both sides; full depth at four octaves from Break; Tracking adds Tracking × (key − 60)/12 to each of the four rates (centred on middle C); a Fixed operator runs at Ratio × 100 Hz (50–3200 Hz) and ignores key, glide, bend, LFO and Sweep; Height 99 is +Sweep, 0 is −Sweep; the walk depth is Velocity × Sense/7 (Sense 7 everywhere keeps today's sound). Mode: Poly = sixteen voices, oldest stolen; Mono = one voice, last priority, retriggered; Legato = one voice, tied notes glide without re-striking.

## Chain

| Stage | Rides |
|---|---|
| allocation by Mode (a table: mode, legato, slide); Glide as the pitch smoother; Hold via the core's flag (polymod's pattern); TIED re-tunes only | `core/voice` (stands) |
| per voice at strike: each operator's Segments copied from its rates and levels, Tracking·(key − 60)/12 added to the rates, shaped | `core/envelope` `Segments`, `shape` (stand) |
| per voice at strike: level = Output + Lower or Upper × curve(octaves from Break), clamped 0–99, through `loudness`; walk scale `weigh(velocity, Velocity × Sense/7)` | `core/envelope` (`loudness`, `weigh` stand) |
| pitch per voice at the 16-frame retune: glided key + Tune + Bend + LFO + Sweep × the pitch walk (one LINEAR Segments for the synth, one Walk per voice, held at the third stage until the lift); Fixed operators at Ratio × 100 Hz | `core/envelope` `Segments`, `Walk`; `core/phase` |
| six operators routed by the algorithm table, feedback on the loop operator | `core/oscillator` `Operator`, `Table` (stand) |
| LFO amplitude, Volume, one sample to every lane | `core/modulator` `Lfo`, `core/block` |

No core is needed this round.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; every lane written (mono as today).
- voicing: `poly` (Mono and Legato play one voice of a polyphonic instrument). Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::OPERATOR`, 4 the sheet (the second per-operator block and the tail rows appended after Volume), 5 names, 6 honesty, 7 cores (Segments, Walk, weigh, hold ridden, never re-rolled).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines (`operator.rows.hpp` stands at 81, `voice.tone.cpp` at 80: the new block's names and rows take their own header, scaling and the pitch walk their own units), functions ≤ 25; no comments; no natural-language strings; Mode and Curve pick from tables, never a switch; no magic literals (100 Hz, four octaves, 60, 50 named).

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::OPERATOR::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a chord sounds; every onset scoops down into pitch (Sweep); a note two octaves up is darker than middle C (Upper2); Mode Legato with Glide 0.3 slides a run without re-striking; Hold 1 keeps notes sounding past their lift.

## Learned

| Method | Source | Licence |
|---|---|---|
| four-stage rate/level envelope; key adds to the rate | Dexed `source/msfa/env.cc` | Apache-2.0 (file), GPL-3.0 (repo) |
| operator gain in the log domain: output level, key level scaling (breakpoint, a curve per side), velocity × per-operator sensitivity | Dexed `source/msfa/dx7note.cc` | Apache-2.0 |
| pitch envelope: four segments, held at the third until the lift | Dexed `source/msfa/pitchenv.cc` | Apache-2.0 |
| mono hand-over and portamento | Dexed `source/msfa/dx7note.cc` | Apache-2.0 |
| ratio or absolute frequency per operator | Surge XT `FM3Oscillator.cpp`; Plaits `plaits/dsp/fm/voice.h`; STK `src/FM.cpp` | GPL-3.0 (method only); MIT; STK (MIT-like) |
| sustain pedal as a hold on lifts | mda ePiano `src/mdaEPiano.cpp`; Pianoteq (manual) | GPL-3.0+ port (method only); commercial |
