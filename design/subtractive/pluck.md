# pluck — round 1

Gains stiffness (allpass dispersion in the loop), an exciter choice, a stereo image by key, a Bend row — matrix lacks 50, 51, 5, 17 (in part). Later rounds: vibrato and glide on the string (lack 17's rest).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Tone | Hz | 200 | 20000 | 5000 | 0 | | |
| 2 | Position | | 0.02 | 0.5 | 0.13 | 0 | | |
| 3 | Damping | | 0 | 1 | 0.4 | 0 | | |
| 4 | Decay | s | 0.01 | 20 | 3 | 0 | | |
| 5 | Release | s | 0.01 | 20 | 0.3 | 0 | | |
| 6 | Body | | 0 | 1 | 0.5 | 0 | | |
| 7 | Size | | 0.5 | 2 | 1 | 0 | | |
| 8 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 9 | Stiffness | | 0 | 1 | 0.2 | 0 | | new |
| 10 | Exciter | | 0 | 2 | 0 | 2 | Noise, Pick, Mallet | new |
| 11 | Width | | 0 | 1 | 0.5 | 0 | | new |
| 12 | Bend | st | -12 | 12 | 0 | 0 | | new |

Chain: the extended Karplus–Strong plucked string (a delay loop seeded by a burst, damping, pick-position comb, decay stretch, a two-band body), sixteen voices. Exciter Noise is today's burst; Pick is one period of a band-limited saw, Mallet one period's half-sine, both at the burst's length and Tone's colour. Stiffness 0 is today's loop. Width spreads voices by key about middle C (60) across the field.

## Chain

| Stage | Rides |
|---|---|
| allocation, sixteen voices, oldest stolen | `core/voice` |
| exciter by a table of three: burst, saw period, half-sine period; velocity to level and colour as today | `core/noise` `Burst`, `core/oscillator` (saw; the sine `Table`) |
| pick-position comb | `core/line` |
| the string loop: fractional read, damping one-pole, then four first-order allpass stages (coefficient from Stiffness and pitch), the loop delay shortened by their phase delay at the note's frequency so the pitch holds; Bend re-tunes sounding voices as Tune does | `core/line` (`Line`, `Allpass` read, `Loop`), `core/filter` `Allpass` (Ring of one frame) |
| per voice place by key × Width, summed per side | NEEDS core/stereo (see pads) |
| body: the two peak bands per side (four biquads) | `core/filter` `Biquad` |
| DC blocker per side, Volume | `core/shaper` `Blocker` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; lane 0 left, lane 1 right, a single lane the mono sum (the dco's `side` rule).
- voicing: `poly`. Meter: `CORE::BLOCK::Meter` over the loudest lane.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::PLUCK`, 4 the sheet (indices in `pluck.hpp` grow by four), 5 names, 6 honesty, 7 cores (the allpass stages are `core/filter`'s; every ring built in `create`).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in, 6 (Bend rests at 0: its centre mark shows).
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; the exciter is a table of three, never a switch; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::PLUCK::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a note plucks and rings in tune; Stiffness 1 makes it bell-like and still in tune; Pick is cleaner, Mallet softer than Noise; low and high notes sit left and right.

## Learned

| Method | Source | Licence |
|---|---|---|
| stiff string: an allpass cascade for dispersion, allpass fractional tuning, pick-position comb, loop gain by pitch | STK `src/StifKarp.cpp`, `src/Plucked.cpp` | STK (MIT-like) |
| loop delay compensated for the loop filters' lag; allpass stretch | Mutable Rings `dsp/string.cc`; Plaits `dsp/physical_modelling/string.cc` | MIT |
| extended Karplus–Strong: pick-direction low-pass, damping, level by velocity | J.O. Smith, *Physical Audio Signal Processing* | copyright |
| exciter choices beyond the noise burst | Mutable Rings (exciters); Korg Prophecy plucked model (product page) | MIT; commercial page |
| equal-power pan | Bogaudio `signal.cpp` (matrix `## Methods`) | GPL-3.0+ (method only) |

Cores: core/stereo
