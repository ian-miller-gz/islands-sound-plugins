# core/percussion — round 1

Five needs, each of three or more instruments:

| Need | Stated by | Matrix |
|---|---|---|
| velocity through a curve, and a touch on a frequency | clap, clave, conga, cowbell, cymbal, hat, kick, kit, machine, percussive, snare, tom | shared 4 |
| a choke fade | cymbal, hat, percussive | shared 19 |
| silence costing nothing | kit, machine, rhythm | |
| a sloped pitch sweep | conga, kick, snare, tom | shared 13 |
| a frame loop for a side pair | kit, machine, pads | |

An existing core grows: it gains `struck(velocity, curve)`, `touched`, `Choke`, `sounding` per circuit, `Sweep::power` with a six-argument `settle`, and `play` for a `STEREO::Pair`. Every circuit, `Metal`, `Resonator`, `keyed`, `turn`, `fall`, `shape`, the one-argument `struck` and the five-argument `Sweep` settle stay as they are.

Each drum's own circuit work (kick's body onto a `Sweep`, snare's heads, tom's modes, clap's bursts) belongs to that drum's order, in its `percussion.<drum>.*`. This order edits `percussion.hpp`, `percussion.circuit.hpp`, `percussion.cpp`, `percussion.sweep.cpp`, the new units below, and one `sounding` line per circuit header and tick.

Sequence: last of eight, after core/stereo.

## Interface

`percussion.circuit.hpp` (90 lines; `play` moves out, so it ends ≤ 80):

```cpp
struct Sweep {
  Resonator body;
  ENVELOPE::Envelope drop;
  ENVELOPE::Gate gate;
  Float base = 0;
  Float depth = 0;
  Float power = 1;
};

constexpr Float FLOOR = 0.0001f;

auto struck(Float velocity, Float curve) -> Float;
auto touched(Float hertz, Float velocity, Float touch) -> Float;

void settle(
  Sweep &sweep, Float decay, Float seconds, Float depth, Float slope,
  Whole rate);

auto sounding(const Resonator &resonator) -> Flag;
auto sounding(const Sweep &sweep) -> Flag;
```

New `percussion.choke.hpp`, with unit `percussion.choke.cpp`:

```cpp
struct Choke {
  Float level = 1;
  Float step = 1;
  Flag closed = false;
};

void settle(Choke &choke, Float seconds, Whole rate);
void open(Choke &choke);
void close(Choke &choke);
auto tick(Choke &choke) -> Float;
auto sounding(const Choke &choke) -> Flag;
```

New `percussion.play.hpp`, which takes the standing `play`. It includes `"percussion.circuit.hpp"` and `"../stereo/stereo.hpp"`:

```cpp
template <class Sound, class Apply>
auto play(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count, Sound &&sound,
  Apply &&apply) -> Float;
```

- `struck(velocity, curve)`:
  - `velocity` is held to 0..1, then raised to 4^−curve.
  - `curve` is −1..1: 0 is linear, +1 a light touch, −1 a heavy one.
  - The one-argument `struck` is curve 0.
- `touched(hertz, velocity, touch)`:
  - Returns hertz × 2^(touch × (velocity − 1)), with `touch` in octaves (≥ 0, or negative to raise soft hits).
  - A full hit keeps the row's value.
- `settle(sweep, decay, seconds, depth, slope, rate)`:
  - `slope` is −1..1 and sets `power` = 4^slope.
  - `tick` sets `turn` = base × (1 + depth × drop^power): 0 is today's exponential fall, +1 a snap, −1 a lingering bend.
  - The five-argument form is slope 0. `tick` skips the power when `power` is 1, so today's arithmetic holds bit for bit.
- `sounding`:
  - A resonator sounds while sine² + cosine² > `FLOOR`² (−80 dB). A sweep sounds while its body or its gate does.
  - Every circuit gains `auto sounding(const <Circuit> &) -> Flag`: true while any of its `ENVELOPE::Gate`s is not idle (`ENVELOPE::sounding`) or any of its resonators sounds.
  - The circuits are `Kick`, `Snare`, `Hat`, `Clap`, `Tom`, `Cymbal`, `Cowbell`, `Clave`, `Conga`, `HYBRID::Kick`, `HYBRID::Snare`, `HYBRID::Tom`, `HYBRID::Hat` and `Pad`.
  - Each declaration sits in its circuit's header, and the definitions go in the new units `percussion.sounding.cpp` and `percussion.hybrid.sounding.cpp`.
  - Each circuit's `tick` opens with `if (!sounding(circuit)) return 0;`, so every rider gains it free.
