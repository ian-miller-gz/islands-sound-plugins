# sampler — round 1

Gains its re-founding on the cores (first, before any gain), a clip of its own so it sounds on an empty session, a cubic read with a 2x path past a step of one, End and Reverse, loop points with four modes and a crossfade, an exponential ADSR, a filter with its own contour, a pitch contour, one LFO, pitch and voice-mode rows, velocity depth, 16 voices — matrix lacks 2, 37, 98, 97, 99, 16, 13, 11 (in part: one LFO), 17, 12, 4, 25. Round 2 (one shift): zones, velocity layers with an equal-power crossfade, round robin, choke groups with a fade, start modulation (94, 95, 96, 19, 100). Later rounds: release trigger (101), programs as whole sheets (3), Pan and Width (5, after core/stereo), effects (10), slicing (102), granular (103), microtuning (7), a sinc read, 32 voices; the live clip list waits on seam S7 and Bend on S1.

## Rows

The public chain is the sample voice of the SoundFont 2.04 generator model (sample → pitch → low-pass → volume envelope; one modulation envelope and one LFO to pitch and cutoff) under the SFZ region's opcodes; `Chain` names the generator or opcode a row answers and its default where one fits. Rows 0–7 keep their indices; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | | Chain |
|---|---|---|---|---|---|---|---|---|---|
| 0 | Gain | | 0 | 1 | 0.80 | 0 | | | volume |
| 1 | Stock | | 0 | clips − 1 | 0 | clips − 1 | the clips' names, then `Tone` | widened | sample |
| 2 | Root | | 0 | 127 | 60 | 0 | | | pitch_keycenter 60 |
| 3 | Tune | ct | -50 | 50 | 0 | 0 | | | tune |
| 4 | Start | % | 0 | 100 | 0 | 0 | | changed (ms) | offset 0 |
| 5 | Loop | | 0 | 3 | 0 | 3 | Off, Forward, Pingpong, Sustain | changed (Off, On) | loop_mode no_loop, loop_continuous, alternate, loop_sustain |
| 6 | Attack | s | 0.001 | 4 | 0.01 | 0 | | changed (0–1) | ampeg_attack |
| 7 | Release | s | 0.005 | 4 | 0.20 | 0 | | changed (–2) | ampeg_release |
| 8 | Decay | s | 0.001 | 4 | 1 | 0 | | new | ampeg_decay |
| 9 | Sustain | | 0 | 1 | 1 | 0 | | new | ampeg_sustain 100 % |
| 10 | Curve | | 0 | 1 | 1 | 1 | Linear, Exponential | new | SF2 envelopes fall in dB |
| 11 | End | % | 0 | 100 | 100 | 0 | | new | end |
| 12 | Reverse | | 0 | 1 | 0 | 1 | Off, On | new | direction reverse |
| 13 | Repeat | % | 0 | 100 | 50 | 0 | | new | loop_start |
| 14 | Turn | % | 0 | 100 | 95 | 0 | | new | loop_end |
| 15 | Fade | % | 0 | 50 | 10 | 0 | | new | loop_crossfade (ours: % of the loop) |
| 16 | Coarse | st | -24 | 24 | 0 | 0 | | new | transpose |
| 17 | Track | | 0 | 1 | 1 | 1 | Off, On | new | pitch_keytrack 100 ct |
| 18 | Mode | | 0 | 2 | 0 | 2 | Poly, Mono, Legato | new | polyphony 1; trigger legato |
| 19 | Voices | | 1 | 16 | 16 | 15 | | new | polyphony |
| 20 | Glide | s | 0 | 2 | 0 | 0 | | new | portamento (ours) |
| 21 | Velocity | | 0 | 1 | 1 | 0 | | new | amp_veltrack 100 % |
| 22 | Filter | | 0 | 4 | 0 | 4 | Off, Low, Band, High, Notch | new | fil_type none, lpf_2p, bpf_2p, hpf_2p, brf_2p |
| 23 | Cutoff | Hz | 20 | 20000 | 8000 | 0 | | new | cutoff |
| 24 | Resonance | | 0 | 1 | 0.2 | 0 | | new | resonance (ours: emphasis) |
| 25 | Keyboard | | 0 | 1 | 0 | 0 | | new | fil_keytrack 0 (1 = 1200 ct) |
| 26 | Envelope | oct | -4 | 4 | 2 | 0 | | new | modEnvToFilterFc |
| 27 | Sweep | s | 0.001 | 4 | 0.005 | 0 | | new | attackModEnv |
| 28 | Close | s | 0.001 | 4 | 0.5 | 0 | | new | decayModEnv = releaseModEnv; sustainModEnv 0 |
| 29 | Touch | oct | 0 | 3 | 0 | 0 | | new | fil_veltrack 0 |
| 30 | Drop | st | -24 | 24 | 0 | 0 | | new | modEnvToPitch |
| 31 | Rate | Hz | 0.05 | 20 | 5 | 0 | | new | freqVibLFO (ours 5) |
| 32 | Vibrato | st | 0 | 2 | 0 | 0 | | new | vibLfoToPitch |
| 33 | Wobble | oct | 0 | 4 | 0 | 0 | | new | modLfoToFilterFc |
| 34 | Quality | | 0 | 2 | 2 | 2 | Linear, Cubic, Double | new | sfizz quality tiers; SCXT 2x |

