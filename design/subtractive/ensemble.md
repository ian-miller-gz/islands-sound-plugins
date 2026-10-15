# ensemble — round 1

Gains a polyphonic envelope beside the paraphonic one, velocity to key level, and a resonator bank after the sections — matrix lacks 53, 4, 52 (the resonator; the ensemble mode switch a later round). Later rounds: brass and horn sections (lack 54).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Violin | | 0 | 1 | 0.8 | 0 | | |
| 1 | Viola | | 0 | 1 | 0.5 | 0 | | |
| 2 | Cello | | 0 | 1 | 0.4 | 0 | | |
| 3 | Contrabass | | 0 | 1 | 0 | 0 | | |
| 4 | Attack | s | 0.005 | 5 | 0.15 | 0 | | |
| 5 | Release | s | 0.005 | 10 | 1.2 | 0 | | |
| 6 | Mix | | 0 | 1 | 1 | 0 | | |
| 7 | Depth | | 0 | 1 | 1 | 0 | | |
| 8 | Chorus | Hz | 0.05 | 2 | 0.6 | 0 | | |
| 9 | Vibrato | Hz | 3 | 10 | 6 | 0 | | |
| 10 | Tune | ct | -100 | 100 | 0 | 0 | | |
| 11 | Bend | st | -12 | 12 | 0 | 0 | | |
| 12 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 13 | Envelope | | 0 | 1 | 0 | 1 | Paraphonic, Polyphonic | new |
| 14 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 15 | Formant | | 0 | 1 | 0.3 | 0 | | new |

Chain: the string machine (divide-down saws per key, four registers, one shared AR, the three-line ensemble on two LFOs). Envelope Paraphonic is today's shared swell; Polyphonic gives every key its own AR over the same Attack and Release. Formant is the wet of three parallel peak bands (450 Hz, 1.1 kHz, 2.6 kHz, +9 dB, Q 3 — ours) mixed before the ensemble lines.

## Chain

| Stage | Rides |
|---|---|
| one master clock divided per key, 8' and 16' saws | `core/oscillator` (`saw` on the divided phase) |
| key level: Paraphonic — the key ramp × the shared AR; Polyphonic — a gate per key (128, held on the instance) over the shared AR shape; either way the key's level × `weigh(velocity, Velocity)` | `core/envelope` (`Gate`, `weigh` stand) |
| the four registers' low-passes, the panel mix | `core/filter` `Biquad` |
| the resonator: three `PEAK` bands in parallel, blended by Formant | `core/filter` `Biquad` |
| the ensemble: three swept reads, slow and fast LFOs, bucket low-pass | `core/line`, `core/modulator` (this instrument's own `voice.ensemble.cpp` stays: its two-LFO sweep is the chain's, not the chorus core's) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; left, right, mono sum as today.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree (a dotted sibling `voice.formant.cpp`), 2 includes, 3 `SOUND::PLUGINS::ENSEMBLE`, 4 the sheet (indices in `ensemble.hpp` grow by three), 5 names, 6 honesty, 7 cores.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; the envelope mode is a table of two key-level reads, never a switch; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::ENSEMBLE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a chord swells in; Polyphonic: a key struck while others hold swells in by itself (Paraphonic: it joins at the held level); soft keys quieter; Formant 1 adds a vocal, nasal colour.

## Learned

| Method | Source | Licence |
|---|---|---|
| the polyphonic envelope option, the ensemble modes, the resonator bank after the sections | the V Collection's Solina V (product page) | commercial page |
| three-phase bucket-brigade ensemble | Holters & Parker DAFx-18; KVR practitioner posts (three-phase BBD) | copyright; none stated (method only) |