- `Choke`:
  - `settle`: `seconds` is the fade, 0.001..1 s, at `rate`; step = 1 ÷ (seconds × rate), at most 1.
  - `open`: level 1, not closed.
  - `close`: from now on, `tick` falls linearly by `step` to 0.
  - `tick`: returns the gain.
  - `sounding`: level > 0.
- `play`:
  - When `sound()` returns a `Float`, it is today's loop: one clipped sample to every lane.
  - When it returns a `STEREO::Pair`, each side is `BLOCK::clipped`, then `STEREO::pour` writes it: lane 0 left, lane 1 right, further lanes and a single lane the mean. The peak is the louder side.
  - One template chooses between the two by `if constexpr` on the result type.

## Laws

- 379 law 7: plain structs and functions, one voice per state. Nothing is allocated. The core reads no sheet and publishes no meter (the standing `play` returns the peak; the rider publishes).
- Context `SOUND::PLUGINS::CORE::PERCUSSION` (and `::HYBRID`). New sibling include: `"../stereo/stereo.hpp"`, in `percussion.play.hpp` only. core/stereo includes nothing of percussion.
- One-word names; files ≤ 100 lines (`percussion.cpp` 66 → ≤ 85; `percussion.hybrid.hat.cpp` stands at 94, so its `sounding` goes to the sounding unit, not the tick unit), functions ≤ 25.
- No comments, no strings, no switch (`if constexpr` on a type is not a switch), no magic literals (`FLOOR`, the 4 of 4^curve and 4^slope named).
- Of `AUDIO::PLUGIN`, `Sample` and `Event` only, as the standing `play` uses them.

## Proof

- The gate: `build.sh` green (every drum and kit links unchanged); `nm -DC … | grep -c 'SOUND::PLUGINS::CORE::PERCUSSION::sounding'` ≥ 14; `pytest tests` green.
- `tests/test_percussion.py` over `tests/harness/percussion.cpp` asserts, at 48 kHz:
  - `struck(0.5, 0)` = 0.5; `struck(0.5, 1)` = 0.8409; `struck(0.5, −1)` = 0.0625; `struck(2, 0)` = 1.
  - `touched(1000, 1, 2)` = 1000; `touched(1000, 0.5, 2)` = 500.
  - Choke over 0.01 s: 1 for 100 open ticks. After `close`, it reaches 0 at tick 480 (±1) and `sounding` is false after.
  - Sweep with slope 0 matches the five-argument settle bit for bit over 0.5 s.
  - Sweep, at a frame where drop ≈ 0.5: `body.turn` at slope +1 < slope 0 < slope −1.
  - Kick (standing settle, decay 0.5 s), struck at 1: `sounding` is true at first and false within 3 s. Every `tick` after that returns exactly 0.
  - `play` with a sound returning {0.5, −0.25}: 2 lanes get 0.5 and −0.25; 1 lane gets 0.125; 3 lanes get 0.5, −0.25, 0.125; the peak is 0.5. {3, 0} clips to 1.
  - `play` with a `Float` sound writes as today.

## Learned

| Method | Source | Licence |
|---|---|---|
| velocity through an adjustable curve | drumkv1 `src/drumkv1.cpp`; DrumGizmo `src/powermap.cc` | GPL-2.0+; LGPL-3.0+ (method only) |
| a pitch sweep on a power law; a cubic bend | LMMS Kicker `KickerOsc.h`; Befaco `Kickall.cpp` | GPL-2.0+; GPL-3.0+ (method only) |
| choke: a new hit gives its group a fast release | sfizz `Voice.cpp`, `Synth.cpp`; FluidSynth `fluid_synth.c` | BSD-2-Clause; LGPL-2.1+ (method only) |
| a voice freed below a floor; an early end under the noise floor | Faust `poly-dsp.h`; FluidSynth `fluid_voice.c` | LGPL-2.1+ with exception; LGPL-2.1+ (method only) |

The 4^curve and 4^slope laws (tine's rule), the linear choke fade and the −80 dB floor are ours. No line is taken.

## Riders

- `instrument/clap`, `clave`, `cowbell`: `struck(velocity, curve)` (clap's Touch on its gaps is its own).
- `instrument/conga`: `struck`, `Sweep` slope.
- `instrument/kick`, `snare`, `tom`: `struck`, `touched`, `Sweep` slope.
- `instrument/hat`: `struck`, `touched`, `Choke`.
- `instrument/cymbal`, `percussive`: `struck`, `Choke`.
- `instrument/kit`, `machine`: `struck`, `sounding`, `play` for a pair.
- `instrument/rhythm`: `sounding`.
- `instrument/pads`: `play` for a pair, in place of the frame loop its requirement writes itself (the same loop; see the notes).
