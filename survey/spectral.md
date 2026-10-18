# spectral — round 1

## Matrix

A cell is `yes` / `no` / `partial` and one word; `?` is unverified. The CPU row reads `low` / `mid` / `high` and how it is known (`est` = estimated from the code's per-sample work, `stated` = the maker's own words). Ours are read from this worktree. Open: Vital, Surge XT, Odin 2, Plaits, Helm, VCV WT-VCO, ZynAddSubFX, Tunefish 4. Popular: Serum 2, Massive X, Pigments, Phase Plant, Harmor, Razor, Alchemy, Loom II, Hive 2, Ableton Wavetable. Rows past the commission's list (partial count, per-partial control, spectral shaping, table import, warp modes, spectral morph, resynthesis, pitch rows, polyphony, velocity, stereo image, programs) are the family's own.

| Capability | additive | wavetable | Vital | Surge XT | Odin 2 | Plaits | Helm | WT-VCO | ZynAddSubFX | Tunefish 4 | Serum 2 | Massive X | Pigments | Phase Plant | Harmor | Razor | Alchemy | Loom II | Hive 2 | Ableton Wavetable |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| oscillator kinds | partial 8-sines | partial 2-tables | yes 3-table+sample | yes 12-types | yes 11-types | yes 16-models | partial 12-waves | partial 1-table | yes ADD+PAD+SUB | yes spectral+noise | yes 5-kinds | yes 2table+2PM | yes 5-engines | yes 5-generators | yes additive | yes additive | yes 5-elements | yes additive+noise | yes 9-waves+tables | yes 2table+sub |
| partial count | partial 8 | partial 32/frame | partial FFT-frames | partial Twist-32 | partial specdraw | yes 32 | partial stacked | no | yes 128 | partial row | no | no | yes 512 | no | yes 516 | yes 320 | yes 600 | yes 512 | no | no |
| per-partial control | partial static-levels | no | partial editor | no | partial specdraw | partial bump | no | no | partial per-voice | no | no | no | partial parametric | no | yes envelopes | yes stereo | yes envelopes | partial modules | no | no |
| spectral shaping (tilt, odd/even, stretch, harmonic filter) | no | partial 16-spectra | yes 11-warps | partial Twist | no | yes bump | no | no | yes harmonic-filter | yes bandwidth+damp | yes spectral-osc | no | yes tilt+parity | no | yes prism | yes filters | ? | yes modules | no | no |
| unison and detune | no | partial 2osc-detune | yes 16 | yes 16 | yes unison | no | yes 15 | no | yes 50 | yes spread | yes ? | yes 6 | yes 8 | yes 18-modes | yes 5-types | ? | yes ? | ? | yes 16 | yes 6-modes |
| wavetable morphing | no | yes 64-frames | yes 256-frames | yes linear | yes linear+vector | yes 8x8 | no | yes linear | partial base-morph | no | yes interpolated | yes 128-frames | yes 256-frames | yes 256-frames | partial timbre-blend | no | partial XY | partial morph-pad | yes 4-modes | yes position |
| table import or drawing | no | no | yes audio+text | yes wt+wav | partial draw | partial user-bank | no | yes wav | partial convert | no | yes import+draw | no | partial wav | yes wav | yes image+audio | no | yes audio | ? | partial script | partial wav |
| warp modes (sync, bend, PD, PWM, fold) | no | no | yes 8 | partial skew+sync | partial sync | yes PD | no | yes sync | yes PM+sync | no | yes dual | yes 10 | yes PD+fold | partial PM/RM | no | partial PWM | ? | ? | ? | yes PW+sync+fold |
| spectral (FFT-domain) morph or filter | no | no | yes 11-warps | no | no | no | no | no | yes harmonic-filter | yes IFFT | yes spectral-osc | no | partial harmonic | no | yes partial-domain | yes partial-domain | yes spectral | yes modules | partial spectral-interp | no |
| resynthesis from audio | no | no | partial period-splice | no | no | no | no | no | partial convert | no | yes harmonic | no | ? | no | yes audio+image | ? | yes additive+spectral | ? | ? | no |
| FM algorithms and operators | no | no | partial osc-FM | yes FM2/FM3 | partial 2op | yes 2op+6op | partial cross | partial TZFM | yes voice-FM | no | partial warp-FM | partial PM | partial engine-FM | yes linear | ? | partial dissonance | partial filter-FM | ? | ? | partial osc-FM |
| filter models | no none | partial ladder | yes 8 | yes 14 | yes 7 | partial LPG | yes SVF-blend | no | yes 5-kinds | yes 4-modes | yes many | yes 9 | yes 19 | yes 2-kinds | yes spectral | yes spectral | yes 39 | partial modules | yes multimode | yes 5-circuits |
| modulation sources and the matrix | no | partial 2env+1lfo | yes 64-slots | yes 12-LFO+MSEG | yes 9-slots | partial CV | yes 3LFO+seq | partial CV | partial fixed | yes matrix | yes 10LFO+matrix | yes any-to-any | yes drag | yes 19-modulators | yes env+LFO | yes env+LFO | yes 4-kinds | yes 3LFO+4env | yes 12-slots | yes matrix |
| voice modes — poly, mono, legato, glide | partial poly | partial poly+glide | yes legato+glide | yes full | yes legato+glide | no mono | yes legato+glide | partial cables | yes poly/mono/legato | partial poly+glide | yes legato+porta | yes legato+glide | yes legato+glide | yes legato+glide | yes legato+porta | partial mono | yes priority | partial glide | yes poly/duo/mono | yes mono+glide |
| polyphony | yes 8 | yes 8 | yes 32 | yes 64 | yes 24 | no 1 | yes 32 | yes 16 | yes 60 | yes row | ? | yes 64 | ? | ? | ? | ? | yes 32 | yes 8 | yes 16 | ? |
| velocity | yes level | partial loudness | yes source | yes source | yes source | partial level | yes source | no | yes per-target | yes source | yes source | yes source | yes source | yes source | yes source | ? | yes source | yes envelopes | yes source | yes source |
| pitch rows (tune, bend, vibrato) | no | yes tune+bend+vibrato | yes bend+tune | yes bend+tune | yes bend+tune | partial CV | yes bend+tune | partial CV | yes bend+tune | yes bend+tune | yes bend+tune | yes bend+tune | yes bend+tune | yes bend+tune | yes bend+tune | ? | yes bend+tune | ? | yes bend+tune | yes bend+tune |
| MPE and aftertouch | no seam | no seam | yes MPE | yes MPE | partial AT | no | partial polyAT | no | partial sysex | no | partial MPE | partial AT | yes MPE | yes MPE | ? | ? | yes MPE | ? | yes MPE | yes MPE |
| microtuning | no 12TET | no 12TET | yes scl+tun | yes scl+MTS | yes scl | no | ? | partial V/oct | yes scl+kbm | no | partial MTS+tun | no | yes scl+tun+MTS | ? | partial scl | ? | ? | ? | yes tun+MTS | yes scl |
| macro controls | no | no | yes 4 | yes 8 | partial XY | yes 3-knobs | ? | no | no | no | yes 8 | yes 16 | yes 4 | yes 8 | ? | yes 8 | yes 8+2XY | partial morph-pad | yes 4XY | no device |
| effects in the instrument | no | no | yes 9 | yes 29-types | yes 5+reverb | no | yes 5 | no | yes 7-types | yes 7 | yes 13 | yes 22-types | yes 20 | yes snapins | yes 5 | yes spectral | yes ? | yes 4 | yes 7 | no |
| oversampling and aliasing | partial truncated | partial one-level | yes FFT-cut | yes BLIT+mips | yes third-tables | yes integrated | yes harmonic-tables | yes FFT-octaves | yes Nyquist-cut | partial short-table | partial Ultra | ? | ? | ? | partial precision | ? | ? | ? | partial modes | partial HQ |
| sample playback and zones | no | no | partial 1-sample | partial window | no | no | no | no | no | no | yes SFZ | no | yes 6-zones | partial 1-sample | partial resynth | no | yes sampler | no | no | no |
| drum synthesis models and layering | no | no | no | partial Twist | no | yes 3-models | no | no | partial kits | no | ? | ? | partial modal | ? | ? | ? | ? | ? | ? | ? |
| vocal formant and glottal models | no | partial vowel-spectra | yes formant | partial Twist | yes formant-filter | yes speech | yes formant-fx | no | yes formant | yes formant-fx | ? | partial formant-mode | partial vocoder | ? | ? | yes formant | yes formant | ? | ? | ? |
| stereo image | no mono | no mono | yes spread | yes scenes | partial unison-pan | partial aux | partial ? | no | yes unison-pan | yes L/R | yes width | yes width | yes partial-pan | ? | yes M/S | yes per-partial | yes width | ? | ? | yes spread |
| programs | no | no | yes 75+ | yes 2800+ | ? | no | ? | no | yes 1100+ | ? | yes 626 | yes 780 | yes 1700+ | ? | ? | ? | ? | yes 850+ | yes 2400+ | ? |
| CPU per voice | low est | low est | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | low stated | partial HQ-25% |

