# core/line — round 1

A stereo chorus of three swept reads, stated by three instruments: dco, polymod and supersaw (matrix shared 10). An existing core grows: it gains `Chorus`, lifted from `instrument/dco/voice.chorus.cpp`. `Line`, `Allpass`, `Tap`, `Sweep`, `Loop` and every `read`/`tick` stay as they are.

Sequence: seventh of eight, after core/stereo and core/modulator.

## Interface

`line.hpp` gains the following (≤ 85 lines), with a new unit `line.chorus.cpp`:

```cpp
#include "../modulator/modulator.hpp"
#include "../stereo/stereo.hpp"

constexpr Whole READS = 3;
constexpr Float CENTRE = 0.0035f;
constexpr Float DEEPEST = 0.003f;
constexpr Float LENGTH = 0.012f;

struct Chorus {
  Line line;
  Sweep sweeps[READS];
  MODULATOR::Lfo lfos[READS];
  Float wet = 0;
  Float whole = 0;
};

void build(Chorus &chorus, Whole rate);
void settle(Chorus &chorus, Float hertz, Float depth, Float wet, Whole rate);
auto tick(Chorus &chorus, Float in) -> STEREO::Pair;
```

- `build` is called once at create. It is the one allocation: a line of `LENGTH` (12 ms) at `rate`. The three LFOs are triangles started a third of a turn apart, then reset.
- `settle` arguments:
  - `hertz`: the sweep rate, 0.05..20 Hz.
  - `depth`: the sweep depth in seconds about `CENTRE` (3.5 ms), held to 0..`DEEPEST`.
  - `wet`: 0..1.
  - `rate`: the sample rate.
- `tick`: reads the three sweeps, then writes `in`.
  - It returns left = in + wet × (read₀ + ½ read₁) and right = in + wet × (read₂ + ½ read₁).
  - It sets `chorus.whole` = in + wet × (read₀ + read₁ + read₂) ÷ 3, the mono sum of the same tick.
  - This is dco's standing arithmetic, unchanged.
- dco keeps its I/II/I+II table: (0.513 Hz, 1.85 ms, 0.5), (0.863 Hz, 1.85 ms, 0.5), (9.75 Hz, 0.2 ms, 0.5), and Off at wet 0. Its `voice.chorus.cpp` leaves in dco's own order. polymod and supersaw settle (0.5 Hz, 2.5 ms, Chorus row).

## Laws

- 379 law 7: plain structs and functions, one chorus per instrument. `build` allocates once at create, and nothing allocates after. The core reads no sheet and publishes no meter.
- Context `SOUND::PLUGINS::CORE::LINE`. New sibling includes: `"../modulator/modulator.hpp"` and `"../stereo/stereo.hpp"`. Neither includes line, so there is no cycle.
- One-word names; files ≤ 100 lines, functions ≤ 25.
- No comments, no strings, no switch (the three reads in a loop over a table of their gains), no magic literals (`CENTRE`, `DEEPEST`, `LENGTH`, the third and the half named).
- Nothing of `AUDIO::PLUGIN`.

## Proof

- The gate: `build.sh` green; `nm -DC … | grep -c 'SOUND::PLUGINS::CORE::LINE::tick(SOUND::PLUGINS::CORE::LINE::Chorus'` non-zero; `pytest tests` green.
- `tests/test_line.py` over `tests/harness/line.cpp` asserts, at 48 kHz:
  - wet 0: left, right and `whole` equal `in` exactly over 1 s of a 1 kHz sine;
  - wet 1, 0.513 Hz, 1.85 ms: every tick satisfies `whole` = in + (left + right − 2·in) ÷ 3 (±1e-6);
  - the left/right correlation over 2 s is < 0.99;
  - every |out| ≤ 2.5;
  - `settle` with depth 0.01 holds it to `DEEPEST` (no read beyond the line).

## Learned

| Method | Source | Licence |
|---|---|---|
| bucket-brigade chorus as swept delay reads | Holters & Parker, DAFx-18 | copyright (method only) |

The three-read mix is dco's own (this repository's); nothing new is learned. No line is taken from any tree.

## Riders

- `instrument/dco`: `Chorus` under its I/II/I+II table; the left, the right, and `whole` for the mono lane.
- `instrument/polymod`: `Chorus` on the voice sum (0.5 Hz, 2.5 ms, Chorus row).
- `instrument/supersaw`: `Chorus` on the mono sum, added to its unison pair (0.5 Hz, 2.5 ms, Chorus row).
