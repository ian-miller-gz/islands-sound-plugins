# core/modulator — round 1

A seeded slow drift per voice, stated by eight instruments: dco, duo, ladder, mono, multimode, polymod, choir and vowel (matrix shared 9). An existing core grows: it gains `Drift`. `Lfo`, `Slew`, `Smoother`, `delta`, `pole` and `draw` stay as they are.

`Lfo` `skew` (lfo) and `Slew` `shape` (slew) have one rider each. They stay in those requirements, for the riders' own shifts to write.

Sequence: third of eight.

## Interface

`modulator.hpp` gains the following (≤ 75 lines in all), with a new unit `modulator.drift.cpp`:

```cpp
constexpr Float WANDER = 0.57735027f;

struct Drift {
  Float leak = 0;
  Float gain = 0;
  Float value = 0;
  Seed seed = SEED;
};

void seed(Drift &drift, Seed state);
void settle(Drift &drift, Float seconds, Whole rate);
auto tick(Drift &drift) -> Float;
```

- `seed`: sets the walk's register (0 reads as `SEED`, as `draw` does). Riders seed per voice slot or per singer seat, once at create.
- `settle`:
  - `seconds` is the walk's time constant, 0.001..10 s; `rate` is the rate `tick` is called at, in Hz (the sample rate, or the sample rate ÷ the rider's stride).
  - `leak` = 1 − `pole(seconds, rate)`; `gain` = `WANDER` × √(1 − leak²).
- `tick`: `value = leak × value + gain × draw(seed)`, held to −1..1. White noise through a leaky one-pole: at rest its RMS is about 1/3, so the hold is rarely reached. The rider scales it (dco: Drift cents; mono and ladder: ±Drift/100 oct on the cutoff).

## Laws

- 379 law 7: one plain struct and plain functions, one voice. Nothing is allocated. The core reads no sheet and publishes no meter.
- Context `SOUND::PLUGINS::CORE::MODULATOR`. Includes stand (`"../phase/phase.hpp"`).
- One-word names; files ≤ 100 lines, functions ≤ 25.
- No comments, no strings, no switch, no magic literals (`WANDER` = 1/√3, the stationary RMS of uniform noise, named).
- Nothing of `AUDIO::PLUGIN`.

## Proof

- The gate: `build.sh` green; `nm -DC … | grep -c 'SOUND::PLUGINS::CORE::MODULATOR::tick(SOUND::PLUGINS::CORE::MODULATOR::Drift'` non-zero; `pytest tests` green.
- `tests/test_modulator.py` over `tests/harness/modulator.cpp` asserts, with seconds 0.4 and rate 3000 (48 kHz ÷ 16), over 60 s:
  - every |value| ≤ 1;
  - the RMS lies within 0.25..0.42;
  - the autocorrelation at lag 120 (seconds × rate ÷ 10) is ≥ 0.8 (slow);
  - seed 1 twice gives identical sequences, and seeds 1 and 2 differ at the first tick.

## Learned

| Method | Source | Licence |
|---|---|---|
| drift as a random walk: noise through a leaky one-pole per voice | sst-basic-blocks `OscillatorDriftUnisonCharacter.h`; Odin 2 `DriftGenerator.cpp` | GPL-3.0 (method only) |
| pitch wobble as slow smooth noises at unrelated rates | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) |

The normalisation to a stationary RMS of 1/3 is ours. No line is taken.

## Riders

- `instrument/dco`, `instrument/polymod`, `instrument/multimode`: one per voice slot, Drift cents on pitch.
- `instrument/duo`: one per part.
- `instrument/ladder`, `instrument/mono`: one for the voice, on pitch and on the cutoff (±Drift/100 oct).
- `instrument/vowel`: two per voice (wobble 0.4 s, jitter 0.01 s), seeded per slot, ticked per stride.
- `instrument/choir`: two per singer, seeded per seat, ticked per stride.
