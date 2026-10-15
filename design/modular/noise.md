# noise — round 1

Gains two colours (Violet, Velvet), a Mode row (Noise, Hold, Walk), a Rate row and CV on that rate. These answer matrix lacks 116, 117, 118 and 15 (noise's line). Later rounds: grey, red and crackle, a continuous colour slope, keytracked rate (needs NOTES), Marbles-style spread and bias.

## Rows

No row index moves; new rows join at the tail. Colour gains two words at its end, so its most goes from 3 to 5.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Colour | | 0 | 5 | 0 | 5 | White, Pink, Brown, Blue, Violet, Velvet | two words added |
| 1 | Gain | | 0 | 1 | 0.25 | 0 | | |
| 2 | Gate | | 0 | 1 | 1 | 1 | | |
| 3 | Mode | | 0 | 2 | 0 | 2 | Noise, Hold, Walk | new |
| 4 | Rate | Hz | 0.1 | 20000 | 400 | 0 | | new |
| 5 | Amount | oct | -4 | 4 | 1 | 0 | | new |

Chain: the noise source with sample-and-hold and a random walk (Bogaudio Walk and S&H, Softube A-118). Values chosen are ours:
- Violet is the first difference of white.
- Velvet is one signed unit impulse at a random place in each period of 1 / Rate.
- Hold samples the chosen colour at Rate.
- Walk is a bounded random walk: at Rate, it steps by a uniform draw of up to ±0.2, is reflected at ±1, and glides one-pole over one period.
- Amount moves Rate by 2^(Amount × cv).
- Each lane keeps its own seeded sources (today's decorrelation).

## Chain

| Stage | Rides |
|---|---|
| `cv` (in 0) read at its law-4 lanes | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| rate = Rate × 2^(Amount × cv), re-settled once per stride (16 frames) | NEEDS core/block — `scaled` (see design/modular/envelope.md), `Stride` (see design/subtractive/acid.md) |
| colour per lane by the standing table of six | `core/noise` `White`, `Pink`, `Brown`, `Blue` (stand); NEEDS core/noise — `Violet` (the first difference of `White`, scaled to white's peak) and `Velvet` (`settle(velvet, hertz, rate)`, `tick(velvet)`: one signed impulse per period at a seeded place). Rider: noise |
| Mode by a table: Noise passes; Hold samples; Walk replaces | `core/noise` `Hold` (stands, first use in the family); NEEDS core/noise — `Walk`: `settle(walk, hertz, step, rate)`, `tick(walk) -> Float`, a seeded bounded walk reflected at ±1 and glided one-pole over a period. Riders: noise (the lfo's Random may ride it later) |
| × the smoothed Gain × Gate, written into in 0's lanes; meter over the loudest lane | `core/modulator` `Smoother` (stands); `core/block` |

## Surface

- ins: AUDIO `cv`, CONTROL; outs: AUDIO. It reads `cv` before it writes in 0's lanes, so it no longer declares itself a source (379 law 6: the in is real and read). An unwired `cv` reads 0, so it sounds at once as today.
- voicing: none. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::NOISE`), law 4, law 5, law 6, law 7 (the colours and the walk are core/noise's).
- 381: law 1; law 2 (`.type = "modular"`, no voicing); law 3; law 4; law 5.
- Size: `voice.render.cpp` stands at 55 lines. The Mode table goes in `voice.modes.cpp`. `Source` gains `violet`, `velvet`, `hold` and `walk`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Colour and Mode by tables; no magic literals (0.2, 16 named). `noise.hpp` moves from `sizeof` counts to `std::size`, as its siblings use.

## Gate

The build waits until the library shift has landed `lane`, `scaled`, `Stride`, `Violet`, `Velvet` and `Walk`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::NOISE::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard:
  - hiss at once (a source of noise; there is no note);
  - Violet is thinner than Blue;
  - Velvet at Rate 400 is a crackle;
  - Hold at Rate 400 is stepped, bit-crushed hiss;
  - Walk at Rate 5 into resonator `cv` is a wandering sweep;
  - an lfo into `cv` makes Hold's rate swing.

## Learned

| Method | Source | Licence |
|---|---|---|
| pink by Voss rows; blue and violet by first differences; red by stacking | VCV Fundamental `src/Noise.cpp`; Bogaudio `src/dsp/noise.hpp`; SuperCollider `NoiseUGens.cpp` | GPL-3.0+; GPL-3.0+; GPL-2.0+ (method only) |
| velvet noise as sparse signed impulses; noise of any slope | Faust `noises.lib` (velvet, colored_noise) | per function (STK-4.3 / LGPL; method only) |
| a bounded random walk; sample-and-hold over noise | Bogaudio Walk and S&H; Softube Modular manual (A-118) | GPL-3.0+ (method only); commercial manual |
| brown as a bounded walk | Geonkick `oscillator.c` | GPL-3.0+ (method only) |

Cores: core/block
