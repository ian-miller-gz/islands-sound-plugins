# snare — round 1

Gains its own pitch drop, a rattle that holds before it decays, a Body row crossfading the two-mode body into a bright five-mode one, rim and side-stick strokes by note, and velocity through a curve to the noise band — matrix lacks 92, 93 (the body), 21, 4. Later rounds: noise through a pitch-tracking band (93's rest), oversampled clip (6), stereo (5), programs (3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | Hz | 100 | 400 | 180 | 0 | | |
| 1 | Tone | Hz | 500 | 10000 | 3000 | 0 | | |
| 2 | Snappy | | 0 | 1 | 0.6 | 0 | | |
| 3 | Decay | s | 0.05 | 1.5 | 0.25 | 0 | | |
| 4 | Level | | 0 | 1 | 0.8 | 0 | | |
| 5 | Bend | st | 0 | 24 | 4 | 0 | | new |
| 6 | Drop | s | 0.005 | 0.2 | 0.02 | 0 | | new |
| 7 | Hold | s | 0 | 0.05 | 0.008 | 0 | | new |
| 8 | Body | | 0 | 1 | 0.3 | 0 | | new |
| 9 | Curve | | -1 | 1 | 0 | 0 | | new |
| 10 | Touch | oct | 0 | 3 | 1 | 0 | | new |

Chain: the analogue snare (two detuned bridged-T heads and noise through a band and a high-pass with its own decay). Bend drops both heads from Bend semitones above (depth 2^(Bend/12) − 1) over Drop; Hold keeps the rattle at its peak for Hold seconds before it decays; Body crossfades today's two heads into five modes at the ideal membrane's ratios (1, 1.59, 2.14, 2.30, 2.65 — physics, not a tree's table), the upper ones damped harder; Touch sets the noise band at Tone × 2^(Touch × (v − 1)). Strokes by note (general MIDI): 38 head, 37 side stick (heads only, a named third of Decay, no rattle, a click), 40 rimshot (head and rattle plus a short ping a named ratio above Tune, a named +3 dB); any other note the head.

## Chain

| Stage | Rides |
|---|---|
| velocity through Curve; the band by Touch | NEEDS core/percussion — `struck(velocity, curve)`, `touched` (see design/drums/clap.md) |
| note → stroke by a key table | `core/percussion` `keyed` (stands) |
| heads and modes swept by Bend, Drop | NEEDS core/percussion — `Sweep` slope (see design/drums/conga.md; slope rests 0 here); `Snare` moves its heads onto sweeps in `percussion.snare.*` |
| noise band at the touched Tone, high-pass, the rattle held then decaying × Snappy | `core/filter` `Biquad`, `Pole`; `core/envelope` (stand); the hold in `percussion.snare.*` |
| the rim ping, the side click | `core/percussion` `Resonator` (stands) |
| soft clip, Level, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

The standing `settle(snare, tune, tone, snappy, decay, rate)` and `strike(snare, velocity)` stay and keep Bend 0, Hold 0, Body 0, the head stroke — kit and rhythm sound as today.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::SNARE`, 4 the sheet, 5, 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Edits `instrument/snare/` and `core/percussion/percussion.snare.*` only (the strokes in a dotted sibling when the unit passes 100 lines).
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves (strokes by a table); no magic literals (the mode ratios, the side's third, the rim's ratio and +3 dB named).

## Gate

- After the library shift has landed `struck(velocity, curve)`, `touched` and the `Sweep` slope.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::SNARE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note; 38 a snare with a slight downward snap, 37 a dry knock, 40 a ringing crack; Body 1 brighter and denser; Hold 0.05 a fatter rattle; soft hits darker.

## Learned

| Method | Source | Licence |
|---|---|---|
| five inharmonic modes, the upper damped harder; a tone crossfading two modes into many | Plaits `plaits/dsp/drums/analog_snare_drum.h` | MIT |
| heads with a pitch drop; noise held, then decaying | Plaits `plaits/dsp/drums/synthetic_snare_drum.h` | MIT |
| rimshot and side-stick strokes | Addictive Drums 2, Superior Drummer 3; the general MIDI map 37/38/40 | commercial; standard |

Cores: core/percussion
