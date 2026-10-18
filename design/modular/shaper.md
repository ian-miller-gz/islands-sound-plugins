# shaper — round 1

Gains a Bias row, the Chebyshev curve (Even, Odd), a Quality row (1x/2x/4x) with antiderivative anti-aliasing for Hard and Fold, and CV on Drive. These answer matrix lacks 121, 122, 6 (shaper's line) and 15. Later rounds: a fold amount apart from Drive, x8–x16, the Warps sum-and-product folder.

## Rows

No row index moves; new rows join at the tail. Curve gains one word at its end, so its most goes from 4 to 5.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Curve | | 0 | 5 | 0 | 5 | Soft, Hard, Tube, Fold, Crush, Chebyshev | one word added |
| 1 | Drive | dB | 0 | 48 | 12 | 0 | | |
| 2 | Bits | | 1 | 16 | 8 | 0 | | |
| 3 | Rate | Hz | 100 | 48000 | 8000 | 0 | | |
| 4 | Mix | | 0 | 1 | 1 | 0 | | |
| 5 | Bias | | -1 | 1 | 0 | 0 | | new |
| 6 | Even | | 0 | 1 | 0.5 | 0 | | new |
| 7 | Odd | | 0 | 1 | 0.5 | 0 | | new |
| 8 | Quality | | 0 | 2 | 1 | 2 | Single, Double, Quad | new |
| 9 | Amount | dB | -48 | 48 | 12 | 0 | | new |

Chain: the eurorack waveshaper with symmetry and oversampling (Befaco Chopping Kinky, Faust `cubicnl`'s offset, Grid's Chebyshev). Values chosen are ours:
- Bias is added after the drive gain and before the curve. The standing DC blocker removes what it leaves.
- Even and Odd are the Chebyshev pair's shares (`CORE::SHAPER::Chebyshev`, which stands).
- Quality Double is today's 2x. Quad nests two `Oversampler`s. Crush ignores Quality, since it lowers the rate by design.
- Hard and Fold go through the first-order antiderivative form at every Quality.
- The drive is Drive + Amount × cv in dB, held 0..48.

## Chain

| Stage | Rides |
|---|---|
| in 0 and `cv` (in 1) read at their law-4 lanes | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| gain = 10^((Drive + Amount × cv) / 20), held to the row's range, + Bias | `core/dynamics` `gain` (stands) |
| the curve by the standing table, now six | `core/shaper` `soft`, `hard`, `tube`, `fold`, `Crusher`, `Chebyshev` (stand) |
| Hard and Fold through their antiderivatives | NEEDS core/shaper — `Integral`: `{Float last; Float area;}`, `tick(integral, in, curve, area) -> Float` = (A(x) − A(x₋₁)) / (x − x₋₁), falling back to curve((x + x₋₁)/2) when the step is below a named `NEAR`. The antiderivatives are `SHAPER::AREA::hard` and `AREA::fold`. Riders: shaper now; the nonlinear stages of lack 6 (24 instruments) later |
| Quality: none, one, or two nested `Oversampler`s per lane, by a table of three paths | `core/shaper` `Oversampler` (stands) |
| DC blocker, then Mix, written in place into in 0's lanes; meter over the loudest lane | `core/shaper` `Blocker` (stands); `core/block` |

## Surface

- ins: AUDIO `in`, AUDIO `cv`, CONTROL; outs: AUDIO. An effect: it reads both ins before it writes.
- voicing: none. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::SHAPER`), law 4, law 5 (Chebyshev is the polynomial's standard name), law 6, law 7 (the antiderivative form is core/shaper's).
- 381: law 1; law 2 (`.type = "modular"`, no voicing); law 3; law 4; law 5.
- Size: `voice.settle.cpp` stands at 64 lines. The curve paths and the Quality table move to `voice.paths.cpp`. `Stage` gains `inner` (the second `Oversampler`), `chebyshev` and two `Integral`s. `shaper.hpp` moves from `sizeof` counts to `std::size`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Curve and Quality by tables, never a switch; no magic literals.

## Gate

The build waits until the library shift has landed `lane` and `Integral`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::SHAPER::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard, with an oscillator sine on a note into `in`:
  - silent before the note, then a warm, clipped sine;
  - Bias 0.5 on Soft adds an even, reedy tone;
  - Chebyshev with Odd 1 and Even 0 gives a bright third harmonic;
  - Fold at Drive 36 with Quality Quad has less fizz than Single;
  - an lfo into `cv` swings the drive.

## Learned

| Method | Source | Licence |
|---|---|---|
| first-order antiderivative anti-aliasing for clippers and folders, with a near-equal fallback | Faust `aanl.lib`; sst-waveshapers `ADAA.h` | MIT; GPL-3.0+ (method only) |
| selectable oversampling behind a steep low-pass around a folder | Befaco `src/ChoppingKinky.cpp` | GPL-3.0+ (method only) |
| an offset (bias) before a cubic curve | Faust `misceffects.lib` (`cubicnl`) | per function (STK-4.3 / MIT; method only) |
| Chebyshev shaping as a module | Bitwig Grid user guide | commercial |

Cores: core/block
