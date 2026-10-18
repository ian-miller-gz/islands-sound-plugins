# clap — round 1

Gains a burst count, scattered burst spacing, a tail tone of its own, a resonant low-pass after the band, and velocity through a curve to level and spread — matrix lacks 80, 79, 81, 4. Later rounds: a noise-colour row (81's rest), stereo (5), oversampled clip (6), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Spread | s | 0.004 | 0.03 | 0.011 | 0 | | |
| 1 | Decay | s | 0.05 | 2 | 0.3 | 0 | | |
| 2 | Tone | Hz | 400 | 5000 | 1100 | 0 | | |
| 3 | Level | | 0 | 1 | 0.8 | 0 | | |
| 4 | Bursts | | 1 | 8 | 4 | 7 | | new |
| 5 | Scatter | | 0 | 1 | 0.3 | 0 | | new |
| 6 | Tail | Hz | 400 | 5000 | 900 | 0 | | new |
| 7 | Cutoff | Hz | 1000 | 16000 | 7000 | 0 | | new |
| 8 | Emphasis | | 0 | 1 | 0.3 | 0 | | new |
| 9 | Curve | | -1 | 1 | 0 | 0 | | new |
| 10 | Touch | | 0 | 2 | 0.5 | 0 | | new |

Chain: the analogue clap (white noise through a band under four staggered bursts and a longer tail). Bursts 4 is today's constant; Scatter moves each gap by ±Scatter × 0.5 of Spread, drawn from the clap's own noise at each burst (ours); Tail is the tail's own band centre (today it shares Tone); Cutoff and Emphasis are a state-variable low-pass after both bands (Emphasis 0..1 to the core's resonance range); Curve shapes velocity before every use; Touch loosens soft claps: gap × 2^(Touch × (1 − v)), so a full hit keeps Spread.

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve | NEEDS core/percussion — `struck(velocity, curve)`: clamp to 0..1, then raise to 4^−curve (0 linear, +1 a light touch, −1 a heavy one; tine's rule); the one-argument `struck` stands as curve 0. And `touched(hertz, velocity, touch)` = hertz × 2^(touch × (velocity − 1)): a full hit keeps the row's value, a negative touch raises it for soft hits. Riders: all thirteen drums (pads later) |
| bursts: Bursts strikes Spread apart, each gap scattered and touched | `core/percussion` `Clap` (this order adds the count, the scatter and the tail band to `percussion.clap.*`; the standing `settle(clap, spread, decay, tone, rate)` keeps today's four bursts, no scatter and one band, so kit and machine sound as today) |
| bursts through the band at Tone, the tail through its own band at Tail | `core/filter` `Biquad` BAND (stands) |
| the sum through the low-pass at Cutoff, Emphasis | `core/filter` `Variable` LOW (stands) |
| soft clip, Level, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; one sample to every lane as today.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::CLAP` and `SOUND::PLUGINS::CORE::PERCUSSION`, 4 the sheet (rows appended to `clap.hpp`), 5 names, 6 honesty, 7 cores (the burst logic stays in the core's clap unit; the instrument only settles and strikes).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- This shift edits `instrument/clap/` and `core/percussion/percussion.clap.*` (plus dotted siblings) only; `percussion.circuit.hpp` and `percussion.cpp` are the library's.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (the 0.5 scatter span named).

## Gate

- After the library shift has landed `struck(velocity, curve)` and `touched`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::CLAP::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a clap on every note; Bursts 1 is a single slap, 8 a ragged crowd; Scatter 1 audibly uneven; a darker tail than the bursts; a soft note looser than a hard one.

## Learned

| Method | Source | Licence |
|---|---|---|
| noise under staggered envelopes, a resonant low-pass, a soft clip | Faust `synths.lib` (`sy.clap`) | LGPL-2.1+ with exception (method only) |
| a randomised burst spacing; a tail apart from the bursts | Ableton DS Clap (manual); the analogue clap's reverb tail (TR-808 manual) | commercial pages |
| velocity through an adjustable curve | drumkv1 `src/drumkv1.cpp`; DrumGizmo `src/powermap.cc` | GPL-2.0+; LGPL-3.0+ (method only) |

Cores: core/percussion
