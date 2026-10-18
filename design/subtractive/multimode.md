# multimode — round 1

Gains the chain's eight-voice form, a saturating resonance in the state-variable loop (2x), velocity to level and cutoff, drift and trims per voice, control-rate settle — matrix lacks 25, 20, 4, 9, 14. Later rounds: the modulation slots (lack 11, a `core/matrix` first).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Bend | st | -12 | 12 | 0 | 0 | | |
| 2 | Glide | s | 0 | 3 | 0 | 0 | | |
| 3 | Frequency1 | st | -24 | 24 | 0 | 0 | | |
| 4 | Shape1 | | 0 | 1 | 0 | 0 | | |
| 5 | Level1 | | 0 | 1 | 0.6 | 0 | | |
| 6 | Frequency2 | st | -24 | 24 | 0.07 | 0 | | |
| 7 | Shape2 | | 0 | 1 | 0 | 0 | | |
| 8 | Level2 | | 0 | 1 | 0.6 | 0 | | |
| 9 | Sync | | 0 | 1 | 0 | 1 | | |
| 10 | Width | | 0.05 | 0.5 | 0.5 | 0 | | |
| 11 | Sweep | | 0 | 1 | 0 | 0 | | |
| 12 | Source | | 0 | 1 | 0 | 1 | LFO, Envelope | |
| 13 | Modulation | st | 0 | 12 | 0 | 0 | | |
| 14 | Modulator | | 0 | 1 | 0 | 1 | LFO, Envelope | |
| 15 | Rate | Hz | 0.1 | 20 | 4 | 0 | | |
| 16 | Cutoff | Hz | 20 | 20000 | 1500 | 0 | | |
| 17 | Resonance | | 0 | 1 | 0.2 | 0 | | |
| 18 | Mode | | 0 | 1 | 0 | 0 | | |
| 19 | Band | | 0 | 1 | 0 | 1 | | |
| 20 | Envelope | oct | -5 | 5 | 2 | 0 | | |
| 21 | Wobble | oct | 0 | 3 | 0 | 0 | | |
| 22 | Tracking | | 0 | 1 | 0.5 | 0 | | |
| 23 | Attack1 | s | 0.001 | 10 | 0.005 | 0 | | |
| 24 | Decay1 | s | 0.001 | 10 | 0.4 | 0 | | |
| 25 | Sustain1 | | 0 | 1 | 0.3 | 0 | | |
| 26 | Attack2 | s | 0.001 | 10 | 0.005 | 0 | | |
| 27 | Decay2 | s | 0.001 | 10 | 0.3 | 0 | | |
| 28 | Sustain2 | | 0 | 1 | 0.8 | 0 | | |
| 29 | Unison | | 0 | 1 | 0 | 1 | | |
| 30 | Detune | ct | 0 | 50 | 10 | 0 | | |
| 31 | Spread | | 0 | 1 | 0.5 | 0 | | |
| 32 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 33 | Voices | | 1 | 8 | 8 | 7 | | new |
| 34 | Saturation | | 0 | 1 | 0.25 | 0 | | new |
| 35 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 36 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 37 | Drift | ct | 0 | 20 | 3 | 0 | | new |
| 38 | Trim | | 0 | 1 | 0.3 | 0 | | new |

Chain: the two-VCO, 12 dB state-variable chain (low → notch → high sweep, band on a switch, two ADS envelopes, one LFO), in its eight-voice form. Voices is the allocator's count; Unison On stacks every voice on one key. Saturation 0 is today's linear loop. Velocity, Touch, Drift and Trim are ours.

## Chain

| Stage | Rides |
|---|---|
| allocation, Voices 1–8, oldest stolen; Unison with Detune; Glide | `core/voice` (`count`, `unison`, `detune` stand); `VOICES` becomes 8 |
| per voice slot: pitch, cutoff, envelope-time trims (±8 ct, ±0.25 oct, ±10 % × Trim) and a slow wander (Drift cents) | NEEDS core/voice — `Trim` (see dco); NEEDS core/modulator — `Drift` (see dco) |
| two VCOs, saw↔pulse blend, sync, PWM by LFO or envelope | `core/oscillator` |
| VCF: low/high crossfade by Mode, band by Band; cutoff × Tracking × Envelope × Wobble × Touch·velocity × trim, settled once per stride, run at 2x with Saturation in the loop | NEEDS core/filter — `Variable` `saturation` (see duo); `core/shaper` `Oversampler`; NEEDS core/block — `Stride` (see acid) |
| two ADS envelopes; loudness depth = Velocity | `core/envelope` |
| pan per voice slot × Spread: −1, 1, −0.75, 0.75, −0.5, 0.5, −0.25, 0.25 (ours), the standing linear lean | this instrument (`PANS[VOICES]`) |
| DC blocker, Volume | `core/shaper` `Blocker` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; left, right, mono sum as today.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::MULTIMODE`, 4 the sheet (rows appended to `multimode.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::MULTIMODE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note; a four-note chord sounds whole (today two notes steal); Saturation 1 at Resonance 0.8 thickens and tames the peak; voices sit apart across the stereo field.

## Learned

| Method | Source | Licence |
|---|---|---|
| TPT state-variable, the loop solved | stmlib `dsp/filter.h`; Zavalishin, *The Art of VA Filter Design* | MIT; copyright |
| two-pole OB-style SVF with a curve in its resonance path | 2DaT Obxd `Source/Engine/Filter.h`; Faust `vaeffects.lib` `oberheim` | GPL-3.0 (method only); STK-4.3 |
| the eight-voice form of the chain | the V Collection's SEM V (product page) | commercial page |
| per-voice variance; drift | 2DaT Obxd `ObxdVoice.h`; sst-basic-blocks `OscillatorDriftUnisonCharacter.h` | GPL-3.0 (method only) |
| allocator: same note, else idle, else oldest | stmlib `algorithms/voice_allocator.h` | MIT |
| coefficients at control rate | stmlib `dsp/parameter_interpolator.h` | MIT |

Cores: core/block, core/filter, core/modulator, core/voice
