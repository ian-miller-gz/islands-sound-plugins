# hat — round 1

Gains a pedal stroke by note, a noise share in the metal, a ring-modulated voicing of the bank, a fade where the closed hat chokes the open one, and velocity through a curve to the band — matrix lacks 21, 44, 83 (the ring pairs), 19, 4. Later rounds: half-open by velocity, a two-stage decay and a swing VCA (83's rest), a stereo metal spread (5), band-limited squares (core/oscillator `Edges`), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | st | -12 | 12 | 0 | 0 | | |
| 1 | Closed | s | 0.01 | 0.5 | 0.06 | 0 | | |
| 2 | Open | s | 0.05 | 2 | 0.45 | 0 | | |
| 3 | Tone | Hz | 2000 | 14000 | 6000 | 0 | | |
| 4 | Level | | 0 | 1 | 0.8 | 0 | | |
| 5 | Pedal | s | 0.01 | 0.2 | 0.04 | 0 | | new |
| 6 | Noise | | 0 | 1 | 0.15 | 0 | | new |
| 7 | Metal | | 0 | 1 | 0 | 1 | Squares, Rings | new |
| 8 | Curve | | -1 | 1 | 0 | 0 | | new |
| 9 | Touch | oct | 0 | 3 | 1 | 0 | | new |

Chain: the analogue hat (six squares at the metallic ratios into a band and a high-pass). Notes follow the general MIDI map: 42 closed, 44 pedal, 46 open; any other note closed (today 44 was closed). The pedal stroke is the closed voicing at Pedal seconds and a named half level; Noise is white noise's share before the band; Metal Rings multiplies the bank in three pairs instead of summing six; a closed or pedal stroke fades a ringing open hat over 5 ms instead of cutting it; Touch moves the band (today's constant 7100 Hz) as 7100 × 2^(Touch × (v − 1)).

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve; the band by Touch | NEEDS core/percussion — `struck(velocity, curve)`, `touched` (see design/drums/clap.md) |
| note → stroke (CLOSED, PEDAL, OPEN) | the instrument's key table over `core/percussion` `keyed` (stands) |
| six squares summed, or three ring pairs (`Metal` Rings), + white × Noise | `core/percussion` `Metal`, `core/noise` `White` (stand); the pair product as a public `ring(metal)` beside `tick(metal)`, written in `percussion.hat.*` (machine's loop keeps its file-local twin this round; the twin is noted for the refactor) |
| band at the touched centre, high-pass at Tone | `core/filter` `Biquad` (stands) |
| the open stroke on its own gate, faded by a closed or pedal stroke | NEEDS core/percussion — `Choke` (see design/drums/cymbal.md) |
| soft clip, Level, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

`PEDAL` joins `OPENING` after `OPEN` in `percussion.hat.hpp`; `HYBRID::Hat`'s envelope array grows by one unused envelope (machine untouched). The standing `settle(hat, tune, closed, open, tone, rate)` and `strike(hat, opening, velocity)` keep today's sound for kit and rhythm, except the 5 ms fade.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::HAT`, 4 the sheet (`HAT::PITCH` gains `CLOSED` and `PEDAL` beside `OPEN`), 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/hat/` and `core/percussion/percussion.hat.*` only.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves (the stroke by a key table, not the `?:` on 46); no magic literals (7100 Hz, half level, 5 ms named).

## Gate

- After the library shift has landed `struck(velocity, curve)`, `touched` and `Choke`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::HAT::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note; 42 a tick, 44 a softer chick, 46 a ringing open hat that a 42 closes without a click; Metal Rings rougher and more clangorous; Noise 1 a hiss; soft hits darker.

## Learned

| Method | Source | Licence |
|---|---|---|
| six squares or three square-times-square ring pairs; noise crossfaded in; a band that follows tone | Plaits `plaits/dsp/drums/hi_hat.h` | MIT |
| a high-Q band run twice per sample for stability | Peaks `peaks/drums/high_hat.cc` | MIT |
| the open hat choked with a very short release | FluidSynth `src/synth/fluid_voice.c` (exclusive class); sfizz `src/sfizz/Voice.cpp` | LGPL-2.1+ (method only); BSD-2-Clause |
| pedal and half-open strokes | Addictive Drums 2, Superior Drummer 3 (stroke types) | commercial |

Cores: core/percussion