- Stock is the one dynamic row: the sheet states 0..0; `voice.stock.cpp` widens `control` to the clip count with the names as words, reads the name, and clamps a turn or a PROGRAM to the count. Every other row lands through `CORE::TABLE::clamped`.
- `Tone`, always the last clip: drawn once at create at the instance's rate, 2 s, key 60: the saw and the pulse of `CORE::OSCILLATOR` half each, through `CORE::FILTER::Variable` low-pass falling 6 kHz → 1.5 kHz, level falling 0.5 → 0.15, both exponential (ours). On an empty session Stock 0 is `Tone`, so the rests sound.
- Laws per voice, settled once per stride: semis = Track × (glided key − Root) + Coarse + Tune/100 + Drop × contour + Vibrato × lfo; step = the clip's rate ratio × `PHASE::ratio(100 × semis)`. Cut = Cutoff × 2^(Keyboard × (key − Root)/12 + Envelope × contour + Wobble × lfo + Touch × (velocity − 1)). Level = loudness × `ENVELOPE::weigh(velocity, Velocity)` × Gain.
- Regions: play [min(Start, End), max(Start, End)]; loop [min(Repeat, Turn), max(Repeat, Turn)] held inside it; a loop under 32 frames plays as Off. Reverse starts at the region's top and walks down; a loop then jumps from its bottom back to its top.

## Chain

| Stage | Rides |
|---|---|
| the sheet: `sampler.indices.hpp`, `sampler.hpp` (`ROWS`, `SHEET`, the word arrays); `params.cpp`, `voice.params.cpp`, the hand-written `SURFACE` leave; Stock as above | `core/table` |
| clips: the inventory's audio clips copied at create (as today), then `Tone` (`voice.tone.cpp`); each clip carries its rate ratio | `core/oscillator`, `core/filter` (Tone only) |
| allocation: Voices, steal `QUIETEST` (note level written each stride), Mode by a table: Poly → `POLY`; Mono → `MONO`, legato off; Legato → `MONO`, legato, slide `LEGATO`; Glide | `core/voice` |
| loudness ADSR (Attack, Decay, Sustain, Release, Curve, reset); the contour ADSR (Sweep, Close, sustain 0, Close, exponential) | `core/envelope` |
| one sine LFO at Rate to Vibrato and Wobble | `core/modulator` `Lfo` |
| pitch, cut and level per voice once per stride (16 frames) | `core/phase` (`ratio`); NEEDS core/block — `Stride` (see design/subtractive/acid.md) |
| the walk by Loop through a table of four walkers (`voice.loop.cpp`), direction by Reverse; Forward and held Sustain cross-fade the last Fade % of the loop into the audio before Repeat, gains √(1 − x) and √x; Pingpong turns at both ends, no fade; Sustain plays out to the region's end after the lift | — (the instrument's own) |
| the read by Quality through a table: Linear (today's); Cubic; Double: when the strike's step (Track, Root, Coarse, Tune) passes 1, two cubic reads at place and place + step/2 per clip lane into a half-band `down`, chosen at strike, never mid-note; taps beyond a loop's end read from after its start | NEEDS core/oscillator — `cubic(const Float *points, Float fraction) -> Float`: the third-order Hermite through four equally spaced points (one before the place, two after), fraction 0..1 between the second and third; pure, no state. Riders: sampler now; additive, `Table` reads, `core/line` later (lack 37). `core/shaper` `Oversampler` (`down` stands) |
| the filter by Filter through a table: Off passes; Low, Band, High, Notch → `Variable` kinds | `core/filter` `Variable` |
| clip lane c to lane c, the last clip lane to any further lane (as today); the voice ends when the loudness gate idles or the walk leaves the region | — |
| flush, cursor, peak, meter | `core/block` |

## Surface

- ins: NOTES `notes`, CONTROL `turns`, DATA `material`, PROGRAM `programs`; outs: AUDIO `signal`, DATA `material` — as today. A source: no AUDIO in; every sample written. The two DATA ports stay as they stand, carried by nothing yet (seam S7); this round neither adds nor reads them.
- voicing: `poly` (Mode moves the allocator, not the offer). Meter: `CORE::BLOCK::Meter`.
- Units after: `sampler.hpp`, `sampler.indices.hpp`, `voice.internal.hpp`, `voice.cpp`, `voice.render.cpp`, `voice.notes.cpp`, `voice.settle.cpp`, `voice.stock.cpp`, `voice.take.cpp`, `voice.tone.cpp`, `voice.loop.cpp`, `voice.read.cpp`.

## Laws

- 379: 1 tree, 2 includes (`../../core/<family>/<family>.hpp`, `../../../plugin.hpp`, `../../../inventory.hpp` in `voice.take.cpp` only — the shape suite admits it; `<xmmintrin.h>`, `<threads.hpp>`, `<cmath>` leave the instrument), 3 `SOUND::PLUGINS::SAMPLER`, 4 the sheet is the core's (rows as `Float rows[PARAMETERS]`; Stock as stated), 5 names (`SECOND`, `NOTES`, `12`, `1200` give way to `CORE::PHASE`), 6 honesty, 7 cores (the flush, meter, allocator, envelope and cents table never re-rolled).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings (`Tone` is a step word in the sheet's array; `none` leaves); no `switch` — events by `CORE::VOICE::apply`, Loop, Quality, Filter and Mode by tables; no magic literals (2 s, 6 kHz, 1.5 kHz, 0.5, 0.15, 32, 16, the step threshold 1 named).

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::SAMPLER::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree; `grep -c switch instrument/sampler/*` 0 in every unit.
- Heard, on a session with no clips: silent before a note; a note sounds the `Tone`, darkening and fading over 2 s; Loop Sustain holds a held note past 2 s with no click at the seam and plays out on the lift; Reverse swells backward; Filter Low sweeps down from the strike; key 96 whistles under Linear and is clean under Double.

