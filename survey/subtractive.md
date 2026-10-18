# subtractive — round 1

## Matrix

A cell is `yes` / `no` / `partial` and one word; `?` is unverified. The CPU row reads `low` / `mid` / `high` and how it is known (`est` = estimated from the code's per-sample work, `stated` = the maker's own words). Ours are read from this worktree. Open: Surge XT, Vital, Odin 2, OB-Xd (the 2DaT GPL source), OB-Xf, amsynth, Monique, Helm, Open303. Popular: Diva, Repro (-1/-5), Hive 2, Zebra 2, the V Collection, Roland Cloud, the Korg Collection.

| Capability | acid | dco | ladder | mono | duo | multimode | polymod | subtractive | supersaw | pads | pluck | ensemble | Surge XT | Vital | Odin 2 | OB-Xd | OB-Xf | amsynth | Monique | Helm | Open303 | Diva | Repro | Hive 2 | Zebra 2 | V Coll. | Roland Cloud | Korg Coll. |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| oscillator kinds | partial saw/square | yes saw+pulse+sub | yes 3vco | yes saw+pulse+sub | yes 2vco | yes 2vco | yes 2osc | partial naive | partial 7saw | partial swept | partial burst | partial divider | yes 12-algos | yes wavetable | yes 11-modules | yes VA | yes VA | yes VA | yes morph | yes 12-waves | partial saw/square | yes 5-models | yes 2vco | yes 9+table | yes 4osc+4fmo | yes models | yes models | yes MOSS |
| oscillator anti-aliasing | yes blep | yes blep | yes blep | yes blep | yes blep | yes blep | yes blep | no naive | yes blep | yes sine | yes loop | yes blep | yes BLIT | yes tables | yes tables | yes BLEP | yes BLEP | partial ad-hoc | yes BLIT | yes tables | yes mipmap | ? unstated | partial HQ | ? unstated | partial resolution | yes TAE | partial ACB | partial CMT |
| sync, ring, cross-mod | no | no | partial osc3 | no | yes sync+ring | partial sync | yes polymod | no | no | no | no | no | yes all | yes all | yes all | partial sync+xmod | yes all | partial sync+ring | partial sync+FM | partial xmod | no | yes all | yes polymod | no | yes all | yes polymod | partial sync+xmod | yes all |
| unison and detune | no | no | partial 3osc | no | partial fine | partial 2 | yes 5 | partial 2osc | yes 7x4 | no | no | partial chorus | yes 16 | yes 16 | yes ? | yes 8 | yes 8 | no | no | yes 15 | no | yes stack | yes 8 | yes 16 | yes 11 | yes 8 | partial unison | yes 16 |
| wavetable morphing | no | no | no | no | no | no | no | no | no | no | no | no | yes 512 | yes 257 | partial 4-frame | no | no | no | partial crossfade | no | partial blend | no | no | yes 256 | yes 16 | no | no | no |
| FM algorithms and operators | no | no | no | no | no | no | partial polymod | no | no | no | no | no | yes 4op | partial cross | partial 2op | partial xmod | partial xmod | no | partial FM-osc | partial xmod | no | partial linear | partial polymod | no | yes 4fmo | partial xmod | partial xmod | partial VPM |
| filter models | yes diode | yes ladder | yes ladder | yes ladder | yes SVF/ladder | yes SVF-morph | yes ladder | partial chamberlin | yes ladder | partial onepole | partial loop | partial fixed-LP | yes 13 | yes 8 | yes 11 | yes 2pole+4pole | yes Xpander | partial biquad | yes ladder | yes SVF | partial TB | yes 5 | partial 4-flavours | yes 3-engines | yes 23 | yes per-model | yes per-model | yes revisions |
| filter routing | no | partial HPF | no | no | partial HPF | no | no | no | partial HPF | no | partial body | partial registers | yes 8 | yes serial/parallel | yes serial/parallel | no | no | no | yes 3 | partial serial | no | partial HPF | no | yes serial/parallel | yes grid | partial serial | partial HPF | partial dual |
| envelopes | partial AD | partial 1 | yes 2 | partial 1 | yes 2 | yes 2 | yes 2 | yes 2 | yes 2 | partial AD | partial decay | partial AR | yes 2+MSEG | yes 6 | yes 4 | yes 2 | yes 2 | yes 2 | yes 4+ | yes 3 | partial decay | yes 2 | yes 2 | yes 4+2 | yes 4+MSEG | yes multi | yes 2 | yes 4+ |
| LFOs | no | yes 1 | partial osc3 | yes 1 | yes 1+S&H | yes 1 | yes 1 | no | no | no | no | partial fixed | yes 12 | yes 12 | yes 4 | yes 1 | yes 2 | yes 1 | yes 7 | yes 3 | no | yes 2 | yes 1 | yes 3 | yes 6 | yes 3 | yes 1 | yes 4 |
| modulation sources and the matrix | no | no | no | no | partial switches | no | partial polymod | no | no | no | no | no | yes unlimited | yes 64 | yes 9 | no | yes 8 | no | partial toggles | yes drag | no | partial selectors | yes 4 | yes 12 | yes 12 | yes advanced | no | yes patches |
| voice modes — poly, mono, legato, glide | partial slide | partial poly | partial mono | partial mono | partial duo | partial poly+unison | yes poly+unison | partial poly | partial poly+glide | partial drum | partial poly | partial paraphonic | yes full | yes full | yes basic | yes 4-legato | yes priority | yes poly/mono | partial mono | yes legato | partial slide | yes 5 | yes poly/unison | yes 4 | yes 5 | yes legato | yes 4 | yes duophonic |
| polyphony | no 1 | yes 6 | no 1 | no 1 | partial 2 | partial 2 | yes 5 | yes 8 | yes 8 | yes 6-pads | yes 16 | yes 128-keys | yes 64 | yes 32 | yes 24 | yes 8 | yes 32 | yes configurable | no 1 | yes 32 | no 1 | yes 16 | yes 8 | yes 16 | yes 16 | yes 36 | yes 8 | yes 128 |
| analog drift, per-voice variance | no | no | no | no | no | no | no | no | no | no | no | no | yes drift | partial random | yes drift | yes slop | yes variation | no | ? | no | no | yes trimmers | yes trimmers | partial random | partial drift | yes dispersion | yes condition | ? |
| velocity | partial accent | no | no | no | no | no | no | yes amp | no | yes level | yes tone | no | yes source | yes source | yes source | yes amp/filter | yes amp/filter | yes amp/filter | ? | yes source | partial accent | yes source | yes source | yes source | yes source | yes source | ? | yes source |
| MPE and aftertouch | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | yes MPE | yes MPE | partial channel-AT | no | partial MPE | no | no | partial poly-AT | no | yes MPE | yes MPE | yes MPE | partial poly-AT | yes MPE | ? | ? |
| microtuning | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | no 12TET | yes scl+MTS | partial scl | partial scl | no | partial MTS | yes scl+MTS | partial MTS | no | no | yes tun+MTS | yes tun+MTS | yes tun+MTS | yes tun+MTS | yes MTS | ? | ? |
| macro controls | no | no | no | no | no | no | no | no | no | no | no | no | yes 8 | yes 4 | partial XY | no | no | no | yes morph | no | no | no | no | yes XY | yes XY | yes 4 | partial circuit-mod | ? |
| effects in the instrument | no | yes chorus | no | no | no | no | no | no | no | no | partial body | yes ensemble | yes 29 | yes 9 | yes 5 | no | no | yes 3 | yes 5 | yes 5 | no | yes 5 | yes 6 | yes 7 | yes grid | yes 4-slots | yes 3 | yes 21 |
| oversampling and aliasing (nonlinear stages) | no none | no none | no none | no none | no none | no none | no none | no none | no none | no none | no none | no none | yes 2x | yes 1-8x | partial 3x | yes 2x | yes 2x | no | no | no | yes 4x | yes divine | yes HQ | partial dirty | partial resolution | ? | partial rate | ? |
| sample playback and zones | no | no | no | no | no | no | no | no | no | no | no | no | partial wav-table | partial 1-sample | no | no | no | no | no | no | no | no | no | partial wav-table | no | no | partial ZENOLOGY | ? |
| drum synthesis models and layering | no | no | no | no | no | no | no | no | no | yes 6-pads | no | no | partial Twist | no | no | no | no | no | no | no | no | no | no | partial comb | partial comb | no | partial kits | ? |
| vocal formant and glottal models | no | no | no | no | no | no | no | no | no | no | no | no | partial Twist | partial formant | partial formant | no | no | no | no | partial vowel | no | no | no | no | partial formant | no | no | no |
| stereo image | no | yes chorus | no | no | no | yes pan | no | no | yes width | no | no | yes 3-line | yes fx | yes fx | yes fx | yes voice-pan | yes voice-pan | yes reverb | yes fx | yes fx | ? | yes fx | yes voice-pan | yes fx | yes fx | yes fx | yes chorus | yes fx |
| programs (presets) | no | no | no | no | no | no | no | no | no | no | no | no | yes 2800+ | yes 75+ | yes bank | yes FXB | ? | yes hundreds | ? | yes bank | no | yes 1200+ | yes 500+ | yes 2400+ | yes 500+ | yes 12000+ | yes banks | yes 200+ |
| arpeggiator or sequencer inside | no | no | no | no | no | no | no | no | no | no | no | no | partial step-LFO | no | yes arp+seq | no | no | no | yes 16-step | yes arp+seq | yes acid-seq | yes arp | partial R-1 | yes arp+seq | yes arp | yes arp+seq | yes TB-seq | yes arp |
| CPU per voice | low est | low est | low est | low est | low est | low est | low est | low est | mid est | low est | low est | mid est | mid ? | mid ? | high ? | low est | low est | low est | high est | mid est | low est | high stated | high stated | low stated | low stated | ? | ? | ? |

