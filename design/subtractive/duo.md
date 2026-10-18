# duo — round 1

Gains the drive switch into both slopes, the 24 dB slope on the saturating cascade and the 12 dB on the saturating state-variable (both 2x), velocity, drift and trims between its two voices, control-rate settle — matrix lacks 48 (in part: the drive switch; the third filter revision a later round), 20, 9, 4, 6, 14.

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Bend | st | -12 | 12 | 0 | 0 | | |
| 2 | Glide | s | 0 | 3 | 0 | 0 | | |
| 3 | Coarse1 | st | -24 | 24 | 0 | 48 | | |
| 4 | Keyboard | | 0 | 1 | 1 | 1 | | |
| 5 | Wave1 | | 0 | 1 | 0 | 1 | Saw, Square | |
| 6 | Level1 | | 0 | 1 | 0.6 | 0 | | |
| 7 | Coarse2 | st | -24 | 24 | 0 | 48 | | |
| 8 | Fine2 | ct | -100 | 100 | 7 | 0 | | |
| 9 | Wave2 | | 0 | 1 | 0 | 1 | Saw, Pulse | |
| 10 | Level2 | | 0 | 1 | 0.6 | 0 | | |
| 11 | Sync | | 0 | 1 | 0 | 1 | | |
| 12 | Width | | 0.05 | 0.5 | 0.5 | 0 | | |
| 13 | Sweep | | 0 | 1 | 0 | 0 | | |
| 14 | Source | | 0 | 1 | 0 | 1 | LFO, ADSR | |
| 15 | Ring | | 0 | 1 | 0 | 0 | | |
| 16 | Noise | | 0 | 1 | 0 | 0 | | |
| 17 | Colour | | 0 | 1 | 0 | 1 | White, Pink | |
| 18 | Rate | Hz | 0.2 | 20 | 5 | 0 | | |
| 19 | Vibrato | st | 0 | 12 | 0 | 0 | | |
| 20 | Mix | | 0 | 1 | 1 | 0 | | |
| 21 | Clock | | 0 | 1 | 0 | 1 | LFO, Keyboard | |
| 22 | Lag | s | 0 | 1 | 0 | 0 | | |
| 23 | Sample | st | 0 | 12 | 0 | 0 | | |
| 24 | Slope | | 0 | 1 | 1 | 1 | 12dB, 24dB | |
| 25 | Cutoff | Hz | 20 | 20000 | 2000 | 0 | | |
| 26 | Resonance | | 0 | 1 | 0.2 | 0 | | |
| 27 | Tracking | | 0 | 1 | 0.5 | 0 | | |
| 28 | Wobble | oct | 0 | 3 | 0 | 0 | | |
| 29 | Scatter | oct | 0 | 3 | 0 | 0 | | |
| 30 | Amount | oct | 0 | 5 | 2 | 0 | | |
| 31 | Contour | | 0 | 1 | 0 | 1 | ADSR, AR | |
| 32 | Highpass | Hz | 10 | 10000 | 10 | 0 | | |
| 33 | Attack | s | 0.001 | 10 | 0.005 | 0 | | |
| 34 | Decay | s | 0.001 | 10 | 0.3 | 0 | | |
| 35 | Sustain | | 0 | 1 | 0.7 | 0 | | |
| 36 | Release | s | 0.001 | 10 | 0.3 | 0 | | |
| 37 | Attack2 | s | 0.001 | 10 | 0.005 | 0 | | |
| 38 | Release2 | s | 0.001 | 10 | 0.5 | 0 | | |
| 39 | Amplifier | | 0 | 1 | 0 | 1 | ADSR, AR | |
| 40 | Repeat | | 0 | 1 | 0 | 1 | | |
| 41 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 42 | Drive | | 0 | 1 | 0 | 1 | | new |
| 43 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 44 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 45 | Drift | ct | 0 | 20 | 3 | 0 | | new |
| 46 | Trim | | 0 | 1 | 0.3 | 0 | | new |

Chain: the duophonic's public panel (two VCOs, sync, ring, sample-and-hold on the LFO or keyboard clock, the 2-pole/4-pole revisions, HPF, ADSR + AR, low/high split, the later units' drive switch). Drive On is a fixed gain into the filter's own saturation (×3 — ours). Velocity, Touch, Drift and Trim are ours.

## Chain

| Stage | Rides |
|---|---|
| duophonic allocation: lowest held to VCO 1, highest to VCO 2; Glide | `core/voice` `DUO` |
| per part: pitch trim drawn once at create (±8 ct × Trim) and a slow wander (Drift cents) — the two voices never quite agree | NEEDS core/voice — `Trim` (see dco); NEEDS core/modulator — `Drift` (see dco) |
| VCO 1, VCO 2 with sync, ring, pink/white noise, LFO + sample-and-hold with lag | `core/oscillator` (`Sync`), `core/noise`, `core/modulator` |
| HPF | `core/filter` `Pole` |
| 12 dB: the state-variable low-pass, saturation 0 or the Drive gain; 24 dB: the cascade, drive 1 or the Drive gain; cutoff × Tracking × Amount × Wobble × Scatter × Touch·velocity, settled once per stride, both at 2x | NEEDS core/filter — `Variable` gains a `saturation` 0..1 (0 = today's linear loop, so wah and resonator keep their sound) softly clipping the band state inside the loop, 2x through `core/shaper` `Oversampler`; NEEDS core/filter — `Transistor` (see dco); NEEDS core/block — `Stride` (see acid) |
| ADSR and AR; the amplifier's depth = Velocity | `core/envelope` |
| DC blocker, Volume | `core/shaper` `Blocker` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `duo` (files under instruments/mono, 381h). Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::DUO`, 4 the sheet (rows appended to `duo.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = "duo"` as it stands), 3, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; the slope stays a table of two passes; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::DUO::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, two held keys sound two pitches; Drive On thickens and growls at both slopes; two voices on an octave beat slowly with Drift and Trim up; hard notes louder and brighter.

## Learned

| Method | Source | Licence |
|---|---|---|
| nonlinear ladder: saturation per stage, 2x, half-sample feedback delay | Huovilainen DAFx-04; sst-filters `VintageLadders.h` | copyright; GPL-3.0 (method only) |
| two-pole state-variable with a curve in its resonance path | 2DaT Obxd `Source/Engine/Filter.h`; Faust `vaeffects.lib` `oberheim` | GPL-3.0 (method only); STK-4.3 |
| the drive switch and the filter revisions | Korg ARP Odyssey (product page) | commercial page |
| per-voice variance and drift between the two voices | 2DaT Obxd `ObxdVoice.h`; Odin 2 `DriftGenerator.cpp` | GPL-3.0 (method only) |
| coefficients at control rate | stmlib `dsp/parameter_interpolator.h` | MIT |

Cores: core/block, core/filter, core/modulator, core/voice