## Lacking

Answering round 0's word: stand beside the open and the popular instruments, family by family; learn the methods; what many need, the library gives once.

### Both instruments (shared, a `core/` matter)

- band-limited tables per pitch: a table holds one level today. It should hold a level per octave (or per minor third), each with only the partials that fit under Nyquist at that level's lowest pitch, chosen by the note's step (Vital, Surge XT, Odin 2, Helm, WT-VCO, ZynAddSubFX). One `CORE::OSCILLATOR::Table` gain serves both instruments and `operator`'s waveforms.
- microtuning: a per-key table from Scala `.scl`/`.kbm` behind `CORE::PHASE::Tuning` (Vital, Surge XT, Odin 2, ZynAddSubFX, Pigments, Hive 2, Ableton Wavetable). The subtractive and operator surveys ask the same.
- unison with detune and stereo spread: `CORE::VOICE` already has `unison`, `detune` and `spread`, but no row of either instrument reaches them (every column but Plaits and WT-VCO).
- stereo out: both write one mono sample to every lane (Vital, Surge XT, Harmor, Razor, Pigments).
- programs: a bank of our own per instrument on the PROGRAM event (Vital 75+, Serum 2 626, Pigments 1700+, Hive 2 2400+).
- macro controls: a few performance macros over the rows (Vital 4, Serum 2 8, Massive X 16, Pigments 4, Hive 2 4 XY).
- effects in the instrument: chorus, delay and reverb over `CORE::LINE` and `CORE::REVERB` (Vital 9, Serum 2 13, Pigments 20, Hive 2 7).
- MPE and aftertouch: seam first. There is no pressure event and no BEND to a plugin, so this is a finding for an engine order.
- table or spectrum from audio (import and resynthesis): seam first. A file or sample must reach a plugin (Vital, Serum 2, Harmor, Alchemy, Phase Plant, Surge XT).

