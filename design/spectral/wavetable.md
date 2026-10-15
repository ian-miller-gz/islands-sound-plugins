# wavetable — round 1

Gains band-limited table levels with up to 256 partials per frame, unison with a stereo spread, phase warps, filter modes, velocity to cutoff and position; its 16-frame counter moves onto the library — matrix lacks 18, 74, 26, 5, 75, 16, 4, 14. Later rounds: a sub and a noise source (76), spectral morph between frames (77), a vector position (78), cross-FM and sync between the sources (40), the modulation matrix (11), effects (10), programs (3), macros (8), microtuning (7).

## Rows

The 29 standing rows keep their indices, units, ranges and rests; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Bend | st | -12 | 12 | 0 | 0 | | |
| 2 | Table1 | | 0 | 15 | 2 | 15 | Sine, Harmonic, Saw, Square, Hollow, Triangle, Pulse, Organ, Octaves, Formant, Vowel, Comb, Sync, Prime, Scatter, Resonant | |
| 3 | Position1 | | 0 | 63 | 0 | 0 | | |
| 4 | Table2 | | 0 | 15 | 9 | 15 | as Table1 | |
| 5 | Position2 | | 0 | 63 | 8 | 0 | | |
| 6 | Interval | st | -24 | 24 | 0 | 48 | | |
| 7 | Detune | ct | -50 | 50 | 7 | 0 | | |
| 8 | Mix | | 0 | 1 | 0.5 | 0 | | |
| 9 | Sweep | | -63 | 63 | 24 | 0 | | |
| 10 | Scan | | 0 | 63 | 0 | 0 | | |
| 11 | Rate | Hz | 0.05 | 20 | 0.5 | 0 | | |
| 12 | Wave | | 0 | 5 | 1 | 5 | Sine, Triangle, Saw, Square, Sample, Random | |
| 13 | Vibrato | st | 0 | 2 | 0 | 0 | | |
| 14 | Cutoff | Hz | 20 | 20000 | 5000 | 0 | | |
| 15 | Resonance | | 0 | 1 | 0.2 | 0 | | |
| 16 | Envelope | oct | 0 | 5 | 1 | 0 | | |
| 17 | Keyboard | | 0 | 1 | 0.5 | 0 | | |
| 18–21 | Attack1, Decay1, Sustain1, Release1 | s, s, —, s | 0.001, 0.001, 0, 0.001 | 10, 10, 1, 10 | 0.005, 1.2, 0.3, 0.6 | 0 | | |
| 22–25 | Attack2, Decay2, Sustain2, Release2 | s, s, —, s | 0.001, 0.001, 0, 0.001 | 10, 10, 1, 10 | 0.005, 0.8, 0.7, 0.5 | 0 | | |
| 26 | Velocity | | 0 | 1 | 0.5 | 0 | | |
| 27 | Glide | s | 0 | 3 | 0 | 0 | | |
| 28 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 29 | Unison | | 1 | 4 | 2 | 3 | 1, 2, 3, 4 | new |
| 30 | Spread | ct | 0 | 50 | 10 | 0 | | new |
| 31 | Width | | 0 | 1 | 0.5 | 0 | | new |
| 32 | Warp | | 0 | 3 | 0 | 3 | Bend, Sync, Knee, Pulse | new |
| 33 | Amount | | 0 | 1 | 0 | 0 | | new |
| 34 | Mode | | 0 | 3 | 0 | 3 | 24dB, 12dB, Band, High | new |
| 35 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 36 | Push | | -63 | 63 | 8 | 0 | | new |

