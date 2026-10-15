# core/filter — round 1

Three needs, each of three or more instruments:
- a saturating four-pole cascade, stated by dco, duo, ladder and mono (matrix lack 39, shared 20);
- a saturating state-variable loop, stated by duo, multimode and resonator (shared 20);
- a derived vocal tract with a smoothed, normalised formant bank, stated by choir, vowel and whisper (shared 1, 29–33).

An existing core grows: it gains `Transistor`, `Variable::saturation`, `Tract`, `Register` and the re-founded `Formant`. `Pole`, `Biquad`, `Ladder`, `Diode`, `Sallen`, `Ring` and the five-argument `Variable` settle stay as they are. `Formant`'s standing `settle(formant, vowel, shift, rate)` and `tick` stay too.

Sequence: fifth of eight.

## Interface

`filter.ladder.hpp` gains (≤ 75 lines), with a new unit `filter.transistor.cpp`:

```cpp
constexpr Float RING = 4.0f;

struct Transistor {
  Float states[POLES] = {0, 0, 0, 0};
  Float gain = 0;
  Float feedback = 0;
  Float drive = 1;
  Float lift = 1;
  Float last = 0;
};

void settle(
  Transistor &transistor, Float cutoff, Float emphasis, Float drive,
  Float bass, Whole rate);
auto tick(Transistor &transistor, Float in) -> Float;
```

- `settle` arguments:
  - `cutoff`: Hz, through `bounded(cutoff, rate)`.
  - `emphasis`: 0..1, giving `feedback` = `RING` × emphasis; emphasis 1 self-oscillates.
  - `drive`: ≥ 1, the gain into the cascade.
  - `bass`: 0..1, giving `lift` = 1 + bass × feedback, which restores the passband loss.
  - `rate`: the rate `tick` runs at. Every rider runs it at 2× the sample rate, inside `SHAPER::tick(oversampler, in, …)`.
  - `gain` = 1 − e^(−2π·cutoff/rate). The shift may correct it by a polynomial of its own in cutoff/rate until the tuning assertion holds.
- `tick`:
  - The fed-back signal is the mean of the fourth stage's output now and its previous output (`last`): the feedback is half a sample late.
  - x = `soft`(drive × lift × in − feedback × fed).
  - Each stage does `state += gain × (soft(stage in) − soft(state))`.
  - It returns the fourth state.

`filter.linear.hpp` gains, and `filter.variable.cpp` reads:

```cpp
struct Variable {
  Float damping = 0;
  Float weights[WEIGHTS] = {0, 0, 0};
  Float states[INTEGRATORS] = {0, 0};
  Float mix[WEIGHTS] = {0, 0, 1};
  Float saturation = 0;
};

void settle(
  Variable &variable, Whole kind, Float cutoff, Float emphasis,
  Float saturation, Whole rate);
```

- `saturation` is 0..1. In `split`, the band value is blended toward `SHAPER::soft(band)` by `saturation` before the integrator states update. At 0 the arithmetic is today's, bit for bit (wah, resonator and the 12 dB riders keep their sound).
- The six-argument shape matches `Sallen`'s, so resonator's table of (settle, tick) pairs holds both. The five-argument `settle` sets saturation 0.

`filter.formant.hpp` is re-founded (≤ 60 lines). New unit `filter.tract.cpp` holds the targets and `aim`/`bend`; `filter.formant.cpp` holds `glide`, `jump`, `makeup`, `settle` and `tick`, and its five-vowel table leaves:

```cpp
enum Vowel : Whole { A, E, I, O, U, VOWELS };
enum Register : Whole { BASS, TENOR, ALTO, SOPRANO, REGISTERS };

constexpr Whole FORMANTS = 5;

struct Tract {
  Float centres[FORMANTS] = {};
  Float widths[FORMANTS] = {};
  Float levels[FORMANTS] = {};
};

struct Formant {
  Biquad bands[FORMANTS];
  Float gains[FORMANTS] = {};
  Float settled[FORMANTS] = {};
  Tract tract;
  Float breadth = 1;
  Float applied = 1;
};

void aim(Tract &tract, Float voice, Float vowel, Float target, Float part);
void bend(Tract &tract, Float hertz, Float effort);
auto glide(
  Formant &formant, const Tract &tract, Float pole, Float shift, Whole rate)
  -> Flag;
void jump(Formant &formant, const Tract &tract, Float shift, Whole rate);
auto makeup(const Formant &formant, Whole rate) -> Float;
void settle(Formant &formant, Float vowel, Float shift, Whole rate);
auto tick(Formant &formant, Float in) -> Float;
```

