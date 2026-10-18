# acid — round 1

Gains the bassline filter voicing beside the diode, its own accent decay, an output drive, both nonlinear stages at 2x and settled at control rate — matrix lacks 45, 46, 6, 14. Later rounds: the pattern sequence (lack 24, a `core/sequence` and TRANSPORT first).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | st | -12 | 12 | 0 | 0 | | |
| 1 | Wave | | 0 | 1 | 0 | 1 | Saw, Square | |
| 2 | Cutoff | Hz | 20 | 20000 | 500 | 0 | | |
| 3 | Resonance | | 0 | 1 | 0.6 | 0 | | |
| 4 | Envelope | oct | 0 | 5 | 2 | 0 | | |
| 5 | Decay | s | 0.2 | 2 | 0.5 | 0 | | |
| 6 | Accent | | 0 | 1 | 0.5 | 0 | | |
| 7 | Slide | s | 0.01 | 0.5 | 0.06 | 0 | | |
| 8 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 9 | Filter | | 0 | 1 | 1 | 1 | Diode, Bassline | new |
| 10 | Snap | s | 0.05 | 1 | 0.2 | 0 | | new |
| 11 | Drive | | 1 | 8 | 1.5 | 0 | | new |

Chain: the bass line box's public path (one oscillator, the four-pole diode-coupled filter, accent, slide). Snap rests at today's fixed accented decay (0.2 s), so Filter is the only move at rest; Drive 1 is unity into the soft clip.

## Chain

| Stage | Rides |
|---|---|
| note: monophonic, last priority, legato, glide only on a tied note (Slide) | `core/voice` |
| oscillator Saw/Square, band-limited; 44 Hz coupling high-pass | `core/oscillator`, `core/filter` `Pole` |
| contours: plain AD (Decay), accented AD (Snap), amplifier ADSR; accent push smoother | `core/envelope`, `core/modulator` `Smoother` |
| filter, settled once per stride at twice the rate: Diode → `FILTER::Diode`; Bassline → this instrument's own `voice.filter.cpp`: four low `Pole` stages, a `Pole` high-pass (our corner) inside the resonance feedback, the feedback gain fitted against cutoff by a `constexpr` curve of our own | `core/filter`; NEEDS core/block — `Stride`: a control-rate counter, `due(stride, every)` true on the first frame and once per `every` frames, `reset(stride)` forcing the next frame due when a row moves |
| the filter and Drive (`SHAPER::soft` of Drive × signal, level-normalised) run inside one 2x pass | `core/shaper` `Oversampler` (its `tick` template, the shape being the filter then the clip) |
| DC blocker, Volume | `core/shaper` `Blocker` |

The Bassline filter stays in the instrument: one instrument states it (a need of fewer than three stays in its requirement).

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in, every sample written, the same sample to every lane.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`, published every block.

## Laws

- 379: 1 tree (`instrument/acid/`, a dotted sibling `voice.filter.cpp`), 2 includes, 3 context `SOUND::PLUGINS::ACID`, 4 the sheet (`CORE::TABLE`; a row change re-settles and resets the stride), 5 names, 6 honesty, 7 cores (the stride and the 2x pass come from core/).
- 381: 1 context, 2 `.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`, 3 files under instruments/mono, 4 no AUDIO in: `channels` lanes.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings but the row names and step words; no switch where a table serves (Filter picks from a table of two ticks); no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (the build lane's snapshot; lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::ACID::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before the first note, a note sounds; at Resonance 0.8 Filter Diode and Bassline differ plainly (the bassline's thinner bottom and its squelch); an accented note's blip shortens as Snap falls; Drive 6 grits without fizz.

## Learned

| Method | Source | Licence |
|---|---|---|
| a one-pole high-pass inside the resonance feedback; resonance gain fitted against cutoff | Open303 `rosic_TeeBeeFilter.h` | MIT |
| accent as its own shorter decay summed onto the plain one; slide only over a held note | Open303 `rosic_Open303.h`, `rosic_Open303.cpp` | MIT |
| the separate accent decay and the output drive as panel switches | JC303 `JC303.cpp` (the modded-unit switches) | GPL-3.0 (method only) |
| the nonlinear filter run oversampled behind a half-band decimator | Open303 `rosic_Open303.h` (4x; ours 2x on the standing half-band) | MIT |
| coefficients settled at control rate, not every sample | stmlib `dsp/parameter_interpolator.h` | MIT |

Cores: core/block
