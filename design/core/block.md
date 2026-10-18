# core/block — round 1

Control rate, law-4 lane reads, gate edges and CV scaling, stated by 18 instruments (`Stride`), the nine modules (`lane`), envelope, lfo and oscillator (`Edge`), and envelope, lfo, noise, resonator and slew (`scaled`). An existing core grows: it gains `Stride`, `lane`, `Edge` and `scaled`. `Meter`, `Cursor`, `due`/`rest`, `denormals`, `publish`, `read`, `magnitude`, `clipped` and `loudest` stay as they are.

Sequence: first of eight library orders. It founds the harness fixture the other seven use.

## Interface

`block.hpp` gains (≤ 80 lines in all):

```cpp
constexpr Float UPPER = 0.1f;
constexpr Float LOWER = 0.05f;

enum Crossing : Whole { STILL, ROSE, FELL, CROSSINGS };

struct Stride {
  Whole left = 0;
};

struct Edge {
  Flag high = false;
  Float last = 0;
  Float since = 0;
};

auto due(Stride &stride, Whole every) -> Flag;
void reset(Stride &stride);

auto lane(
  AUDIO::PLUGIN::Sample *const *lanes, Whole in, Whole channel, Whole channels)
  -> AUDIO::PLUGIN::Sample *;

auto crossed(Edge &edge, Float in) -> Whole;

auto scaled(Float base, Float cv, Float octaves, Float least, Float most)
  -> Float;
```

- `due`: true on the first call after construction or `reset`, then once every `every` calls (one call per frame). `every` is in frames, and 0 reads as 1. `reset` makes the next call due; a rider calls it when a row moves.
- `lane`: returns `lanes[in × channels + channel]`. `in` counts AUDIO ins only, in port order (381 law 4); `channel` is below `channels`. No bounds are checked, because the rider's own port list states the number of ins.
- `crossed`: returns `ROSE` when `in` passes above `UPPER` while low, `FELL` when it drops below `LOWER` while high, and `STILL` otherwise. Every call sets `last = in`. On `ROSE`, `since = (in − UPPER) / (in − last)`, in 0..1: the part of the frame after the crossing.
- `scaled`: returns `base × 2^(octaves × cv)`, held to `least..most`. `cv` is a law-5 signal (unbounded read); `octaves` ≥ 0; `base`, `least` and `most` share one unit (Hz, s, …).
- New unit: `block.control.cpp` (Stride, lane, Edge, scaled). `block.cpp` stays.

## Laws

- 379 law 7: plain functions and plain state structs, one voice. Nothing is allocated, ever. The core reads no sheet and publishes no meter beyond the standing `Meter` pair.
- Context `SOUND::PLUGINS::CORE::BLOCK`. Includes stay as they stand (`<threads.hpp>`, `<xmmintrin.h>`, `"../../../plugin.hpp"`); no sibling core.
- One-word names; files ≤ 100 lines, functions ≤ 25.
- No comments: the shape suite admits none, not even a `///` citation. Sources are cited in this file.
- No strings; no switch (the crossing is two comparisons); no magic literals (`UPPER`, `LOWER` and the 0-as-1 rule named).
- Of `AUDIO::PLUGIN`, only `Sample` and `Event`.

## Proof

- The gate: `cartridges/build.sh` green over the snapshot; `nm -DC <SNAPSHOT>/posix/libsound.so | grep -c 'SOUND::PLUGINS::CORE::BLOCK::crossed'` non-zero; `pytest tests` green, with the shape suite covering the new unit.
- **Founds the harness fixture** `tests/conftest.py`, with fixture `harness(name, *extra)`:
  - Engine root E is `$ISLANDS_ENGINE`, else `~/Repositories/mine/islands-private`. Snapshot S is the worktree's parent.
  - Skips with a reason when `E/src/cartridge/plugin.hpp` or `S/plugin.hpp` is missing. GitHub CI has no engine, so it skips there; the shift's notes state that the harness tests passed rather than skipped.
  - Units: `core/<name>/*.cpp`, plus every sibling family its headers include as `"../<family>/<family>.hpp"`, followed transitively, plus the `extra` families.
  - Compile: `g++ -std=c++23 -O1 <roots> -c <unit> -MMD`, then link with `tests/harness/<name>.cpp` into one binary.
    - `<roots>` is the set `cartridges/build.sh` lays, read the same way: `-isystem` every directory under `E/src` holding a `.hpp`, plus `E/src`, `E/src/core`; `-I E/build/include`; `-I E/<each configs/make.yaml includes: entry>`.
    - The core reaches S through its own `"../../../plugin.hpp"`, so S needs no root.
  - Objects are cached under pytest's cache (`config.cache.mkdir('core')`). A unit is rebuilt when it is older than its source or than any header its `.d` names (build.sh's rule).
  - The binary prints `<word> <number>…` lines. The fixture returns word → list of floats.
  - A harness `.cpp` carries the SPDX line, no comments, and opens no namespace outside `SOUND::PLUGINS`.
- `tests/test_block.py` over `tests/harness/block.cpp` asserts:
  - Stride, every 16, 64 calls: due exactly at calls 0, 16, 32, 48. With a reset just before call 5: due at 0, 5, 21, 37, 53.
  - `lane(lanes, 2, 1, 2)` is `lanes[5]`.
  - Edge over 0, 0.08, 0.2, 0.07, 0.04, 0.09, 0.5: STILL, STILL, ROSE with since = 0.8333 (±1e-4), STILL, FELL, STILL, ROSE.
  - `scaled(100, 1, 1, 20, 20000)` = 200; `(100, −1, 2, …)` = 25; `(100, 10, 1, …)` = 20000; `(100, −10, 1, …)` = 20.

## Learned

| Method | Source | Licence |
|---|---|---|
| coefficients settled at a control rate | stmlib `dsp/parameter_interpolator.h` | MIT |
| a reset placed at its sub-sample crossing | stmlib `dsp/polyblep.h`; Plaits `variable_shape_oscillator.h` | MIT |
| a few CV inputs scaled onto rows by a depth | Surge XT Rack | GPL-3.0+ (method only) |

The hysteresis thresholds and the octave law are ours. No line is taken.

## Riders

- `instrument/acid`, `dco`, `duo`, `ladder`, `mono`, `multimode`, `supersaw`: `Stride` (filter settle per stride).
- `instrument/additive`, `wavetable`, `sampler`: `Stride` (spectrum, filter, pitch and cut per stride).
- `instrument/choir`, `vowel`, `whisper`: `Stride` (pitch, tract and formant per stride).
- `modular/envelope`: `lane`, `Edge` (gate), `scaled` (Time), `Stride`.
- `modular/lfo`: `lane`, `Edge` (sync), `scaled` (rate), `Stride`.
- `modular/oscillator`: `lane`, `Edge` (sync, with `since`).
- `modular/noise`: `lane`, `scaled`, `Stride`.
- `modular/resonator`: `lane`, `scaled`, `Stride`.
- `modular/amplifier`: `lane`, `Stride`.
- `modular/slew`: `lane`, `scaled`.
- `modular/mixer`, `modular/shaper`: `lane`.
