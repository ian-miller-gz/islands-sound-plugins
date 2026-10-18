# mixer — round 1

Gains a mute per in, attenuverting levels, a Taper row, the equal-power pan and a `cv` on the sum. These answer matrix lacks 115, 42, 5 (mixer's line) and 15. Later rounds: CV per in (four more AUDIO ins), solo, and polyphonic cables (S3).

## Rows

No row index moves; new rows join at the tail. One standing range widens: Level1..4 least goes from 0 to −1. The rest stays 1.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0–3 | Level1..Level4 | | -1 | 1 | 1 | 0 | | least was 0 |
| 4–7 | Pan1..Pan4 | | -1 | 1 | 0 | 0 | | |
| 8 | Gain | | 0 | 2 | 1 | 0 | | |
| 9–12 | Mute1..Mute4 | | 0 | 1 | 0 | 1 | Off, On | new |
| 13 | Taper | | 0 | 1 | 1 | 1 | Linear, Audio | new |
| 14 | Amount | | -1 | 1 | 1 | 0 | | new |

Chain: the eurorack mixer with mutes, attenuverters and pans (Shades, Bogaudio Mix4). Values chosen are ours:
- A negative level inverts its in.
- Taper Audio is the 60 dB audio taper of each level. It rests on Audio, so a half level now sits well below half (the demo hears the taper).
- The sum is multiplied by max(0, 1 + Amount × cv). An unwired `cv` keeps today's sum.
- Each in's weight glides over 5 ms, so a Mute or a Level turn does not click.
- With one lane, pan is ignored, as today.

## Chain

| Stage | Rides |
|---|---|
| ins 0–3 and `cv` (in 4) read at their law-4 lanes | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| level by Taper, × (1 − Mute) | NEEDS core/dynamics — `taper` (see design/modular/amplifier.md) |
| pan to a left/right pair | NEEDS core/stereo — the equal-power pan law (see design/subtractive/pads.md); a centred in keeps today's level on each lane |
| weights glided per sample | `core/modulator` `Smoother` (stands) |
| sum × Gain × max(0, 1 + Amount × cv), written into in 0's lanes; meter over the loudest lane | `core/block` |

## Surface

- ins: AUDIO `1`, `2`, `3`, `4`, AUDIO `cv`, CONTROL; outs: AUDIO. An effect: it reads every in before it writes in 0's lanes.
- voicing: none. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::MIXER`), law 4 (the four Mute rows as sheet literals Mute1..Mute4), law 5, law 6, law 7 (the taper and the pan law are cores').
- 381: law 1; law 2 (`.type = "modular"`, no voicing); law 3; law 4 (five lane sets in port order); law 5.
- Size: `mixer.hpp` stands at 34 lines and gains 6 rows. The smoothers go on the `Module`; the weight settle stays in `voice.settle.cpp`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Taper by a table of two functions; no magic literals (60, 5 ms named).

## Gate

The build waits until the library shift has landed `lane`, `taper` and core/stereo's pan law.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::MIXER::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard, with an oscillator into `1` and noise into `2`:
  - both summed at rest;
  - Mute2 On removes the noise without a click;
  - Level1 −1 against a second copy of the oscillator at +1 cancels it;
  - Pan1 −1 puts the oscillator on the left at full level;
  - an lfo into `cv` gives a tremolo on the sum.

## Learned

| Method | Source | Licence |
|---|---|---|
| equal-power pan from quarter-offset sine gains | Bogaudio `src/dsp/signal.cpp` | GPL-3.0+ (method only) |
| attenuverting levels; mutes per channel | Mutable Shades manual; Bogaudio Mix4 | MIT (docs); GPL-3.0+ (method only) |
| a dB taper with a linear ramp near silence | Bogaudio `src/dsp/signal.cpp`; Veils manual | GPL-3.0+ (method only); MIT (docs) |
| CV on levels | VCV Fundamental `src/VCMixer.cpp` | GPL-3.0+ (method only) |

Cores: core/block, core/stereo
