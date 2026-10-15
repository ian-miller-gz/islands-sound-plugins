# resonator — round 1

Gains a Model row riding the filters that stand (Variable, Ladder, Diode, Sallen) and a Drive row with saturation in the loop, run at 2x. The cutoff stride shortens. These answer matrix lacks 16, 20, 6 (resonator's line) and 15 (the cutoff CV read finer). Later rounds: linear FM and a per-sample cutoff for audio-rate sweeps, keytracking (needs NOTES), and polyphonic cables (S3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Mode | | 0 | 3 | 0 | 3 | Low, Band, High, Notch | |
| 1 | Cutoff | Hz | 20 | 20000 | 1000 | 0 | | |
| 2 | Emphasis | | 0 | 1 | 0.3 | 0 | | |
| 3 | Rate | Hz | 0.01 | 20 | 0.5 | 0 | | |
| 4 | Sweep | oct | 0 | 4 | 0.5 | 0 | | |
| 5 | Depth | oct | -10 | 10 | 0 | 0 | | |
| 6 | Model | | 0 | 3 | 0 | 3 | Variable, Ladder, Diode, Sallen | new |
| 7 | Drive | | 0 | 1 | 0.25 | 0 | | new |

Chain: the eurorack VCF with model choice and nonlinear feedback (Ripples LP2/LP4/BP from one core, Fundamental VCF's saturating ladder, Voltage's ladder and SVF). Values chosen are ours:
- Mode applies in full to Variable.
- Sallen takes Low and High; Band and Notch fall back to Low there.
- Ladder (24 dB) and Diode (18 dB) are low-pass only.
- Drive is the Variable's saturation 0..1, and the drive gain 1 + 7 × Drive for the other three.
- Every model runs at twice the rate inside the `Oversampler`.
- The rest of Drive 0.25 adds a little grit to today's sound. Drive 0 on Variable keeps today's linear loop.
- The cutoff re-settles every 4 frames, where it was 16.

## Chain

| Stage | Rides |
|---|---|
| in 0 and `cv` (in 1) read at their law-4 lanes | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| cutoff = Cutoff × 2^(LFO × Sweep + Depth × cv), held 20 Hz..20 kHz, once per stride (4 frames) | NEEDS core/block — `scaled` (see design/modular/envelope.md), `Stride` (see design/subtractive/acid.md); `core/modulator` `Lfo` (stands) |
| the model per lane by a table of four (settle, tick) pairs, settled at 2 × rate | `core/filter` `Ladder`, `Diode`, `Sallen` (stand); NEEDS core/filter — `Variable` `saturation` (see design/subtractive/duo.md; riders duo, multimode, resonator) |
| twice the rate: up, two ticks, down | `core/shaper` `Oversampler` `up`/`down` (stand) |
| written in place into in 0's lanes; meter over the loudest lane | `core/block` |

## Surface

- ins: AUDIO `in`, AUDIO `cv`, CONTROL; outs: AUDIO. An effect: it reads both ins before it writes.
- voicing: none. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::RESONATOR`), law 4, law 5 (model words are the circuits' own topology names, no maker's), law 6, law 7.
- 381: law 1; law 2 (`.type = "modular"`, no voicing); law 3; law 4; law 5.
- Size: per lane, a `Stage` holds the four filters and an `Oversampler` (a `Vector<Stage>` replaces `filters`). The model table goes in `voice.models.cpp`. `resonator.hpp` moves from `sizeof` counts to `std::size`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Model by a table, never a switch; no magic literals (7, 4 named).

## Gate

The build waits until the library shift has landed `lane`, `scaled`, `Stride` and `Variable` `saturation`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::RESONATOR::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard, with an oscillator saw on a note into `in`:
  - silent before the note, then a filtered saw that sweeps slowly;
  - Model Ladder is rounder and steeper than Variable;
  - Diode at Emphasis 0.9 squelches;
  - Drive 1 growls and holds the peak without blowing up;
  - an envelope into `cv` at Depth 4 gives a pluck.

## Learned

| Method | Source | Licence |
|---|---|---|
| ladder of four saturating one-poles, 2x, a whisper of noise to self-oscillate | VCV Fundamental `src/VCF.cpp`; Huovilainen DAFx-04 | GPL-3.0+ (method only); paper |
| OTA soft-limiting inside the resonance loop; LP2/LP4/BP from one core | Mutable Ripples manual | MIT (docs) |
| model choice in one filter module | Bitwig Grid user guide; Surge XT Rack manual (VCF) | commercial; GPL-3.0+ (docs) |

Cores: core/block, core/filter