### additive

- the sheet over `CORE::TABLE`, the allocator over `CORE::VOICE`, the envelope over `CORE::ENVELOPE` and the meter over `CORE::BLOCK` (379 law 4). It is still the founding shape: a hand table, three `switch`es, and "Harmonic N" labels composed at run time. This comes before any gain below.
- partial count: 8 → 32 to 64 harmonics with a per-note Nyquist cut (Plaits 32, ZynAddSubFX 128, Razor 320, Pigments 512, Harmor 516).
- spectral shaping as a few rows instead of one row per harmonic: a centre and width bump, a tilt, an odd/even balance and a stretch for inharmonic partials (Plaits, Pigments, Tunefish 4, ZynAddSubFX, Razor).
- spectrum over time: a second spectrum and an envelope or LFO morph between the two, or per-band envelopes (Harmor, Alchemy, Razor, Loom II).
- a spectral filter on the partial gains: a cutoff and resonance applied to the amplitudes before summing (Harmor, Razor, Loom II, ZynAddSubFX harmonic filter).
- bandwidth per partial: the PADsynth ensemble tone, each harmonic smeared and baked into one looping table at create (ZynAddSubFX PAD, Tunefish 4).
- pitch rows: tune, bend, vibrato and glide, as `wavetable` has (every column).
- modulation: at least one LFO and a second envelope routed to the spectral rows (every popular column).
- velocity to brightness as well as level (ZynAddSubFX, Pigments, Harmor).
- an interpolated read, or a recursive sine bank, in place of the truncating 4096-point lookup (Plaits, SuperCollider Klang, Pure Data).
- a filter after the sum, riding `CORE::FILTER` (Pigments, Alchemy, ZynAddSubFX).

