# kick — round 1

Gains the pitch envelope as rows, a Click kind, the circuit's sigh, an oversampled drive, a post-drive EQ, velocity through a curve to the tone, and chromatic play — matrix lacks 13, 84, 85, 6, 86, 4, 23. Later rounds: a sample click (seam S2), the pulse shaper's asymmetric clip (85's rest), stereo (5), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | Hz | 30 | 150 | 55 | 0 | | |
| 1 | Decay | s | 0.05 | 3 | 0.5 | 0 | | |
| 2 | Attack | | 0 | 1 | 0.4 | 0 | | |
| 3 | Drive | | 0 | 1 | 0.2 | 0 | | |
| 4 | Tone | Hz | 200 | 12000 | 4000 | 0 | | |
| 5 | Level | | 0 | 1 | 0.8 | 0 | | |
| 6 | Bend | st | 0 | 36 | 16 | 0 | | new |
| 7 | Drop | s | 0.005 | 0.3 | 0.03 | 0 | | new |
| 8 | Slope | | -1 | 1 | 0 | 0 | | new |
| 9 | Click | | 0 | 1 | 0 | 1 | Noise, Pulse | new |
| 10 | Sigh | | 0 | 1 | 0.3 | 0 | | new |
| 11 | Low | dB | -12 | 12 | 0 | 0 | | new |
| 12 | High | dB | -12 | 12 | 0 | 0 | | new |
| 13 | Curve | | -1 | 1 | 0 | 0 | | new |
| 14 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 15 | Key | | 0 | 1 | 0 | 1 | Off, On | new |

Chain: the bridged-T bass drum (a resonator pinged by a shaped pulse, its frequency pushed up for the first milliseconds, a leak that makes the sigh, an op-amp's soft clip, tone and level — Werner et al., DAFx-14). Bend is the jump in semitones, depth 2^(Bend/12) − 1 (16 st ≈ today's constant ×2.5); Drop its time (today's 30 ms); Slope its shape; Click Noise is today's high-passed snap, Pulse the circuit's own excitation (a positive pulse, a delayed negative one, a short plateau) scaled by Attack; Sigh feeds the body's own low-passed output back into its frequency (a named depth, ours); Low and High are shelves after the drive at named 80 Hz and 5 kHz; Touch sets Tone × 2^(Touch × (v − 1)); Key On plays Tune × 2^((note − 60)/12).

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve; Tone by Touch | NEEDS core/percussion — `struck(velocity, curve)`, `touched` (see design/drums/clap.md) |
| Key → Tune re-settled before the strike | `core/phase` `ratio` (stands) |
| the body: a resonator swept by Bend, Drop, Slope, plus Sigh | NEEDS core/percussion — `Sweep` slope (see design/drums/conga.md); `Kick` moves its body onto a `Sweep` in `percussion.kick.*` |
| the click: noise snap or the shaped pulse, × Attack | `core/noise` `White`, `core/filter` `Pole` HIGH (stand); the pulse in `percussion.kick.*` |
| drive: soft clip at 1 + 8 × Drive, at twice the rate | `core/shaper` `soft`, `Oversampler` (stand; first drum to use it) |
| Tone low-pass, Low and High shelves | `core/filter` `Pole` LOW, `Biquad` LOWSHELF, HIGHSHELF (stand) |
| Level, every lane | `core/percussion` `play` (stands) |

The standing `settle(kick, tune, decay, attack, drive, tone, rate)` stays and settles Bend 16, Drop 0.03, Slope 0, Noise, Sigh 0, flat shelves, so kit and rhythm keep today's kick (now with the oversampled clip).

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::KICK`, 4 the sheet, 5 (no maker's name for the circuit), 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/kick/` and `core/percussion/percussion.kick.*` only (the click and the shelves in a dotted sibling such as `percussion.kick.click.cpp` when the unit passes 100 lines).
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves (the click kind by a table of two functions); no magic literals (60, 80 Hz, 5 kHz, the pulse's widths and the sigh depth named).

## Gate

- After the library shift has landed `struck(velocity, curve)`, `touched` and the `Sweep` slope.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::KICK::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a kick on every note; Bend 36 with Drop 0.2 a long zap; Click Pulse a rounder thump; Sigh 1 a wobble in the tail; Drive 1 clean of fizz; Low +12 boom; soft hits darker; Key On a bass line.

## Learned

| Method | Source | Licence |
|---|---|---|
| the circuit in blocks: pulse shaper, a bridged-T in feedback, the pitch jump, the retrigger leak (the sigh) | Werner, Abel, Smith, DAFx-14, "TR-808 Bass Drum Circuit" | paper |
| a pinged band with attack FM and self-FM (the sigh) | Plaits `plaits/dsp/drums/analog_bass_drum.h` | MIT |
| the excitation: positive pulse, delayed negative pulse, plateau | Peaks `peaks/drums/bass_drum.cc` | MIT |
| the kick behind an oversampler; a cubic pitch bend | Befaco Kickall `src/Kickall.cpp` | GPL-3.0+ (method only) |
| start and end pitch on a power law; a slope row | LMMS Kicker `plugins/Kicker/KickerOsc.h` | GPL-2.0+ (method only) |
| a post-drive EQ; click kinds | KICK 3 (reviews), Ableton DS Kick (manual) | commercial |

Cores: core/percussion