## Lacking

Answering round 0's word — stand beside the open and the popular instruments, family by family; what many need, the library gives once.

### Every instrument (shared — a `core/` matter)

- microtuning — all twelve: a per-key tuning table loaded from Scala `.scl`/`.kbm` (amsynth, Surge XT, Diva, Repro, Hive, Zebra; MTS-ESP in most) where `CORE::PHASE::hertz` is fixed twelve-tone equal temperament.
- MPE and aftertouch — all twelve: per-note pitch, pressure and timbre (Surge XT, Vital, Diva, Repro, Hive, the V Collection). The seam has no pressure event and sound sends no BEND, so this is a finding for an engine order first.
- analog drift and per-voice variance — dco, ladder, mono, duo, multimode, polymod, supersaw, ensemble: slow per-voice pitch, cutoff and envelope-time offsets (OB-Xd, Diva, Repro, the V Collection, Roland Cloud).
- velocity — dco, ladder, mono, duo, multimode, polymod, supersaw, ensemble: velocity to amplifier and filter depth. Every competitor has it, but `CORE::ENVELOPE` leaves velocity depth at 0.
- programs — all twelve: a factory bank of our own on the PROGRAM event (every popular competitor ships hundreds).
- oversampling of the nonlinear stages — acid, dco, ladder, mono, duo, polymod, supersaw: the ladder and diode soft-clip runs unoversampled. Others run it at 2x (Surge XT, OB-Xd) or 4x (Open303), and Diva and Repro offer a quality switch.
- macro controls — all but pads: a few performance macros over the rows (Surge XT, Vital, Hive, Zebra, the V Collection).
- tempo-synced LFOs — dco, mono, duo, multimode, polymod: they wait on TRANSPORT to plugins (379m).
- control-rate coefficients — acid, dco, ladder, mono, duo, multimode, polymod: they re-settle the filter every sample. A block-rate or interpolated settle (supersaw already does it every 16 frames) frees CPU for the rows above.