- The tables in `filter.tract.cpp`'s anonymous context use our own values (from design/vocal/vowel.md):
  - `Place {open, front}` per vowel: A (1, 0.25), E (0.45, 0.8), I (0, 1), O (0.4, 0.05), U (0, 0).
  - `Tube {length cm, pharynx, ring}` per register: Bass (18.5, 0, 0.5), Tenor (17.5, 0, 0.4), Alto (15.5, 0.1, 0), Soprano (14.5, 0.15, 0).
- `aim` takes these inputs:
  - `voice`: 0..3, which blends adjacent tubes.
  - `vowel` and `target`: 0..4, each blending adjacent places.
  - `part`: 0..1, which blends the vowel's place toward the target's.
- `aim` computes:
  - Rn = (2n − 1) × 35 000 ÷ (4 × length).
  - F1 = R1 × (0.55 + 0.95 open).
  - F2 = R2 × (0.55 + 0.95 front) × (1 + pharynx × front).
  - F3 = R3 × (0.95 + 0.2 front); F4 = R4; F5 = R5.
  - F3–F5 are then drawn toward their mean by `ring`.
  - Widths are 40 Hz + 0.04 × Fn.
  - Levels are −6 dB per octave of Fn ÷ F1, so F1 sits at 0 dB.
- `bend`: `hertz` is the sung pitch in Hz; 0 leaves the tract as it is. F1 is raised to at least 1.15 × hertz and F2 to at least 2.3 × hertz. `effort` is 0..1 and adds 12 dB × effort to the F3–F5 levels.
- `glide`: called once per stride. `pole` is 0..1 per call; it moves `formant.tract`'s centres, widths and levels toward `tract`.
  - It re-settles the five `BAND` biquads only when a shifted centre (centre × `shift`) has moved more than `MOVED` (0.1 %) from `settled`, or when `breadth` ≠ `applied`. Each band takes Q = centre ÷ (width × breadth) and gain 10^(level/20).
  - It returns true when it re-settled. `rate` is the audio rate the bands run at; `shift` is a ratio (Sex and the per-singer fan).
- `jump`: lands on `tract` at once and re-settles.
- `makeup`: 1 ÷ √(Σ gainₙ² × π × widthₙ × breadth ÷ rate). It is the gain that leaves white noise at its own RMS.
- The standing `settle(formant, vowel, shift, rate)` now means: `aim` Tenor at `vowel` (part 0), then `jump`. It stays until the last of the three vocal orders lands and leaves no caller.

## Laws

- 379 law 7: plain structs and functions, one voice. Nothing is allocated (every array is fixed).
  - The core reads no sheet and publishes no meter.
  - The 2x stays in the rider (`SHAPER::Oversampler` around `tick`), so `Transistor` and `Variable` stay single-rate states.
- Context `SOUND::PLUGINS::CORE::FILTER`. Includes stand (`"../shaper/shaper.hpp"`); no new sibling.
- One-word names; files ≤ 100 lines, functions ≤ 25 (`aim` splits its blends into file-local helpers).
- No comments, no strings, no switch (register and vowel by table), no magic literals: 35 000, 0.55, 0.95, 0.2, 40 Hz, 0.04, −6 dB, 1.15, 2.3, 12 dB, `MOVED` and the 2π are all named.
- Nothing of `AUDIO::PLUGIN`.

## Proof

