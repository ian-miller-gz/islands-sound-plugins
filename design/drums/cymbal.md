# cymbal — round 1

Gains the circuit's three bands with their own envelopes, a bell stroke by note, a high-pass Edge apart from the balance, noise in the wash, a choke on the lift with a short fade, and velocity through a curve to the high band — matrix lacks 82, 21, 22, 44, 19, 4. Later rounds: a stereo metal spread (5), choke by aftertouch (seam S1), band-limited squares (core/oscillator `Edges`), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | st | -12 | 12 | 0 | 0 | | |
| 1 | Decay | s | 0.2 | 5 | 1.5 | 0 | | |
| 2 | Tone | | 0 | 1 | 0.5 | 0 | | |
| 3 | Splash | | 0 | 1 | 0.5 | 0 | | |
| 4 | Level | | 0 | 1 | 0.8 | 0 | | |
| 5 | Edge | Hz | 1000 | 10000 | 2000 | 0 | | new |
| 6 | Noise | | 0 | 1 | 0.15 | 0 | | new |
| 7 | Choke | | 0 | 1 | 0 | 1 | Off, On | new |
| 8 | Curve | | -1 | 1 | 0 | 0 | | new |
| 9 | Touch | | 0 | 1 | 0.5 | 0 | | new |

Chain: the analogue cymbal (six squares at the metallic ratios, three bands each with its envelope, a tone stage mixing them — Werner et al., 2014). The bands: low at 3440 Hz and mid at 7100 Hz (today's two), high at a named 10.5 kHz (ours), each with a length per plate in a table of ours; Tone balances low against high about the mid; Edge is a high-pass after the mix; Noise is white noise's share in the mid and high bands; Choke On: a lift (NOTE_OFF or velocity 0) fades the cymbal out over 5 ms; Touch weighs the high band by velocity, `ENVELOPE::weigh(v, Touch)`. Notes: 51 and 59 ride, 53 bell (general MIDI's ride bell; today a ride), any other crash. The bell: the low band long and loud, the mid short, the high silent, no splash (ours).

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve | NEEDS core/percussion — `struck(velocity, curve)` (see design/drums/clap.md) |
| note → plate (RIDE, CRASH, BELL) by the key table; the lift → choke when Choke is On | `core/percussion` `keyed` (stands) |
| six squares at Tune, plus white noise × Noise into the mid and high bands | `core/percussion` `Metal`, `core/noise` `White` (stand) |
| three bands, three envelopes per plate, the flash on the mid band × Splash | `core/percussion` `Cymbal` rewritten in `percussion.cymbal.*` (`TAIL` gains a third band, `PLATE` gains `BELL`); the standing `settle(cymbal, tune, decay, tone, splash, rate)` stands with Edge, Noise and the bell at rests — kit and rhythm hear the third band too, by design |
| a short fade on the choke | NEEDS core/percussion — `Choke`: a gain that is 1 while open and falls linearly to 0 over `seconds` once closed; `settle(choke, seconds, rate)`, `open(choke)`, `close(choke)`, `tick(choke) -> Float`, `sounding(choke) -> Flag`; allocation-free. Riders: cymbal, hat, percussive (kit, machine and pads later) |
| Edge high-pass, soft clip, Level, every lane | `core/filter` `Biquad` HIGH, `core/shaper` `soft`, `core/percussion` `play` (stand) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::CYMBAL`, 4 the sheet, 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/cymbal/` and `core/percussion/percussion.cymbal.*` only (`percussion.cymbal.cpp` stands at 68 lines: the band table goes to a dotted sibling).
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (10.5 kHz, 5 ms named).

## Gate

- After the library shift has landed `struck(velocity, curve)` and `Choke`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::CYMBAL::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note; a crash on 49, a ride on 51, a clear bell on 53; a sizzle on top the old two bands lacked; Choke On cuts a crash at the lift without a click; soft hits darker.

## Learned

| Method | Source | Licence |
|---|---|---|
| one bank of six squares; three bands, each with an envelope and a high-pass; a tone stage mixing them | Werner, Abel, Smith, "The TR-808 Cymbal", ICMC/SMC 2014 | CC-BY-3.0 |
| noise crossfaded into the metal | Plaits `plaits/dsp/drums/hi_hat.h` | MIT |
| choke as a short fade, not a cut | sfizz `src/sfizz/Voice.cpp`; FluidSynth `src/synth/fluid_voice.c` | BSD-2-Clause; LGPL-2.1+ (method only) |
| bell and choke strokes | Addictive Drums 2, Superior Drummer 3 (manual, product page) | commercial |

Cores: core/percussion
