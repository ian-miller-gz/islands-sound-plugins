# core/stereo — round 1

The equal-power pan, a key place, and a side pair written to the lanes, stated by ten instruments: pads, pluck, tine, wavetable, kit, machine, mixer, choir, vowel and whisper (matrix shared 5). A new family (379 law 5: one word; no plugin is named `stereo`).

Sequence: second of eight, after core/block. core/line `Chorus` and core/percussion `play` return its `Pair`.

## Interface

`core/stereo/stereo.hpp`, with unit `stereo.cpp`:

```cpp
#pragma once
#include "../block/block.hpp"

namespace SOUND::PLUGINS::CORE::STEREO {

constexpr Float CENTRE = 60.0f;
constexpr Float SPAN = 30.0f;

struct Pair {
  Float left = 0;
  Float right = 0;
};

auto pan(Float place) -> Pair;
auto place(Float pitch, Float width) -> Float;
void add(Pair &sum, Float in, Pair gains);
auto magnitude(Pair pair) -> Float;
void pour(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame, Pair pair);

}  // namespace SOUND::PLUGINS::CORE::STEREO
```

- `pan`: takes `place` from −1 (left) to 1 (right), held to that range, and returns the gains √2·cos(π(place + 1)/4) and √2·sin(π(place + 1)/4).
  - At 0 it returns {1, 1}, so a centred source keeps today's level on each lane.
  - At −1 it returns {√2, 0}; at 1 it returns {0, √2}.
  - left² + right² = 2 at every place.
- `place`: (pitch − `CENTRE`) ÷ `SPAN` × width, held to −1..1. `pitch` is in keys (a glided key may be fractional); `width` is 0..1. The full field is at ±30 keys about middle C (tine's and pluck's rule).
- `add`: `sum.left += in × gains.left`, and the same for the right.
- `magnitude`: the larger of |left| and |right|. It is the peak of the loudest side.
- `pour`: lane 0 takes the left, lane 1 the right. Every further lane, and a single lane, takes the mean (left + right) ÷ 2. `frame` is below the block's frames.

## Laws

- 379 law 7: plain functions and one plain struct. Nothing is allocated. The core reads no sheet and publishes no meter.
- Context `SOUND::PLUGINS::CORE::STEREO`. It includes `"../block/block.hpp"` (for `BLOCK::magnitude` and the cartridge header) and nothing else.
- One-word names; files ≤ 100 lines, functions ≤ 25.
- No comments, no strings, no switch (the lane rule is one comparison on the lane index), no magic literals (`CENTRE`, `SPAN`, √2 and π/4 named).
- Of `AUDIO::PLUGIN`, `Sample` alone.

## Proof

- The gate: `build.sh` green; `nm -DC … | grep -c 'SOUND::PLUGINS::CORE::STEREO::pan'` non-zero; `pytest tests` green.
- `tests/test_stereo.py` over `tests/harness/stereo.cpp` (fixture from core/block) asserts:
  - `pan(0)` = {1, 1}, `pan(−1)` = {1.41421, 0}, `pan(1)` = {0, 1.41421} (±1e-5).
  - Over 41 places −1..1, left² + right² = 2 (±1e-5).
  - `pan(−3)` equals `pan(−1)`.
  - `place(60, 1)` = 0; `place(90, 1)` = 1; `place(45, 1)` = −0.5; `place(120, 1)` = 1; `place(90, 0.5)` = 0.5.
  - {0.5, −0.25} poured into 1 lane gives [0.125]; into 2 lanes [0.5, −0.25]; into 4 lanes [0.5, −0.25, 0.125, 0.125].
  - `magnitude({−0.5, 0.3})` = 0.5.

## Learned

| Method | Source | Licence |
|---|---|---|
| equal-power pan from quarter-offset sine gains | Bogaudio `src/dsp/signal.cpp` | GPL-3.0+ (method only) |
| notes panned by key about the middle of the keyboard | mda ePiano `mdaEPiano.cpp` | GPL-3.0+ port read (method only); original MIT or GPL-2.0 |

The √2 normalisation (a centred source at today's level) and the ±30-key field are ours. No line is taken.

## Riders

- `instrument/pads`: `pan` per pad, summed by `add`, written through core/percussion `play` for a pair.
- `instrument/kit`, `instrument/machine`: `pan` per drum, `add`, through `play` for a pair.
- `instrument/pluck`, `instrument/tine`: `place` by key × Width, `pan`, `add`, `pour`.
- `instrument/wavetable`: `pan` of `VOICE::stack`'s unison pan, `add`, `pour`.
- `modular/mixer`: `pan` per in (a centred in keeps today's level), `pour`.
- `instrument/choir`, `instrument/vowel`, `instrument/whisper`: `pan` of the seeded place × Width, `add`, `pour`, `magnitude` for the meter.