- The gate: `build.sh` green; `nm -DC … | grep -c 'SOUND::PLUGINS::CORE::FILTER::aim'` and `… 'FILTER::tick(SOUND::PLUGINS::CORE::FILTER::Transistor'` non-zero; `pytest tests` green.
- `tests/test_filter.py` over `tests/harness/filter.cpp` (with `harness('filter', 'noise')`) asserts Transistor at rate 96000:
  - emphasis 0, bass 0, drive 1, cutoff 1 kHz: DC gain of a 0.01 step is 0.98..1.02.
  - emphasis 0, cutoff 500 Hz: a 5 kHz sine at 0.01 leaves ≤ −60 dB.
  - emphasis 0.5: DC gain is 1/3 ± 0.03 with bass 0, and 0.9..1.1 with bass 1.
  - emphasis 1, cutoff 1 kHz, an impulse of 0.1: the RMS over 0.9–1.0 s is ≥ 0.01, and its zero-crossing frequency lies within 5 % of 1 kHz.
  - drive 8, a 0.9 sine: every |out| ≤ 1.05.
- It asserts Variable:
  - saturation 0 equals the five-argument settle bit for bit over 1 s of white noise;
  - LOW, 1 kHz, emphasis 0.95, a 1 kHz sine at 4: the peak at saturation 1 is below the peak at saturation 0 and ≤ 3;
  - a 200 Hz sine at 0.001: saturation 1 matches saturation 0 within 1 %.
- It asserts Tract:
  - Tenor A centres 750, 1181.25, 2900, 3500, 4100 (±0.5); widths 70, 87.25, 156, 180, 204; F2 level −3.93 dB (±0.01).
  - Tenor I: F1 275, F2 2250.
  - `aim(1.5, A, A, 0)`: F1 795.45 (±0.5).
  - `bend(Tenor I, 300, 0)`: F1 345, F2 2250.
  - `bend(…, 0, 1)`: F3–F5 levels + 12 dB, F1 unchanged.
- It asserts Formant:
  - Jump to Tenor A, then glide toward Tenor I with pole 0.5: F1 is 512.5 after one call, which returns true; the 40th call returns false.
  - Tenor A, breadth 1, rate 48000, 2 s of white noise × `makeup`: the out/in RMS lies within ±1.5 dB.
  - `settle(formant, 0, 1, 48000)` leaves the Tenor A centres.

## Learned

| Method | Source | Licence |
|---|---|---|
| nonlinear four-pole: saturation per stage, 2x, the feedback half a sample late | Huovilainen DAFx-04; VCV Fundamental `src/VCF.cpp`; sst-filters `VintageLadders.h` | copyright (paper); GPL-3.0+; GPL-3.0 (method only) |
| a two-pole state-variable with a curve in the resonance path | 2DaT Obxd `Filter.h`; Faust `vaeffects.lib` | GPL-3.0 (method only); STK-4.3 |
| limiting inside the resonance loop | Mutable Ripples manual | MIT (docs) |
| a voice type × vowel grid blended, register acting as the scale | Plaits `dsp/speech/naive_speech_synth.cc`; Braids `digital_oscillator.cc` | MIT |
| every formant parameter one-pole smoothed | ZynAddSubFX `src/DSP/FormantFilter.cpp` | GPL-2.0+ (method only) |
| autobend and vocal effort | Faust `physmodels.lib` (after chant-lib) | LGPL-2.1+ with exception (method only) |

The quarter-wave resonances of a uniform tube (Rn = (2n − 1)c ÷ 4L), the targets, the bass lift and the noise makeup are ours. No line, table or coefficient is taken.

## Riders

- `instrument/dco`: `Transistor`, bass 0 (named), at 2x, settled per stride.
- `instrument/ladder`: `Transistor` with Drive, Emphasis and Bass rows, at 2x.
- `instrument/mono`: `Transistor`, bass 0, drive 1, at 2x.
- `instrument/duo`: `Transistor` (24 dB) and `Variable` saturation (12 dB), both at 2x.
- `instrument/multimode`: `Variable` saturation at 2x.
- `modular/resonator`: `Variable` saturation through the six-argument settle in its table of four, at 2x.
- `instrument/vowel`: `aim`, `bend`, `glide`, `jump`.
- `instrument/choir`: `aim` per part, `bend`, then `glide` and `jump` per singer with its own shift.
- `instrument/whisper`: `aim` (no bend), `breadth`, `glide`, `jump`, `makeup` on the noise.
