# clave — round 1

Gains a Tone row over the rim's crack, a rim tune apart from the bar's, the bar's upper modes with velocity reaching them, a velocity curve, and chromatic play — matrix lacks 22, 4, 27 (the bar's modes), 23. Later rounds: a stick model's excitation filter, stereo (5), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | Hz | 1000 | 5000 | 2500 | 0 | | |
| 1 | Decay | s | 0.01 | 0.5 | 0.06 | 0 | | |
| 2 | Level | | 0 | 1 | 0.8 | 0 | | |
| 3 | Tone | Hz | 100 | 3000 | 300 | 0 | | new |
| 4 | Rim | st | -12 | 12 | 0 | 0 | | new |
| 5 | Ring | | 0 | 1 | 0.25 | 0 | | new |
| 6 | Curve | | -1 | 1 | 0 | 0 | | new |
| 7 | Touch | | 0 | 1 | 0.5 | 0 | | new |
| 8 | Key | | 0 | 1 | 0 | 1 | Off, On | new |

Chain: the damped bridged-T strike (a bar resonator for the clave, two short resonators and a high-passed crack for the rim, by note 37). Tone is the crack's high-pass (today's constant 300 Hz); Rim shifts both rim resonators by 2^(Rim/12) about today's ratios; Ring is the share of two upper bar modes at the free bar's ratios (2.756 and 5.404, from beam physics, not from any tree), each decaying faster by its ratio; Touch weighs Ring by velocity, Ring × `ENVELOPE::weigh(v, Touch)`; Key On plays every note as the bar at Tune × 2^((note − 60)/12) (no rim then).

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve | NEEDS core/percussion — `struck(velocity, curve)` (see design/drums/clap.md) |
| note → stroke (37 rim, else clave), or Key → the bar re-settled at the keyed tune before the strike | `core/percussion` `keyed`, `core/phase` `ratio` (stand) |
| bar and its two upper modes; rims at Rim; crack high-pass at Tone | `core/percussion` `Clave`, `Resonator` (this order adds the modes, the rim shift and the crack's cutoff to `percussion.clave.*`; the standing `settle(clave, tune, decay, rate)` keeps today's sound for kit) |
| Ring by velocity | `core/envelope` `weigh` (stands) |
| soft clip, Level, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::CLAVE`, 4 the sheet (rows appended to `clave.hpp`), 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/clave/` and `core/percussion/percussion.clave.*` only.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (60, the two mode ratios named).

## Gate

- After the library shift has landed `struck(velocity, curve)`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::CLAVE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a clave on 75 and a rim on 37; Ring 1 woody and bright, a soft hit duller than a hard one; Key On walks a scale with the keys.

## Learned

| Method | Source | Licence |
|---|---|---|
| one resonator per mode at a ratio and radius; strike strength shapes the excitation | STK `src/Modal.cpp`, `src/ModalBar.cpp` | STK (MIT-like) |
| a mode bank whose higher modes decay sooner | Faust `physmodels.lib` (`strikeModel`) | LGPL-2.1+ with exception (method only) |
| chromatic play about a root | Plaits V/oct; RipplerX keyed | MIT; GPL-3.0 (method only) |

Cores: core/percussion
