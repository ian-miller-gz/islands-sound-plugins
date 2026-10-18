# lfo — round 1

Gains a Skew row, a Smooth row for Random, a Fast range up to 200 Hz, a sync in and CV on its rate. These answer matrix lacks 112, 114, 113, 111 and 15 (lfo's line). Later rounds: tempo sync (S4: needs TRANSPORT to plugins), band-limited shapes at audio rate, Marbles-style spread and bias, polyphonic cables (S3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Wave | | 0 | 5 | 0 | 5 | Sine, Triangle, Saw, Square, Hold, Random | |
| 1 | Rate | Hz | 0.01 | 20 | 1 | 0 | | |
| 2 | Depth | | 0 | 1 | 1 | 0 | | |
| 3 | Offset | | -1 | 1 | 0 | 0 | | |
| 4 | Phase | | 0 | 1 | 0 | 0 | | |
| 5 | Fade | s | 0 | 10 | 0 | 0 | | |
| 6 | Skew | | 0 | 1 | 0.5 | 0 | | new |
| 7 | Smooth | | 0 | 1 | 1 | 0 | | new |
| 8 | Range | | 0 | 1 | 0 | 1 | Slow, Fast | new |
| 9 | Amount | oct | -4 | 4 | 1 | 0 | | new |

Chain: the eurorack LFO with reset, shape and a random source (Tides shape, Fundamental Random). Values chosen are ours:
- Skew warps the phase so that the first half-cycle spans Skew of the period. Triangle becomes a ramp at the ends, Square becomes a pulse width, and Sine and Saw lean. 0.5 gives today's shapes.
- Smooth is Random's glide in cycles: 0 gives stepped draws and 1 gives today's one-cycle glide.
- Fast multiplies Rate by 10, reaching 200 Hz. The shapes stay naive (a little aliasing at the top).
- Amount moves the rate by 2^(Amount × cv), held to the range.
- A rise on `sync` resets the phase to Phase, restarts Fade and draws again.

## Chain

| Stage | Rides |
|---|---|
| `sync` (in 0) and `cv` (in 1) read at their law-4 lanes | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| sync: a rise calls `reset` | NEEDS core/block — `Edge` (see design/modular/envelope.md); `core/modulator` `reset` (stands) |
| rate = Rate × (1 or 10) × 2^(Amount × cv), re-settled once per stride (16 frames) | NEEDS core/block — `scaled` (see design/modular/envelope.md), `Stride` (see design/subtractive/acid.md) |
| the shape with Skew on the phase; Random's slew = Smooth / rate | NEEDS core/modulator — `Lfo` gains `skew` 0..1. The phase is warped before the shape: p < s maps to p / (2s), otherwise 0.5 + (p − s) / (2(1 − s)). Hold and Random are untouched. The default 0.5 leaves every rider unchanged. Rider: lfo (the subtractive LFOs may ride it later) |
| Offset + the swing written into every lane of in 0; meter over the swing | `core/block` |

## Surface

- ins: AUDIO `sync`, AUDIO `cv`, CONTROL; outs: AUDIO `cv`. It reads both ins before it writes in 0's lanes. Unwired ins read 0, so it runs free as today.
- voicing: none (no NOTES in). Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::LFO`), law 4, law 5, law 6 (two AUDIO ins declared and read), law 7 (the skew warp is core/modulator's).
- 381: law 1; law 2 (`.type = "modular"`, no voicing); law 3; law 4; law 5 (−1..1 about Offset).
- Size: the in reading goes to a new `voice.ins.cpp`. `voice.settle.cpp` (25 lines) takes Smooth and Range through named constants.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Range by a table of two factors; no magic literals (10, 16 named).

## Gate

The build waits until the library shift has landed `lane`, `Edge`, `scaled`, `Stride` and `Lfo` `skew`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::LFO::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard, with the lfo into resonator `cv` (Depth 2) over noise:
  - the sweep from create; it is a modulation source, with no note;
  - Skew 0.05 on Triangle gives a falling ramp, and on Square a thin pulse;
  - Random with Smooth 0 gives steps;
  - Fast at Rate 20 (200 Hz) gives a buzz on the cutoff;
  - an oscillator square into `sync` restarts the cycle.

## Learned

| Method | Source | Licence |
|---|---|---|
| shape and slope from two stacked curve families | Mutable `tides2/ramp_shaper.h`, `tides2/poly_slope_generator.h` | MIT |
| random with stepped, linear and smooth interpolation between draws | VCV Fundamental `src/Random.cpp`; SuperCollider `NoiseUGens.cpp` (LFNoise0/1/2) | GPL-3.0+; GPL-2.0+ (method only) |
| reset on a trigger in; unipolar/bipolar | VCV Fundamental `src/LFO.cpp`; Bogaudio; Softube A-147 | GPL-3.0+ (method only); commercial manual |
| an LFO reaching audio rate | Reaktor Blocks (product page, 100 Hz); Tides 2 manual | commercial; MIT (docs) |

Cores: core/block