Chain: the two-table wavetable voice (two morphing table sources, a 24 dB filter, two ADSRs) with the unison, warp and filter-mode panels Vital, Serum 2 and Ableton Wavetable publish; the words and ranges are ours (Unison, Spread, Width, Mode, Touch as supersaw's). Unison stacks voices of one note Spread cents wide, panned ± Width, each × 1/√Unison; sixteen voices, so Unison 2 keeps today's eight-note chord. Warp acts on each source's read phase p (0..1), Amount 0 the identity for all four: Bend p^(4^Amount); Sync p × (1 + 7 × Amount), wrapped; Knee breaks at w = 0.5 − 0.45 × Amount (p < w: 0.5 × p/w, else 0.5 + 0.5 × (p − w)/(1 − w)); Pulse min(p/(1 − 0.9 × Amount), 1). Each warp's steepest slope (4^Amount, 1 + 7 × Amount, 0.5/w, 1/(1 − 0.9 × Amount)) multiplies the step that picks the level. Cutoff × 2^(Touch × (velocity − 1)): a full-velocity note keeps today's cutoff. Position + Push × velocity. Mode 24dB is today's filter. The sync warp's wrap edge is not band-limited this round.

## Chain

| Stage | Rides |
|---|---|
| allocation: sixteen voices (from eight), oldest stolen; Unison → `unison`, Spread → `detune`, Width → `spread`; Glide | `core/voice` (`stack` stands) |
| tables: sixteen built at create; each key frame's spectrum evaluated for 256 partials (from 32) and drawn into every level; frames between key frames blended on every level (the instrument's `blend` leaves) | NEEDS core/oscillator — `Table` levels: `build(table, frames, levels)`; level k holds partials 1..(TOP >> k), TOP = SIZE/8 = 256, at length SIZE >> k, so a table's memory at most doubles; `draw(table, frame, partials, count)` writes every level from one spectrum, every level scaled by level 0's peak so loudness holds across a switch, by a radix-2 inverse transform of our own (no per-sample `std::sin` sum); `blend(table, from, to)` on every level; `level(table, step) -> Whole`, the least level whose top partial × step stays under half a turn; `read(table, frame, level, phase)`, `morph(table, position, level, phase)`. A one-level table reads as today (operator, glottis untouched). Riders: wavetable now; operator, choir, vowel later (lack 18) |
| two sources per voice: phase warped by Warp and Amount, read at Position + Sweep × contour + Scan × lfo + Push × velocity, from the level of the step × the warp's slope; chosen once per stride | `core/oscillator` (as above); this instrument's `voice.warp.cpp` (a table of four warps and their slopes) |
| pitch: glided key + unison cents + Tune + Bend + Vibrato × lfo; the second source + Interval + Detune | `core/phase`, `core/modulator` `Lfo` (stand) |
| filter by Mode through a table: 24dB → `Ladder`; 12dB, Band, High → `Variable` `LOW`, `BAND`, `HIGH`; cutoff × Keyboard × Envelope × Touch, settled once per stride (today's 16-frame `clock` replaced) | `core/filter`; NEEDS core/block — `Stride` (see design/subtractive/acid.md) |
| contour and loudness ADSRs, loudness depth Velocity | `core/envelope` (stand) |
| each voice placed by its unison pan, summed per side | NEEDS core/stereo (see design/subtractive/pads.md: the equal-power pan law) |
| Volume; flush, cursor, meter | `core/block` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; lane 0 left, lane 1 right, a single lane the mono sum (dco's rule); every sample written.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter` over the loudest lane (`CORE::BLOCK::loudest`).

## Laws

- 379: 1 tree, 2 includes (core aggregates only), 3 `SOUND::PLUGINS::WAVETABLE`, 4 the sheet (new rows after Volume), 5 names, 6 honesty, 7 cores (the level choice and the blend are the core's; the unison place is `CORE::VOICE::stack`'s, never re-rolled).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25: `wavetable.hpp` stands at 79 (the step words, with `UNISONS`, `WARPS`, `MODES`, move to `wavetable.words.hpp`); `voice.tone.cpp` at 91 (the filter's mode table and settle go to `voice.filter.cpp`, the warps to `voice.warp.cpp`); `voice.formants.cpp` at 100 is not touched. No comments; no natural-language strings; Warp and Mode pick from tables, never a switch; no magic literals (256, 4, 7, 0.45, 0.9, 16 named).

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green (after the library shift has landed `Levels`, `Stride` and core/stereo).
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::WAVETABLE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note; a chord sounds, wider than before (Unison 2, Width); a C7 on Table1 Saw at Position1 63 is clean, no inharmonic whistle, and a C2 brighter than before (levels, 256 partials); Warp Sync at Amount 0.6 sweeps a formant; Mode High thins; a soft note darker than a hard one (Touch).

## Learned

| Method | Source | Licence | Here |
|---|---|---|---|
| per-octave levels by a spectral cut, the level from the note's step | VCV Fundamental `src/Wavetable.hpp`, `src/WTVCO.cpp` | GPL-3.0+ | this round (core) |
| levels a minor third apart from harmonic coefficients | Odin 2 `WavetableContainer.cpp`, `WavetableOsc1D.cpp` | GPL-3.0 | read; octaves chosen |
| time-domain mip levels, each half the length | Surge XT `src/common/dsp/Wavetable.cpp` | GPL-3.0 | this round (the halving) |
| integrated tables, differenced on read | Plaits `plaits/dsp/engine/wavetable_engine.cc` | MIT | not taken |
| phase warps: sync, squeeze, pulse, bend | Vital `src/synthesis/producers/synth_oscillator.cpp` | GPL-3.0 | this round |
| phase distortion by an asymmetric knee | Plaits `plaits/dsp/engine2/phase_distortion_engine.cc` | MIT | this round (Knee) |
| sync without aliasing: passes per sample, low-pass, DC block | Odin 2 `WavetableOsc1D.cpp` | GPL-3.0 | later |
| unison: sharp/flat pairs, pan by voice, sum by 1/√N | Vital `synth_oscillator.cpp`; Surge XT `WindowOscillator.cpp`; Helm `helm_oscillators.cpp` | GPL-3.0 | this round |
| per-voice spectral refresh, crossfaded | Vital `synth_oscillator.cpp` | GPL-3.0 | later |
| frames on a grid; four corner tables | Plaits `wavetable_engine.cc`; Odin 2 `VectorOscillator.cpp` | MIT; GPL-3.0 | later (78) |
| spectral frame morph; spectral warps | Vital `wave_source.cpp`, `wave_frame.cpp`, `spectral_morph.h` | GPL-3.0 | later (77) |
| period detection for import | Vital `pitch_detector.cpp`, `file_source.cpp` | GPL-3.0 | after seam S2 |

Cores: core/block, core/stereo
