# slew — round 1

Gains a Shape row (logarithmic, linear, exponential) and CV on Rise and Fall. These answer matrix lacks 123 and 15 (slew's line). Later rounds: the nonlinear mode (124: small moves one-pole, large ones rate-limited, which needs a second time row) and an end-of-rise gate out.

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Rise | s | 0 | 5 | 0.001 | 0 | | |
| 1 | Fall | s | 0 | 5 | 0.001 | 0 | | |
| 2 | Shape | | -1 | 1 | 0 | 0 | | new |
| 3 | Amount | oct | -4 | 4 | 1 | 0 | | new |

Chain: the slew limiter with a shape control (Befaco Slew Limiter and Rampage, Bogaudio Slew, SuperCollider VarLag). Values chosen are ours:
- Shape crossfades three laws for the step toward the input. 0 is a constant rate (linear, today's).
- +1 is a rate proportional to the distance left: a one-pole, quick to start and slow to arrive.
- −1 is a rate falling with the distance left: slow to start and quick to arrive.
- Rise and Fall stay the time of a full-scale move on every law.
- Amount scales both times by 2^(−Amount × cv): a positive cv quickens.
- Shape rests at 0, so the rest is today's sound. The row is heard when turned.

## Chain

| Stage | Rides |
|---|---|
| in 0 and `cv` (in 1) read at their law-4 lanes | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| rise and fall × 2^(−Amount × cv), held 0..5 s, settled per sample | NEEDS core/block — `scaled` (see design/modular/envelope.md) |
| the step by Shape | NEEDS core/modulator — `Slew` gains `shape` −1..1, with 0 the standing constant rate, so `tick` keeps its output for every rider. The positive side blends in a step proportional to the distance; the negative side a step shrinking with it. Rider: slew |
| written in place into in 0's lanes; meter over the loudest lane | `core/block` |

## Surface

- ins: AUDIO `in`, AUDIO `cv`, CONTROL; outs: AUDIO. An effect: it reads both ins before it writes.
- voicing: none. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::SLEW`), law 4, law 5, law 6, law 7 (the shaped step is core/modulator's).
- 381: law 1; law 2 (`.type = "modular"`, no voicing); law 3; law 4; law 5.
- Size: small. The cv read goes in `voice.render.cpp`. `slew.hpp` moves from `sizeof` counts to `std::size`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no magic literals.

## Gate

The build waits until the library shift has landed `lane`, `scaled` and `Slew` `shape`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::SLEW::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard, with an lfo square into `in` and the slew into oscillator `pitch` under Play Free:
  - steps in pitch at rest;
  - Rise and Fall 0.3 give portamento;
  - Shape +1 gives a swoop that settles;
  - Shape −1 gives a swoop that lands hard;
  - an envelope into `cv` makes the glide quicken on each note.

## Learned

| Method | Source | Licence |
|---|---|---|
| slew shape by crossfading constant, distance-proportional and inverse rate laws | Befaco `src/Rampage.cpp`, `src/SlewLimiter.cpp` | GPL-3.0+ (method only) |
| separate up and down lags; a smoother morphing exponential to linear | Faust `signals.lib` (lag_ud, smoothq); SuperCollider `LagUD`, `VarLag` (docs) | per function; GPL-3.0 (method only) |
| nonlinear slew (read; a later round) | Pure Data `src/d_filter.c` (slop~) | BSD-3-Clause |

Cores: core/block
