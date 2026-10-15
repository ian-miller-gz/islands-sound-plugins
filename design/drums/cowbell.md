# cowbell — round 1

Gains a Tone row over the band, the two squares' interval as a row, the clank's share with velocity reaching it, a velocity curve, and chromatic play — matrix lacks 22, 4, 23. Later rounds: band-limited squares (core/oscillator `Edges`, named by chip), stereo (5), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | st | -12 | 12 | 0 | 0 | | |
| 1 | Decay | s | 0.05 | 2 | 0.35 | 0 | | |
| 2 | Level | | 0 | 1 | 0.8 | 0 | | |
| 3 | Tone | Hz | 800 | 6000 | 2640 | 0 | | new |
| 4 | Interval | st | 3 | 12 | 6.8 | 0 | | new |
| 5 | Clank | | 0 | 1 | 0.6 | 0 | | new |
| 6 | Curve | | -1 | 1 | 0 | 0 | | new |
| 7 | Touch | | 0 | 1 | 0.5 | 0 | | new |
| 8 | Key | | 0 | 1 | 0 | 1 | Off, On | new |

Chain: the analogue cowbell (two squares, the 540 Hz and 800 Hz pair of the circuit's public schematic, into a band, a sharp clank over a longer ring). Tone is the band centre (today's constant 2640 Hz); Interval sets the upper square at the lower × 2^(Interval/12) (6.8 st is today's pair); Clank is the attack's share (today's 0.6); Touch weighs Clank by velocity, `ENVELOPE::weigh(v, Touch)`; Key On adds note − 60 semitones to Tune.

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve | NEEDS core/percussion — `struck(velocity, curve)` (see design/drums/clap.md) |
| two squares at Tune (+ keyed), the second at Interval | `core/percussion` `Metal` (stands); `Cowbell` takes its own pair in `percussion.cowbell.*`, the standing `settle(cowbell, tune, decay, rate)` keeps today's pair and band for kit |
| band at Tone | `core/filter` `Biquad` BAND (stands) |
| clank × weigh(v, Touch) + ring | `core/envelope` `weigh` (stands) |
| soft clip, Level, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::COWBELL`, 4 the sheet, 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/cowbell/` and `core/percussion/percussion.cowbell.*` only.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (60 named).

## Gate

- After the library shift has landed `struck(velocity, curve)`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::COWBELL::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, the bell on every note; Interval 12 hollow and octave-like; Tone low muffles it; soft hits carry less clank; Key On plays the keys.

## Learned

| Method | Source | Licence |
|---|---|---|
| the cowbell circuit: two squares, a band, a two-part envelope | Werner, Abel, Smith, AES 137 (citation page); Sound On Sound, Synth Secrets (summaries) | paper; commercial |
| the circuit's own tuning trimmers as rows | Roland Cloud TR-808 (manual) | commercial manual |
| velocity through an adjustable curve | drumkv1 `src/drumkv1.cpp` | GPL-2.0+ (method only) |

Cores: core/percussion