### acid

- pattern sequencer with accent and slide per step (Open303, Roland TB-303, the V Collection's Acid V), or a proven `steps` recipe that writes them.
- the TB-specific filter: a high-pass inside the resonance feedback and resonance tuned against cutoff (Open303). Ours is the generic diode ladder.
- separate normal and accent decay, and an output drive stage (JC303's modded-unit switches, Roland TB-303).

### dco

- voice modes: mono, unison and two poly assignments, plus portamento (Roland JUNO-106 software).
- the chain's own four-pole OTA filter model in place of the generic ladder (Roland Cloud's modelled filter, the V Collection's Jun-6 V).

### ladder

- a nonlinear ladder with saturation per stage (Huovilainen ladder in Surge XT, VCV, Diva). Ours clips once at the input.
- bass compensation as emphasis rises (the V Collection's Mini V).
- an added modulation page (LFO, matrix) beside the panel (the V Collection's advanced panels).

### mono

- the chain's own OTA filter model in place of the generic ladder (Roland SH-101 software).
- arpeggiator or sequencer inside, the panel's own (Roland SH-101 software arp). The `arpeggiator` and `steps` plugins exist beside it.

### duo

- a third filter revision and a drive switch (Korg ARP Odyssey Rev1/2/3, DRIVE).
- per-voice drift between its two voices (Korg Collection, the V Collection).

### multimode

- polyphony beyond two: the chain's eight-voice form (the V Collection's SEM V, 8).
- nonlinear resonance in the state-variable filter (the OB-Xd two-pole with a curve in its resonance path).
- a modulation matrix of a few slots (the V Collection's SEM V drag-and-drop slots).

### polymod

- per-voice trimmers: detune, cutoff and envelope spread per voice slot (Repro, OB-Xd).
- filter flavours or a drive on the ladder (Repro's four flavours).
- a small modulation matrix and effects (Repro-5: 2×2 slots, five effects).

### subtractive

- re-founded on the cores: a `CORE::TABLE` sheet, `CORE::VOICE`, `CORE::ENVELOPE`. Today it is the exemplar outside the library's laws, with its own rows and six `switch`es.
- anti-aliased oscillators (polyBLEP like its siblings). Its oscillators are naive.
- filter modes beyond the Chamberlin low-pass, with keyboard tracking.
- an LFO, glide and a mono mode (every competitor).

### supersaw

- LFOs and a modulation route. It has none, where the JP-style chain had them (the V Collection's Jup-8000 V, Hive).
- a second oscillator or feedback wave beside the seven saws (Jup-8000 V).
- filter modes: low, band and high, 12 and 24 dB (Jup-8000 V, Hive). Ours is a low-pass ladder only.
- effects in the instrument: chorus and delay (Jup-8000 V, Hive, Vital).

### pads

- per-pad pan and a stereo out (the drum competitors carry it; see the drums survey).
- per-pad pitch-envelope depth beyond Bend, and a choke group. These are drum rows; the drums survey is their home.

### pluck

- stiffness and dispersion: an allpass stretch in the loop (STK's stiff string, Mutable's Rings).
- an exciter choice beyond the noise burst (Rings, Korg Prophecy's plucked model).
- pitch modulation (vibrato, glide) and a stereo image.

### ensemble

- ensemble mode switch and a resonator or formant bank after the sections (the V Collection's Solina V MK1/MK2, 3-band resonator).
- a polyphonic envelope option beside the paraphonic one (Solina V).
- brass and horn sections beside the strings (Solina V).

## Methods

Ideas only; no line, table or coefficient is taken. A permissive source (MIT, BSD, 0BSD, STK) may be cited by name in a `///` contract. GPL/LGPL and papers are read for the method alone.

| Method (our words) | Source and file | Licence | Lands |
|---|---|---|---|
| TPT/ZDF one-pole and state-variable filter: trapezoidal integrators with the loop solved, no unit delay; cheap tan prewarp approximations | Mutable stmlib `dsp/filter.h`; Zavalishin, *The Art of VA Filter Design* | MIT; copyright | core/filter (`Pole`, `Variable`, `warped`) |
| nonlinear ladder: saturation per stage, 2x oversampled, half-sample delay in the feedback to fix tuning | Huovilainen DAFx-04; VCV Fundamental `src/VCF.cpp`; sst-filters `VintageLadders.h` | copyright; GPL-3.0; GPL-3.0 | core/filter `Ladder` (dco, ladder, mono, polymod, supersaw, duo) |
| ladder integrated by Runge–Kutta with cubic clip per stage, as a second model | sst-filters `include/sst/filters/VintageLadders.h` | GPL-3.0 | core/filter |
| diode ladder: coupled stages, closed-form zero-delay solve, saturation inside the loop; the bassline's halved bottom stage | Stinchcombe "Diode Ladder Filters"; Faust `vaeffects.lib` `diodeLadder` | copyright; STK-4.3 | core/filter `Diode` (acid) |
| bassline filter voicing: a one-pole high-pass inside the resonance feedback, resonance gain fitted against cutoff | Open303 `Source/DSPCode/rosic_TeeBeeFilter.h` | MIT | acid |
| accent as a second RC smoother on the decay envelope summed in octaves, accent shortening decay; slide only when a note arrives over a held one | Open303 `rosic_Open303.h`, `rosic_Open303.cpp` | MIT | acid |
| run oscillator and filter at 4x behind an elliptic quarter-band decimator | Open303 `rosic_Open303.h` | MIT | core/shaper `Oversampler` (acid, then the ladder users) |
| whole-voice 2x: each voice rendered twice per output frame, then one half-band decimator | 2DaT Obxd `Source/Engine/Motherboard.h`; Surge `src/common/globals.h` | GPL-3.0 | core/shaper, core/block |
| two-pole OB-style SVF with a curve in the resonance path; one low↔high crossfade passing notch, band on a switch | 2DaT Obxd `Source/Engine/Filter.h`; Faust `vaeffects.lib` `oberheim` | GPL-3.0; STK-4.3 | core/filter `Variable` (multimode, duo) |
| pole-mix modes: weighted sums of input and the four pole taps give fifteen responses from one ladder | OB-Xf `src/engine/Filter.h` | GPL-3.0 | core/filter `Ladder` (a mode row for its users) |
| per-voice variance: offsets for level, envelope times, cutoff and glide drawn once per voice slot, scaled by a few global rows | 2DaT Obxd `Source/Engine/ObxdVoice.h`; Repro trimmers (manual) | GPL-3.0; manual | core/voice `Note` (dco, polymod, multimode, duo, supersaw) |
| drift as a random walk: noise through a leaky one-pole per voice; or a linear glide between random targets | sst-basic-blocks `OscillatorDriftUnisonCharacter.h`; Odin 2 `Source/audio/Oscillators/DriftGenerator.cpp` | GPL-3.0; GPL-3.0 | core/modulator |
| unison: linear detune spread, pan alternating by distance from centre, 1/√N gain, random start phase but the centre | sst-basic-blocks `OscillatorDriftUnisonCharacter.h`; Helm `src/synthesis/helm_oscillators.cpp` | GPL-3.0; GPL-3.0 | core/voice `stack` (supersaw, polymod, multimode) |
| allocator: reuse the voice holding the note, else the least-recently-used free voice, else steal the oldest released, then the oldest | stmlib `algorithms/voice_allocator.h`; Odin 2 `Source/audio/Voice.h`; amsynth `src/core/synth/VoiceAllocationUnit.cpp` | MIT; GPL-3.0; GPL-2.0 | core/voice `POLYPHONY` |
| crossfade a stolen voice within the block; free a voice once its level falls below a floor | Faust `architecture/faust/dsp/poly-dsp.h` | LGPL-2.1+ w/ exception | core/voice, core/envelope `choke` |
| mono modes and portamento: priority last/low/high, constant-time vs constant-rate glide with curve choice, glissando | Surge `src/common/dsp/SurgeVoice.cpp`; Yarns `part.h` | GPL-3.0; MIT | core/voice `Glide` |
| Scala `.scl` (cents or ratios) and `.kbm` (map size, reference key and frequency, per-key degree) into a per-key frequency table | Scala format pages; amsynth `src/core/synth/TuningMap.cpp`; Vital `src/common/tuning.cpp` | spec; GPL-2.0; GPL-3.0 | core/phase `Tuning` (all) |
| a runtime per-note tuning query from a tuning master | ODDSound MTS-ESP `Client/libMTSClient.h` | 0BSD | core/phase — a third-party client, the developer's call |
| per-note expression: one member channel per note carrying bend, pressure and timbre | MIDI Association MPE; Surge `SurgeVoice.cpp` | spec; GPL-3.0 | the seam first (engine order), then core/voice `Note` |
| polyBLEP and integrated BLAMP placed at the fractional crossing; hard sync evaluated at the sub-sample reset with BLEP and BLAMP | stmlib `dsp/polyblep.h`; Plaits `dsp/oscillator/variable_shape_oscillator.h` | MIT | core/oscillator (subtractive's naive oscillators; `Sync`) |
| minimum-phase BLEP from a stored step table mixed through a ring at fractional offsets; minBLAMP for corners | VCV Rack `include/dsp/minblep.hpp`; Fundamental `src/VCO.cpp` | GPL-3.0+ | core/oscillator (a higher-quality option) |
| parameter interpolation across a block; coefficients settled at control rate | stmlib `dsp/parameter_interpolator.h` | MIT | core/modulator `Smoother`, core/filter (the per-sample settles) |
| stiff Karplus–Strong: allpass fractional tuning, pick-position comb, an allpass cascade for stiffness, loop gain by pitch | STK `src/StifKarp.cpp`, `src/Plucked.cpp` | STK (MIT-like) | pluck, core/line |
| string loop with allpass stretch and jitter, damping set by brightness, delay compensated for the loop filters' lag, low notes by resampling | Mutable Rings `dsp/string.cc`; Plaits `dsp/physical_modelling/string.cc` | MIT | pluck |
| extended Karplus–Strong: pick-direction low-pass, damping filter, dynamic-level low-pass by velocity | J.O. Smith, *Physical Audio Signal Processing* | copyright | pluck |
| bucket-brigade line as a fixed-length delay at a varying clock, with input and output filters split into parallel one-poles in impulse-invariant form | Holters & Parker DAFx-18 | copyright | core/line (dco chorus, ensemble) |
| multi-segment envelope of typed curves with one-shot, loop and gated-loop modes | Surge `src/common/dsp/modulators/MSEGModulationHelper.cpp` | GPL-3.0 | core/envelope `SEGMENTS` |
| modulation matrix: source→destination slots with amount and a second scaling source | Odin 2 manual `ModulationMatrix.tex`; Zebra 2 manual ("via"); OB-Xf `src/engine/Motherboard.h` | GPL-3.0; manual; GPL-3.0 | new core/matrix (dco, multimode, polymod, supersaw) |
| morph groups: rows held as two states and blended by one macro | Monique `src/core/monique_core_Datastructures.h` | GPL-3.0 / MIT dual | core/table (a macro over rows) |
| one quality row switching oversampling and model accuracy together | Diva manual (draft…divine); Hive 2 manual (clean/normal/dirty) | manual | core/shaper (an accuracy row per instrument) |

## Sources

| Name | Licence | Home | What was read |
|---|---|---|---|
| Surge XT | GPL-3.0 | https://github.com/surge-synthesizer/surge | manual-xt; `ClassicOscillator.cpp`, `SurgeVoice.cpp`, `QuadFilterChain.cpp`, `globals.h`, `MTSESPTuning.cpp`, `OscillatorCommonFunctions.h`, `TwistOscillator.cpp`, `MSEGModulationHelper.cpp` |
| sst-filters, sst-basic-blocks | GPL-3.0 | https://github.com/surge-synthesizer/sst-filters | `VintageLadders.h`; `OscillatorDriftUnisonCharacter.h` |
| Vital | GPL-3.0 | https://github.com/mtytel/vital | vital.audio; `synth_oscillator.h`, `synth_filter.h`, `ladder_filter.cpp`, `formant_filter.cpp`, `vocal_tract.cpp`, `upsampler.cpp`, `synth_voice_handler.cpp`, `filters_module.cpp`, `random_lfo_module.cpp`, `wavetable.h`, `synth_constants.h`, `synth_parameters.cpp`, `tuning.cpp` |
| Odin 2 | GPL-3.0 | https://github.com/TheWaveWarden/odin2 | thewavewarden.com feature pages; manual `.tex` sources; `change_log.md`; `OdinConstants.h`, `WavetableOsc1D.cpp`, `DriftGenerator.cpp`, `LadderFilter.cpp`, `FormantFilter.cpp`, `Voice.h`, `PluginProcessor*.cpp` |
| OB-Xd | GPL-3.0 (≤2.11; later binary-only) | https://github.com/2DaT/Obxd (reales/OB-Xd README) | `Motherboard.h`, `ObxdVoice.h`, `Filter.h`, `Lfo.h`, `SynthEngine.h` |
| OB-Xf | GPL-3.0 | https://github.com/surge-synthesizer/OB-Xf | README; `SawOsc.h`, `Filter.h`, `Decimator.h`, `Voice.h`, `Tuning.h`, `Motherboard.h`, `SynthEngine.h`, `configuration.h` |
| amsynth | GPL-2.0 | https://github.com/amsynth/amsynth | COPYING; `TuningMap.cpp`, `Oscillator.cpp`, `LowPassFilter.cpp`, `VoiceAllocationUnit.cpp`, `VoiceBoard.cpp`, `Synthesizer.cpp`, `MidiController.cpp`, `controls.h`, `Configuration.cpp` |
| Monique | GPL-3.0 / MIT | https://github.com/surge-synthesizer/monique-monosynth | README; `monique_core_Synth.h/.cpp`, `monique_core_Datastructures.h` |
| Helm | GPL-3.0 | https://github.com/mtytel/helm | `wave.h`, `state_variable_filter.cpp`, `formant_manager.cpp`, `fixed_point_wave.h`, `helm_voice_handler.cpp`, `helm_oscillators.cpp` |
| Open303 | MIT | https://github.com/RobinSchmidt/Open303 | `License.txt`; `rosic_TeeBeeFilter.h`, `rosic_Open303.h/.cpp`, `rosic_BlendOscillator.h`, `rosic_AnalogEnvelope.h`, `rosic_AcidSequencer.h` |
| JC303 | GPL-3.0 (bundled open303: MIT) | https://github.com/midilab/jc303 | LICENSE; `JC303.cpp`; `dfl_DiodeLadderFilter.h` (no licence header — not cited as a method source) |
| Mutable Instruments eurorack, stmlib | MIT | https://github.com/pichenettes/eurorack | `stmlib/dsp/polyblep.h`, `filter.h`, `parameter_interpolator.h`, `algorithms/voice_allocator.h`; Plaits `variable_saw_oscillator.h`, `variable_shape_oscillator.h`, `virtual_analog_engine.cc`, `string.cc`; Rings `string.cc`; Braids oscillators; Yarns `part.h`, `just_intonation_processor.cc` |
| Faust libraries | LGPL-2.1+ w/ exception, MIT, STK-4.3 per function | https://github.com/grame-cncm/faustlibraries | `vaeffects.lib`, `oscillators.lib`, `filters.lib`, `physmodels.lib`, `basics.lib`; faust `poly-dsp.h` |
| STK | STK licence (MIT-like) | https://github.com/thestk/stk | LICENSE; `StifKarp.cpp/.h`, `Plucked.cpp/.h`, `Voicer.cpp` |
| VCV Rack, Fundamental; Cardinal | GPL-3.0+ | https://github.com/VCVRack/Fundamental | `VCO.cpp`, `VCF.cpp`; Rack `minblep.hpp`, `midi.hpp`, `MIDI_CV.cpp`; Cardinal README |
| ODDSound MTS-ESP | 0BSD | https://github.com/ODDSound/MTS-ESP | LICENSE; `Client/libMTSClient.h` |
| Scala format | spec (© Op de Coul) | https://www.huygens-fokker.org/scala/scl_format.html | `.scl` format page; help (mappings) |
| MPE | MIDI Association spec | https://midi.org/midi-polyphonic-expression-mpe | overview page |
| Zavalishin, *The Art of VA Filter Design* 2.1.2 | copyright | https://www.discodsp.net/VAFilterDesign_2.1.2.pdf | book (TPT, ladder, diode, Sallen-Key, nonlinear ZDF) |
| Huovilainen, nonlinear Moog ladder (DAFx-04) | copyright | https://dafx.de/paper-archive/2004/P_061.PDF | paper |
| Stilson & Smith, Moog VCF analysis (ICMC 1996) | copyright | https://ccrma.stanford.edu/~stilti/papers/moogvcf.pdf | paper |
| Stinchcombe, diode ladder filters | copyright | https://www.timstinchcombe.co.uk/index.php?pge=diode | analysis page |
| Holters & Parker, BBD model (DAFx-18) | copyright | https://dafx.de/paper-archive/2018/papers/DAFx2018_paper_12.pdf | paper |
| Raffel & Smith, BBD circuits (DAFx-10) | copyright | https://dafx.de | archive page |
| J.O. Smith, *Physical Audio Signal Processing* | copyright | https://ccrma.stanford.edu/~jos/pasp/Extended_Karplus_Strong_Algorithm.html | KS and extended KS pages |
| string-ensemble practitioner posts | none stated | https://www.kvraudio.com/forum/viewtopic.php?p=2623291 | forum posts (three-phase BBD ensemble) |
| u-he Diva | commercial | https://u-he.com/products/diva/ | product page; user guide (2025) |
| u-he Repro-1 / Repro-5 | commercial | https://u-he.com/products/repro/ | product page; Repro-5 user guide 1.1.3 |
| u-he Hive 2 | commercial | https://u-he.com/products/hive/ | product page; user guide |
| u-he Zebra 2 (Zebra 3 noted) | commercial | https://u-he.com/products/zebra-legacy/ | product page; Zebra 2 user guide; Zebra 3 product page |
| Arturia V Collection | commercial | https://www.arturia.com/products/software-instruments/v-collection/overview | Mini V, Jup-8 V, Prophet-5 V, SEM V, OP-Xa V, Solina V, Jun-6 V, CS-80 V, Jup-8000 V, Acid V product pages; TAE page |
| Roland Cloud | commercial | https://www.roland.com/us/products/rc_juno-106/ | JUNO-106, JUPITER-8, TB-303, SH-101, ZENOLOGY Pro product pages; TB-303 sequencer article; SH-101 software manual |
| Korg Collection | commercial | https://www.korg.com/us/products/software/korg_collection/ | ARP Odyssey, Polysix, MS-20, Mono/Poly, Prophecy product pages |

## Moved

none
