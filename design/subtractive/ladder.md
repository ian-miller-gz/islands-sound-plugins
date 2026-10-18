# ladder — round 1

Gains the nonlinear ladder (saturation per stage, 2x) with bass compensation, velocity to level and cutoff, drift, control-rate settle — matrix lacks 20, 47, 6, 4, 9, 14. Later rounds: the modulation page (lack 11, a `core/matrix` first).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Bend | st | -7 | 7 | 0 | 0 | | |
| 2 | Glide | s | 0 | 3 | 0 | 0 | | |
| 3 | Modulation | | 0 | 1 | 0 | 0 | | |
| 4 | Mix | | 0 | 1 | 0 | 0 | | |
| 5 | Pitch | | 0 | 1 | 1 | 1 | | |
| 6 | Filter | | 0 | 1 | 0 | 1 | | |
| 7 | Range1 | | 0 | 5 | 3 | 5 | Lo, 32, 16, 8, 4, 2 | |
| 8 | Wave1 | | 0 | 5 | 2 | 5 | Triangle, Sharktooth, Saw, Square, Wide, Narrow | |
| 9 | Level1 | | 0 | 1 | 0.7 | 0 | | |
| 10 | Range2 | | 0 | 5 | 3 | 5 | Lo, 32, 16, 8, 4, 2 | |
| 11 | Wave2 | | 0 | 5 | 2 | 5 | as Wave1 | |
| 12 | Frequency2 | st | -7 | 7 | 0.1 | 0 | | |
| 13 | Level2 | | 0 | 1 | 0.7 | 0 | | |
| 14 | Range3 | | 0 | 5 | 0 | 5 | Lo, 32, 16, 8, 4, 2 | |
| 15 | Wave3 | | 0 | 5 | 0 | 5 | Triangle, Sharktooth, Reverse, Saw, Square, Wide | |
| 16 | Frequency3 | st | -7 | 7 | 0 | 0 | | |
| 17 | Level3 | | 0 | 1 | 0 | 0 | | |
| 18 | Keyboard | | 0 | 1 | 0 | 1 | | |
| 19 | Noise | | 0 | 1 | 0 | 0 | | |
| 20 | Colour | | 0 | 1 | 0 | 1 | White, Pink | |
| 21 | Drive | | 1 | 4 | 1 | 0 | | |
| 22 | Cutoff | Hz | 20 | 20000 | 1000 | 0 | | |
| 23 | Emphasis | | 0 | 1 | 0.3 | 0 | | |
| 24 | Amount | oct | 0 | 5 | 2 | 0 | | |
| 25 | Tracking | | 0 | 3 | 1 | 3 | Off, 1/3, 2/3, Full | |
| 26 | Sweep | s | 0.001 | 10 | 0.01 | 0 | | |
| 27 | Close | s | 0.001 | 10 | 0.4 | 0 | | |
| 28 | Floor | | 0 | 1 | 0.2 | 0 | | |
| 29 | Attack | s | 0.001 | 10 | 0.005 | 0 | | |
| 30 | Decay | s | 0.001 | 10 | 0.3 | 0 | | |
| 31 | Sustain | | 0 | 1 | 0.8 | 0 | | |
| 32 | Release | | 0 | 1 | 1 | 1 | | |
| 33 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 34 | Bass | | 0 | 1 | 0.5 | 0 | | new |
| 35 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 36 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 37 | Drift | ct | 0 | 20 | 3 | 0 | | new |

Chain: the three-oscillator monosynth's public panel (Range Lo–2', six waves, keyboard tracking 1/3–2/3, Drive as the mixer's overload, low-note priority, single trigger). Bass, Velocity, Touch and Drift are ours. Bass 0 is the bare ladder's thinning as Emphasis rises; 1 restores the passband.

## Chain

| Stage | Rides |
|---|---|
| note: monophonic, low priority, legato, Glide | `core/voice` |
| three VCOs (osc 3 also the modulation source), noise white/pink, the mixer | `core/oscillator`, `core/noise` |
| slow pitch and cutoff wander of the one voice, Drift cents deep (cutoff ±Drift/100 oct — ours) | NEEDS core/modulator — `Drift` (see dco) |
| filter contour ADS (Sweep, Close, Floor), loudness ADS (Release switch); loudness depth = Velocity | `core/envelope` |
| VCF: Cutoff × Tracking × Amount·contour × wheel × Touch·velocity; Drive, Emphasis and Bass into the cascade, settled once per stride | NEEDS core/filter — `Transistor` (see dco); NEEDS core/block — `Stride` (see acid) |
| DC blocker, Volume | `core/shaper` `Blocker` |

`FILTER::Ladder` leaves this instrument; it stays in core/ for its other riders.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::LADDER`, 4 the sheet (rows appended to `ladder.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::LADDER::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a note sounds; at Emphasis 0.8 the bottom holds with Bass 1 and thins with Bass 0; Drive 4 growls, no aliasing whine on a high note; a hard note is louder and brighter.

## Learned

| Method | Source | Licence |
|---|---|---|
| nonlinear ladder: saturation per stage, 2x, half-sample feedback delay | Huovilainen DAFx-04; VCV Fundamental `src/VCF.cpp`; sst-filters `VintageLadders.h` | copyright; GPL-3.0; GPL-3.0 (method only) |
| bass compensation as emphasis rises | the V Collection's Mini V (manual); Zavalishin, *The Art of VA Filter Design* | commercial manual; copyright |
| Moog VCF analysis (the loop's tuning) | Stilson & Smith ICMC 1996 | copyright |
| drift as a random walk per voice | sst-basic-blocks `OscillatorDriftUnisonCharacter.h`; Odin 2 `DriftGenerator.cpp` | GPL-3.0 (method only) |
| coefficients at control rate | stmlib `dsp/parameter_interpolator.h` | MIT |

Cores: core/block, core/filter, core/modulator
