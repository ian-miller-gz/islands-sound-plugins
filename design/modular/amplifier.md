# amplifier — round 1

Gains a Response row (linear to exponential) and a Mode row (Clamp, Ring, Lowpass). These answer matrix lacks 42, 106 and 107, and move the module onto the shared CV read (15). Later rounds: a bleed row for the low-pass gate, polyphonic cables (S3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Offset | | 0 | 1 | 0 | 0 | | |
| 1 | Depth | | -1 | 1 | 1 | 0 | | |
| 2 | Gain | | 0 | 2 | 1 | 0 | | |
| 3 | Response | | 0 | 1 | 0.5 | 0 | | new |
| 4 | Mode | | 0 | 2 | 0 | 2 | Clamp, Ring, Lowpass | new |

Chain: the VCA with a linear-to-exponential response knob (Veils), a four-quadrant mode (Blinds, Befaco A*B+C), and the low-pass gate (one control opens level and brightness together; Plaits LPG). Values chosen are ours:
- The control is c = Offset + Depth × cv, as today.
- Clamp holds c ≥ 0 (today's two-quadrant). Ring passes a negative c, so a negative control inverts the signal. Lowpass is Clamp plus a two-pole low-pass whose cutoff is 40 Hz × 500^c (40 Hz to 20 kHz).
- Response morphs the gain from c to the audio taper of c over a 60 dB span. Response 0 is today's linear sound. The rest of 0.5 is half-way, so the release closes faster than today.
- Offset stays 0 at rest: a VCA with no CV is closed. That is the module's job in an envelope patch (see the notes).

## Chain

| Stage | Rides |
|---|---|
| in 0 and `cv` (in 1) read at their law-4 lanes | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| c = Offset + Depth × cv; Mode by a table of three functions | in the module |
| Response: c + Response × (taper(c, 60) − c), with the sign kept for Ring | NEEDS core/dynamics — `taper(level, span) -> Float`: level × `gain(span × (\|level\| − 1))`, the sign of level kept, span in dB. 0 stays 0 and ±1 stays ±1. Riders: amplifier, mixer |
| × Gain on every lane | in the module |
| Lowpass: a `Variable` LOW per lane, emphasis 0, re-settled once per stride (16 frames) from c | `core/filter` `Variable` (stands); NEEDS core/block — `Stride` (see design/subtractive/acid.md) |
| meter over the loudest lane | `core/block` |

## Surface

- ins: AUDIO `in`, AUDIO `cv`, CONTROL; outs: AUDIO. An effect: it reads both ins before it writes in 0's lanes.
- voicing: none (no NOTES in). Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::AMPLIFIER`), law 4, law 5 (Ring and Lowpass are the positions' own words), law 6, law 7.
- 381: law 1; law 2 (`.type = "modular"`, no voicing); law 3; law 4 (in 0 in place, `cv` read-only); law 5.
- Size: the Lowpass state (a `Vector<CORE::FILTER::Variable>` and a `Stride`) goes on the `Module`. The mode functions go in `voice.modes.cpp`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Mode by a table, never a switch; no magic literals (40, 500, 60, 16 named).

## Gate

The build waits until the library shift has landed `lane`, `taper` and `Stride`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::AMPLIFIER::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard, in the patch oscillator → `in` with envelope → `cv`:
  - silent before the note, sounding on the note, and silent after the release;
  - Response 1 makes the tail snappier than Response 0;
  - Ring with an lfo at Offset 0 makes the tremolo pass through zero and invert;
  - Lowpass darkens as the note closes.

## Learned

| Method | Source | Licence |
|---|---|---|
| a lin↔exp response morph; a dB taper with a linear ramp near silence; a blend of linear and cubed | Veils manual; Bogaudio `src/dsp/signal.cpp`; surge-rack `src/EGxVCA.h` | MIT (docs); GPL-3.0+ (method only); GPL-3.0+ (method only) |
| low-pass gate: one control on a gain stage and an SVF low-pass | Mutable `plaits/dsp/fx/low_pass_gate.h` | MIT |
| four-quadrant multiply; a diode ring | Mutable Blinds manual; `warps/dsp/modulator.cc`; Befaco A*B+C | MIT; MIT; GPL-3.0+ (method only) |

Cores: core/block