## Learned

| Method | Source | Licence | Here |
|---|---|---|---|
| a 4-point cubic read through one point before and two after, guard points past the ends | Pure Data `src/d_array.c`; Puckette, *Theory and Techniques* ch. 2 | BSD-3-Clause | this round (`cubic`) |
| per-voice 2x only past a step threshold, then a half-band decimation | Shortcircuit XT `src/scxt-core/voice/voice.cpp` | MIT (source) | this round |
| one quality number per region picking nearest, linear, Hermite or a sinc length | sfizz `src/sfizz/Interpolators.hpp`, `Voice.cpp` | BSD-2-Clause | this round (the tiers); the sinc later |
| guard points past a loop's end filled from its start | FluidSynth `src/rvoice/fluid_rvoice_dsp.cpp` | LGPL-2.1+ | this round (method only) |
| loop held only before release, then play out to the end | FluidSynth `src/rvoice/fluid_rvoice.c` | LGPL-2.1+ | this round (Sustain; method only) |
| loop crossfade: the tail fades out while the audio before the loop start fades in | sfizz `src/sfizz/Voice.cpp` | BSD-2-Clause | this round (Fade) |
| ping-pong turns; loop and direction flags as a small index into walkers | Shortcircuit XT `src/scxt-core/dsp/generator.cpp` | MIT (source) | this round (the walker table) |
| a second read head one loop behind; loop points snapped to zero crossings | samplv1 `src/samplv1_sample.cpp` | GPL-2.0+ | later (method only) |
| the sinc read: one wing stored finely, stretched past a step of one; polyphase quality levels | J. O. Smith, *Digital Audio Resampling*; Speex `libspeexdsp/resample.c` | text; BSD-3-Clause | later |
| round robin by a per-layer counter; choice by loudness distance, recency and chance | sfizz `src/sfizz/Layer.cpp`; DrumGizmo `src/sample_selection.cc` | BSD-2-Clause; LGPL-3.0+ | round 2 |
| exclusive class: same-class voices given a very fast release | FluidSynth `src/synth/fluid_synth.c`, `fluid_voice.c` | LGPL-2.1+ | round 2 (method only) |
| velocity curves filled into a table at load | sfizz `src/sfizz/Curve.cpp` | BSD-2-Clause | later |
| early end when envelope × loop peak sinks under the floor | FluidSynth `src/synth/fluid_voice.c` | LGPL-2.1+ | later (method only) |
| release clip level scaled by held time | LinuxSampler `src/engines/common/AbstractVoice.cpp` | GPL-2.0+ with a no-commercial-use exception | later (idea only) |

Cores: core/block
