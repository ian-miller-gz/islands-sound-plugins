# core/glottis — round 1

A tension glottal model with a spectral tilt and breath in the open phase alone, stated by three instruments: choir, vowel and whisper (matrix lack 34, shared 34). An existing core grows: `Source` gains a tilt, and a four-argument `settle` by tension is added. `build`, `seed`, `tune`, `reset`, `quotient`, `place`, `tick` and the three-argument `settle(source, open, breath)` stay as they are; the last stays until the last vocal order lands and leaves no caller.

Sequence: sixth of eight, after core/filter.

## Interface

`glottis.hpp` reads (≤ 50 lines):

```cpp
#pragma once
#include "../../../plugin.hpp"
#include "../filter/filter.hpp"
#include "../noise/noise.hpp"
#include "../oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::CORE::GLOTTIS {

constexpr Whole SHAPES = 16;
constexpr Float CLOSEST = 0.3f;
constexpr Float WIDEST = 0.9f;
constexpr Float SKEW = 0.7f;
constexpr Float FULL = 1.0f;
constexpr Float CLOSED = 0.25f;

struct Source {
  PHASE::Wheel phase = 0;
  PHASE::Wheel step = 0;
  PHASE::Wheel edge = 0;
  Float place = 0;
  Float breath = 0;
  Float closed = CLOSED;
  Flag tilted = false;
  FILTER::Pole tilt;
  NOISE::White white;
};

void settle(Source &source, Float open, Float breath);
void settle(Source &source, Float tension, Float breath, Whole rate);
```

The other declarations stand.

- The four-argument `settle`:
  - `tension` is 0..1. The open quotient is `WIDEST` − tension × (`WIDEST` − `CLOSEST`), so 0.5 gives today's 0.6; `place` and `edge` follow as today.
  - `tilt` is a `FILTER::Pole` LOW at 500 Hz × 2^(4 × tension): a lax voice is dark, a pressed one bright. `tilted` is set.
  - `closed` = 0, so breath sounds only in the open phase.
  - `breath` (0..1) × (1 − 0.5 × tension), held to 0..1.
  - `rate` is the sample rate, in Hz.
- The three-argument `settle` sets `tilted` false and `closed` back to `CLOSED`, so it keeps today's sound.
- `tick`: the pulse passes `tilt` when `tilted`. The noise gate is `FULL` in the open phase and `closed` after it. It returns pulse + (noise − pulse) × breath, as today.

## Laws

- 379 law 7: one plain struct, plain functions, one voice. The pulse table is built at create by `build` (as today); nothing is allocated after. The core reads no sheet and publishes no meter.
- Context `SOUND::PLUGINS::CORE::GLOTTIS`. New sibling include: `"../filter/filter.hpp"`. core/filter includes nothing of glottis, so there is no cycle.
- One-word names; files ≤ 100 lines (`glottis.cpp` stands at 81; the tension settle goes to a new `glottis.tension.cpp`), functions ≤ 25.
- No comments, no strings, no switch, no magic literals (500 Hz, the 4 octaves and the 0.5 breath fall named; `CLOSED` moves from the unit to the header).
- Nothing of `AUDIO::PLUGIN`.

## Proof

- The gate: `build.sh` green; `nm -DC … | grep -c 'SOUND::PLUGINS::CORE::GLOTTIS::settle'` ≥ 2; `pytest tests` green.
- `tests/test_glottis.py` over `tests/harness/glottis.cpp` asserts, at 48 kHz and 110 Hz:
  - tension 0.5 gives `edge` = 0.6 of a turn (±1e-3);
  - the brightness (RMS of the first difference ÷ RMS) at tension 1 is ≥ 2 × that at tension 0, with breath 0;
  - breath 1, tension 0.5, two noise seeds: every closed-phase sample matches between the two runs, while open-phase samples differ;
  - the three-argument `settle(source, 0.6, 0.2)` sounds as it does today: its `tick` output over 1 s equals a run with `closed` 0.25 and no tilt, bit for bit.

## Learned

| Method | Source | Licence |
|---|---|---|
| one tension value shaping the pulse from lax to pressed | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) |
| aspiration lowered as tension rises, gated by the glottal phase | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) |
| a one-pole spectral tilt; breath only in the open phase | klatt-syn `src/Klatt.ts` | MIT |

The corner law (500 Hz × 2^(4t)) and the breath fall are ours. No line is taken.

## Riders

- `instrument/vowel`: the four-argument `settle` by Tension and Breath.
- `instrument/choir`: the four-argument `settle` by Tension and Breath, per singer.
- `instrument/whisper`: the four-argument `settle` at a named lax tension 0.3 with no breath, under the noise by Voicing.
