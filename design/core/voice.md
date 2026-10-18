# core/voice — round 1

Per-slot trims drawn once, stated by four instruments: dco, duo, multimode and polymod (matrix shared 9, the `Note` offsets). An existing core grows: it gains `Trim` and `draw`. `Allocator`, `Note`, `Glide`, the three modes, `stack`, `hold` and `drain` stay as they are.

Sequence: fourth of eight, after core/modulator.

## Interface

New header `voice.trim.hpp`, included at the foot of `voice.hpp` beside `voice.modes.hpp` (`voice.hpp` stands at 94 lines and grows by one). New unit `voice.trim.cpp`.

```cpp
#pragma once
#include "../modulator/modulator.hpp"
#include "voice.hpp"

namespace SOUND::PLUGINS::CORE::VOICE {

struct Trim {
  Float pitch = 0;
  Float cutoff = 0;
  Float time = 0;
};

void draw(Trim *trims, Whole count, MODULATOR::Seed seed);

template <Whole COUNT>
void draw(Trim (&trims)[COUNT], MODULATOR::Seed seed) {
  draw(trims, COUNT, seed);
}

}  // namespace SOUND::PLUGINS::CORE::VOICE
```

- Each field is −1..1. `draw` fills every slot by `MODULATOR::draw` from a local copy of `seed`, slot by slot, in the order pitch, cutoff, time, so a seed always yields the same trims.
- It is called once at create; it needs no allocation, so a rider may re-draw at any time.
- The rider scales the fields (dco: ±8 ct, ±0.25 oct, ±10 % of each envelope time, × its Trim row).

## Laws

- 379 law 7: one plain struct and one function, per-slot state. Nothing is allocated. The core reads no sheet and publishes no meter.
- Context `SOUND::PLUGINS::CORE::VOICE`. New sibling include: `"../modulator/modulator.hpp"`. core/modulator includes nothing of voice, so there is no cycle.
- One-word names; files ≤ 100 lines (`voice.hpp` ≤ 95), functions ≤ 25.
- No comments, no strings, no switch, no magic literals.
- Nothing of `AUDIO::PLUGIN` is added.

## Proof

- The gate: `build.sh` green; `nm -DC … | grep -c 'SOUND::PLUGINS::CORE::VOICE::draw'` non-zero; `pytest tests` green.
- `tests/test_voice.py` over `tests/harness/voice.cpp` asserts, for 16 trims from seed 7:
  - every field lies within −1..1;
  - the pitch fields are not all equal, and their mean |pitch| is > 0.2;
  - seed 7 drawn again gives identical trims, and seed 8 differs in slot 0.

## Learned

| Method | Source | Licence |
|---|---|---|
| per-voice variance drawn once per slot, scaled by global rows | 2DaT Obxd `Source/Engine/ObxdVoice.h`; Repro trimmers (manual) | GPL-3.0 (method only); manual |

No line is taken.

## Riders

- `instrument/dco`: pitch, cutoff and time trims per slot (Trim row).
- `instrument/polymod`: pitch, cutoff and time trims per slot.
- `instrument/multimode`: pitch, cutoff and time trims per slot.
- `instrument/duo`: the pitch trim per part.
