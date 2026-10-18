# envelope — round 1

Gains Delay and Hold stages, a bow per stage (Rise, Fall), a Loop, a gate in and CV on its times. These answer matrix lacks 108, 109, 110, 43 and 15 (envelope's line). This file defines the family's shared `core/block` lines (`lane`, `Edge`, `scaled`). Later rounds: a logarithmic bow (curvature below linear), end-of-cycle out, tempo sync (S4), polyphonic cables (S3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Shape | | 0 | 3 | 0 | 3 | ADSR, AD, AR, Gate | |
| 1 | Attack | s | 0.001 | 10 | 0.005 | 0 | | |
| 2 | Decay | s | 0.001 | 10 | 0.2 | 0 | | |
| 3 | Sustain | | 0 | 1 | 0.7 | 0 | | |
| 4 | Release | s | 0.001 | 10 | 0.3 | 0 | | |
| 5 | Curve | | 0 | 1 | 1 | 1 | Linear, Exponential | |
| 6 | Trigger | | 0 | 1 | 1 | 1 | Reset, Resume | |
| 7 | Velocity | | 0 | 1 | 0.5 | 0 | | |
| 8 | Depth | | 0 | 1 | 1 | 0 | | |
| 9 | Delay | s | 0 | 10 | 0 | 0 | | new |
| 10 | Hold | s | 0 | 10 | 0 | 0 | | new |
| 11 | Rise | | 0 | 1 | 0.5 | 0 | | new |
| 12 | Fall | | 0 | 1 | 1 | 0 | | new |
| 13 | Loop | | 0 | 2 | 0 | 2 | Off, Held, Free | new |
| 14 | Time | oct | -4 | 4 | 1 | 0 | | new |

Chain: the eurorack DAHDSR with a cycle switch (Bogaudio DADSRH and the Stages segment chain, as described in their manuals). Values chosen are ours:
- Rise and Fall bow the exponential stages. The ratio the stage aims past its end is r = 10^(2 − 5 × bow): bow 0 is nearly linear, and bow 1 is r = 0.001. Rise 0.5 gives r ≈ 0.32, today's 0.3 attack; Fall 1 gives today's 0.001 decay and release. At rest the sound is today's. Under Curve Linear both rows are ignored.
- Delay holds the strike back by its time. Hold freezes the level at the peak for its time before Decay starts.
- Loop Held strikes again when the stage reaches the sustain (ADSR, AR) or its end (AD) while the gate or a note is held. Loop Free does the same from create, with nothing held: a shaped LFO. Trigger decides whether the strike starts from 0 or from the level.
- Time moves every stage time by 2^(−Time × cv): a cv of 1 with Time 1 halves the times.

## Chain

| Stage | Rides |
|---|---|
| the AUDIO ins read at their law-4 lanes: `gate` (in 0), `cv` (in 1) | NEEDS core/block — `lane(lanes, in, channel, channels) -> AUDIO::PLUGIN::Sample *`: 381 law 4's address `lanes[in × channels + channel]`, where `in` counts AUDIO ins only. Riders: all nine modules (amplifier, mixer and resonator move their hand indexing onto it) |
| gate in, lane 0: a rise strikes at velocity 1; a fall lifts unless a note is held. A lift of the last note lifts unless the gate is high | NEEDS core/block — `Edge`: `{Flag high; Float last; Float since;}`; `crossed(edge, in) -> Whole` returns `STILL`, `ROSE` or `FELL` with hysteresis (rises above `UPPER` 0.1, falls below `LOWER` 0.05, both named in core/block); on a rise, `since` holds the part of the frame after the crossing, (in − UPPER)/(in − last). Riders: envelope (gate), lfo (sync), oscillator (sync) |
| notes: the mono legato allocator (stands) | `core/voice` |
| Delay countdown before `strike`; Hold countdown entered when the stage passes RISING → FALLING, not ticking while it counts | in the module (`voice.stages.cpp`) |
| stage times × 2^(−Time × cv), re-shaped once per stride (16 frames) while cv moves | NEEDS core/block — `scaled(base, cv, octaves, least, most) -> Float`: base × 2^(octaves × cv) held to least..most. Riders: envelope, lfo, noise, resonator, slew. NEEDS core/block — `Stride` (see design/subtractive/acid.md) |
| the ADSR/AD/AR/Gate envelope, with Rise and Fall setting the exponential ratios | NEEDS core/envelope — `Envelope` gains `rise` and `fall`, the ratios the exponential builder aims past a stage's end. They default to 0.3 and 0.001 (today's `RISE` and `SETTLE`), so every rider keeps its sound. `shape` reads them |
| Loop: restrike at sustain or end, by the table Off/Held/Free | in the module (`voice.gate.cpp`) |
| level × Depth written into every lane of in 0; meter over the level | `core/block` |

## Surface

- ins: AUDIO `gate`, AUDIO `cv`, NOTES, CONTROL; outs: AUDIO `cv`. It reads the gate before it writes. The output leaves in in 0's lanes (381 law 4). An unwired in reads 0, so NOTES alone play it as today.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1 (tree), law 2 (includes: core aggregates only), law 3 (`SOUND::PLUGINS::ENVELOPE`), law 4 (the sheet), law 5 (names), law 6 (honesty: two AUDIO ins declared and read), law 7 (cores: `lane`, `Edge` and `scaled` are core/block's, never re-rolled).
- 381: law 1; law 2 (`.type = "modular"`, `.voicing = SOUND::PLUGIN::MONO`); law 3 (files under `modular`); law 4 (two lane sets in port order; the first leaves holding the output); law 5 (CV in the unit interval).
- Size: `voice.settle.cpp` stands at 83 lines. The shape builders move to `voice.shapes.cpp`; gate and Loop go in `voice.gate.cpp`, Delay and Hold in `voice.stages.cpp`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Loop and Shape by tables, never a switch; no magic literals (2, 5, 16 named).

## Gate

The build waits until the library shift has landed `lane`, `Edge`, `scaled`, `Stride` and the `rise`/`fall` fields.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::ENVELOPE::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard, in the patch oscillator → amplifier with envelope → amplifier `cv`:
  - silent before the note, sounding on the note, and silent within the release;
  - Hold 0.3 gives a plateau at the peak, and Delay 0.2 gives a late start;
  - Fall 0 gives a straight-line decay;
  - Loop Held repeats the attack-decay while the key is held;
  - an lfo square into `gate` plays it with no note.

## Learned

| Method | Source | Licence |
|---|---|---|
| power-curve stages with a linear switch; delay and hold stages | Bogaudio `src/dsp/envelope.cpp` | GPL-3.0+ (method only) |
| a segment curve as a rational warp; one segment chain gives ADSR, loop and slew | Mutable `stages/segment_generator.cc` | MIT |
| curvature as one signed number per segment; loop and release nodes | SuperCollider `Env` (class docs) | GPL-3.0 (method only) |
| each stage aims at a target past its end (our ratio) | VCV Fundamental `src/ADSR.cpp` | GPL-3.0+ (method only) |
| a gate read from CV beside the keyboard; a retrigger in | VCV Fundamental `ADSR.cpp`; Softube Modular manual (A-140) | GPL-3.0+; commercial manual |

Cores: core/block