### wavetable

- the band-limited levels (shared, above). Today each frame is drawn from 32 partials at one level, so the 32nd partial passes Nyquist at 48 kHz above about 740 Hz (F#5), and the low notes are duller than they need be.
- more partials per frame where the level allows: up to the table's half-length at the lowest level (Vital, Odin 2, WT-VCO).
- unison: up to 8 or 16 voices per oscillator, detune on a curve, a blend row, random start phases and stereo spread (Vital, Surge XT, Serum 2, Hive 2, Massive X, Ableton Wavetable).
- warp modes on the read phase: sync, bend, squeeze, pulse width, mirror and a phase-distortion knee (Vital 8, Massive X 10, Serum 2, Ableton Wavetable, Plaits PD).
- cross-oscillator FM or PM and hard sync between the two sources (Vital, Ableton Wavetable, Phase Plant, Massive X).
- a sub and a noise source beside the two tables (Serum 2, Ableton Wavetable, Pigments, Hive 2).
- spectral morph between frames, blending amplitudes and phases in the spectrum rather than samples in time (Vital, Hive 2, Serum 2 spectral).
- a modulation matrix: per-voice LFOs and a free envelope assignable to position, warp, cutoff and pitch, in place of two fixed routes (Vital 64 slots, Hive 2 12, Ableton Wavetable, Surge XT).
- filter choice and routing: `CORE::FILTER` already holds the state-variable, Sallen-Key, diode and formant models, and only the ladder is ridden (Vital 8, Ableton Wavetable 5 circuits, Massive X 9).
- a vector (2D) position: four tables mixed bilinearly from X and Y (Odin 2 vector, Plaits 8x8).
- velocity to position and cutoff, not only loudness (Vital, Serum 2, Pigments).

## Methods

| Method (own words) | Source and file | Licence | Lands |
|---|---|---|---|
| per-octave levels from each frame's spectrum: transform once, zero the bins above a cutoff that halves per level, transform back; the read takes the level whose top partial fits under Nyquist at the note's step | VCV Fundamental `src/Wavetable.hpp`, `src/WTVCO.cpp` | GPL-3.0+ | core/oscillator `Table` (both; operator) |
| levels a minor third apart, each summed from the frame's harmonic coefficients up to what fits at its lowest pitch; switched at band edges | Odin 2 `Source/audio/Oscillators/WavetableContainer.cpp`, `WavetableOsc1D.cpp` | GPL-3.0 | core/oscillator `Table` |
| per-voice refresh: the chosen frame's spectrum is warped, cut at the pitch's Nyquist and transformed back into a voice buffer, crossfaded into the old one and read cubically | Vital `src/synthesis/producers/synth_oscillator.cpp` | GPL-3.0 (plus README terms) | wavetable (a heavier, later step) |
| time-domain mip levels: each level low-passed by a short symmetric kernel and halved in length; the level from samples spanned per output sample | Surge XT `src/common/dsp/Wavetable.cpp` | GPL-3.0 | core/oscillator `Table` (an alternative to the spectral cut) |
| integrated tables: store the running sum of each wave, blend, difference on playback and scale by the inverse step; the difference is a pitch-tracking low-pass and loudness holds across the keyboard | Plaits `plaits/dsp/engine/wavetable_engine.cc` | MIT | core/oscillator `Table` (cheapest band-limit) |
| frames navigated on a grid: the neighbouring frames read and blended along X, Y and bank; position inputs smoothed by a one-pole low-pass, the bank slowest | Plaits `plaits/dsp/engine/wavetable_engine.cc` | MIT | wavetable (vector position) |
| vector oscillator: four tables at the corners of an XY pad, mixed bilinearly from a smoothed position | Odin 2 `VectorOscillator.cpp` | GPL-3.0 | wavetable |
| phase warps: multiply the phase (sync, with a half-sine window for formant), pinch it (squeeze), clip it (pulse width), floor it (quantise), bend it by a cubic (bend), offset it by another source (FM) | Vital `src/synthesis/producers/synth_oscillator.cpp` | GPL-3.0 | core/oscillator (a `warp` on the read phase: wavetable) |
| phase distortion: an asymmetric triangle bends a sine's phase, the carrier reset in sync; run at 2x and averaged | Plaits `plaits/dsp/engine2/phase_distortion_engine.cc` | MIT | core/oscillator (wavetable warp) |
| sync without aliasing: several internal passes per output sample, then a short symmetric low-pass and a DC blocker | Odin 2 `WavetableOsc1D.cpp` | GPL-3.0 | core/oscillator (rides core/shaper `Blocker`) |
| spectral frame morph: blend magnitudes in the square-root domain and move phase by a fraction of the phase difference, harmonic by harmonic | Vital `src/common/wavetable/wave_source.cpp`, `lookups/wave_frame.cpp` | GPL-3.0 | wavetable `build` (frames between keyframes) |
| spectral warps on a frame: stretch harmonics, smear, shift formant, brick-wall low or high pass, disperse phase, take each harmonic from a different frame | Vital `src/synthesis/producers/spectral_morph.h` | GPL-3.0 | wavetable (a spectral row) |
| unison: voices in sharp/flat pairs on a power curve of the detune, equal-power pan by voice, a blend of centre against the rest, random start phases, an optional frame offset per voice | Vital `synth_oscillator.cpp`; Surge XT `WindowOscillator.cpp`; Helm `src/synthesis/helm_oscillators.cpp` | GPL-3.0 | core/voice `unison`/`spread` (both instruments) |
| unison sum scaled by one over the root of the voice count; a slow drift per voice | Surge XT `WindowOscillator.cpp` | GPL-3.0 | core/voice |
| bump spectral envelope: each partial's gain falls with its distance from a centre, the slope set by a second row, a ripple adding further bumps; gains smoothed per partial by a one-pole and normalised to a fixed sum | Plaits `plaits/dsp/engine/additive_engine.cc` | MIT | additive (centre, width, ripple rows) |
| organ subset: the same bump read only at a sparse set of drawbar-like harmonic indices | Plaits `additive_engine.cc` | MIT | additive (a mode row) |
| sine bank by recurrence: one sine evaluated, the higher harmonics built from the previous two by a Chebyshev-style recurrence, each gain ramped across the block | Plaits `plaits/dsp/oscillator/harmonic_oscillator.h` | MIT | core/oscillator (additive) |
| Nyquist fade: each partial's gain shrinks linearly as its frequency nears Nyquist and is zero beyond | Plaits `harmonic_oscillator.h`; ZynAddSubFX `src/Synth/OscilGen.cpp` | MIT; GPL-2.0+ | core/oscillator (additive, wavetable) |
| sine bank by coupled two-state resonators, one per partial, seeded from phase and amplitude, grouped by four | SuperCollider `server/plugins/OscUGens.cpp` (Klang) | GPL-3.0 | core/oscillator (additive) |
| rotation-form sine oscillators as the reference for recursion against table | Faust `oscillators.lib` | LGPL-2.1+ with exception / STK-4.3 | core/oscillator |
| PADsynth: each harmonic spread over a smooth profile whose width in cents grows with harmonic number, random phase per bin, one inverse transform into a seamless looping table per octave range, read from a random start | ZynAddSubFX `src/Params/PADnoteParameters.cpp`, `src/Synth/PADnote.cpp` | GPL-2.0+ | additive (a bandwidth row; tables built at create) |
| spectral generator: a lobe per harmonic, bandwidth widening up the series, damp lowering the highs, scale stretching the spacing; rebuilt only when a row moves | Tunefish 4 `src/tunefish4/Source/synth/tf4.cpp` | GPL-3.0+ | additive |
| harmonic filter in the spectrum domain: each harmonic's gain multiplied by a filter curve, then renormalised | ZynAddSubFX `src/Synth/OscilGen.cpp` | GPL-2.0+ | additive (spectral filter rows); wavetable `build` |
| harmonic position modes: partials moved off integer ratios by a shift, a power or a sine law, with a pull back toward harmonic | ZynAddSubFX `src/Params/PADnoteParameters.cpp` | GPL-2.0+ | additive (a stretch row) |
| noise through a bandpass per harmonic, gain compensated for bandwidth, harmonics near DC and Nyquist tapered out | ZynAddSubFX `src/Synth/SUBnote.cpp` | GPL-2.0+ | additive (a breath row, later); core/filter |
| Scala tuning: degrees held as base-2 logarithms; the keyboard map gives degree and octave; the reference key pinned to the reference frequency | ZynAddSubFX `src/Misc/Microtonal.cpp` | GPL-2.0+ | core/phase `Tuning` (every instrument) |
| table rebuilds off the audio thread, crossfaded in when ready | Yoshimi (docs; `src/globals.h`) | GPL-2.0+ | additive (if a spectrum grows too costly to settle in render) |
| 4-point cubic read through one point before and two after, with guard points at the table ends; error falls about twice as fast per doubling as linear | Pure Data `src/d_array.c`; Puckette, *Theory and Techniques* ch. 2 | BSD-3-Clause | core/oscillator `read` |
| period detection for import: a coarse whole-sample search, then a fractional refinement on a squared-difference cost; each period becomes one frame | Vital `src/common/wavetable/pitch_detector.cpp`, `file_source.cpp` | GPL-3.0 | wavetable (after the seam) |
| partials with time-varying frequency, amplitude and noisiness for resynthesis and morphing | Loris (search results only) | GPL-2.0+ | additive (after the seam) |

No line, table, constant or coefficient is taken from any of these; each row is the idea in this survey's words.

## Sources

| Name | Licence | Home | Read |
|---|---|---|---|
| Vital | GPL-3.0 (README adds name and store terms; presets separate) | https://github.com/mtytel/vital | README; LICENSE; vital.audio; `lookups/wavetable.{h,cpp}`, `lookups/wave_frame.cpp`, `producers/spectral_morph.h`, `producers/synth_oscillator.cpp`, `common/wavetable/file_source.cpp`, `wave_source.cpp`, `pitch_detector.cpp`, `synth_constants.h` |
| Surge XT | GPL-3.0 | https://github.com/surge-synthesizer/surge | LICENSE; README; manual-xt; `WavetableOscillator.cpp`, `WindowOscillator.cpp`, `Wavetable.cpp`, `TwistOscillator.cpp`, `AliasOscillator.cpp` |
| Odin 2 | GPL-3.0 | https://github.com/TheWaveWarden/odin2 | README; LICENSE; thewavewarden.com/pages/odin-2; `WavetableContainer.cpp`, `WavetableOsc1D.cpp`, `WavetableOsc2D.cpp`, `VectorOscillator.cpp`, `MultiOscillator.cpp`, `ChiptuneOscillator.cpp`, `GlobalIncludes.h`; KVR 2.3 post |
| Mutable Instruments eurorack (Plaits, Braids) | MIT (STM32 code, per README) | https://github.com/pichenettes/eurorack | README; Plaits and Braids manuals; `wavetable_engine.cc`, `additive_engine.cc`, `harmonic_oscillator.h`, `engine2/wave_terrain_engine.cc`, `engine2/phase_distortion_engine.cc` |
| Helm | GPL-3.0 | https://github.com/mtytel/helm | README; COPYING; `mopo/src/wave.h`; `src/synthesis/helm_oscillators.cpp` |
| VCV Fundamental (WT-VCO) | GPL-3.0+ (code); panels CC BY-NC-ND 4.0 | https://github.com/VCVRack/Fundamental | LICENSE.md; `src/WTVCO.cpp`, `src/Wavetable.hpp` |
| ZynAddSubFX | GPL-2.0+ | https://github.com/zynaddsubfx/zynaddsubfx | README; `globals.h`, `OscilGen.cpp`, `ADnote.cpp`, `SUBnote.cpp`, `PADnote.cpp`, `PADnoteParameters.cpp`, `ADnoteParameters.cpp`, `Microtonal.cpp` |
| Yoshimi | GPL-2.0+ | https://github.com/Yoshimi/yoshimi | README; `src/globals.h`; yoshimi.sourceforge.io; man page (not a column: Zyn's engine) |
| Tunefish 4 | GPL-3.0+ | https://github.com/paynebc/tunefish | README; COPYING; `src/tunefish4/Source/synth/tf4.cpp`, `tf4fx.cpp` |
| SuperCollider | GPL-3.0 | https://github.com/supercollider/supercollider | Klang, DynKlang, VOsc, Wavetable and Signal help; `server/plugins/OscUGens.cpp` (not a column: a language) |
| Pure Data vanilla | BSD-3-Clause | https://github.com/pure-data/pure-data | LICENSE.txt; `src/d_osc.c`, `src/d_array.c`; Puckette, *Theory and Techniques* (msp.ucsd.edu/techniques.htm) nodes 27, 31 (not a column) |
| Faust libraries | LGPL-2.1+ with exception; STK-4.3 by function | https://github.com/grame-cncm/faustlibraries | `oscillators.lib` doc page and header (not a column) |
| Loris | GPL-2.0+ | https://github.com/tractal/loris | search results only (not a column) |
| WaveEdit | GPL-3.0 | https://github.com/AndrewBelt/WaveEdit | repo page (an editor for hardware tables; not a column) |
| Xfer Serum 2 | commercial | https://xferrecords.com/products/serum-2 | product page; "What's New in Serum 2" PDF; web-manual page; CPU guidelines article |
| Native Instruments Massive X | commercial | https://www.native-instruments.com/en/products/komplete/synths/massive-x/ | product and details pages; online manual (voice, wavetable oscillators, insert and stereo effects, filter) |
| Arturia Pigments | commercial | https://www.arturia.com/products/software-instruments/pigments/overview | overview; Pigments 7 manual (parts) |
| Kilohearts Phase Plant | commercial | https://kilohearts.com/products/phase_plant | product page; docs (Phase Plant, modulation) |
| Image-Line Harmor | commercial | https://www.image-line.com/fl-studio/plugins/harmor/ | FL Studio online manual, Harmor page |
| Native Instruments Razor | commercial | https://www.native-instruments.com/en/products/komplete/synths/razor/ | product page |
| Apple Alchemy | commercial | https://support.apple.com/guide/logicpro/alchemy-overview-lgsi2618652a/mac | Logic Pro guide pages, seen as search excerpts only |
| AIR Loom II | commercial | https://www.airmusictech.com/virtual-instruments/loom-ii/ | product page |
| u-he Hive 2 | commercial | https://u-he.com/products/hive/ | product page |
| Ableton Wavetable | commercial | https://www.ableton.com/en/manual/live-instrument-reference/ | Live manual §Wavetable; Live 12 release notes and tuning FAQ (excerpts) |
| Waldorf Iridium | commercial (hardware) | https://waldorfmusic.com/iridium-en/ | product page (not a column: hardware, the page too thin) |

## Moved

none
