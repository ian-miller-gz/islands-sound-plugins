# operator — round 1

## Matrix

A cell is `yes` / `no` / `partial` and one word; `?` is unverified. The CPU row reads `low` / `mid` / `high` and how it is known (`est` = estimated from the code's per-sample work, `stated` = the maker's own words). Ours are read from this worktree. Open: Dexed, Surge XT, Vital, ZynAddSubFX, Plaits, STK (Rhodey, Wurley, BeeThree), setBfree, mda ePiano, Furnace, Magical 8bit Plug 2, ADLplug. Popular: FM8, DX7 V, opsix native, Ableton Operator, Pianoteq (electric pianos), Stage-73 V, B-3 V2, VB3-II, chipsounds, chipsynth MD. Rows past the commission's list (operator envelopes, keyboard scaling, pitch envelope, polyphony, velocity, the mechanism modelled, sustain pedal, step sequences, patch import, programs, stereo image) are the family's own.

| Capability | operator | tine | drawbar | chip | Dexed | Surge XT | Vital | ZynAddSubFX | Plaits | STK | setBfree | mda ePiano | Furnace | Magical 8bit | ADLplug | FM8 | DX7 V | opsix | Ableton Operator | Pianoteq EP | Stage-73 V | B-3 V2 | VB3-II | chipsounds | chipsynth MD |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| oscillator kinds | partial sine | partial FM-sine | yes 91-wheels | yes pulse+tri+noise | partial sine | yes 12-types | yes wavetable | yes harmonics | partial sine | yes FM | yes tonewheel | yes sample | yes multichip | partial 2A03-like | yes OPL3/OPN2 | yes 32-waves | yes 25-waves | yes 21-waves | yes 10+user | yes physical | yes physical | yes physical | yes 91-wheels | yes 15-chips | yes OPN2+PSG |
| unison and detune | partial op-detune | no | no | partial offset | no | yes 16 | yes 16 | yes unison | no | no | no | no | no | no | partial pseudo-4op | yes unison | yes 4 | ? | partial spread | partial string-width | ? | no | no | partial slot-detune | yes 6-layers |
| wavetable morphing | no | no | no | no | no | yes 512-frames | yes spectral | no | yes 8x8 | no | no | no | yes wavesynth | no | partial op-waves | no | no | no | no | no | no | no | no | yes drawable | no |
| FM algorithms and operators | yes 32x6 | partial 2op+bell | no | no | yes 32x6 | partial 3-op | partial osc-FM | partial voice-chain | yes 32x6 | yes 4op | no | no | yes 4op | no | yes 2op/4op | yes free-matrix | partial 32x6 | yes 40+user | yes 11x4 | no | no | no | no | no | yes 8x4 |
| operator envelopes | yes 4rate/4level | partial AD | no gate | partial stepped | yes 4rate/4level | partial mod-EGs | partial 6-EG | yes free-point | yes DX | partial ADSR | no gate | partial decay | yes chip-EG | partial vol-seq | yes chip-EG | yes breakpoint+loop | yes DX+DADSR | ? | yes 3rate/3level+loop | no physical | no physical | no gate | no gate | partial EG | yes chip-EG |
| keyboard scaling (level, rate) | no | partial decay-track | no | no | yes breakpoint | partial keytrack | partial keytrack | partial key-stretch | yes DX | no | no | partial key-decay | ? | no | ? | yes breakpoint | yes curves | ? | yes level+time | ? | ? | ? | ? | ? | ? |
| pitch envelope | no | no | no | no | yes 4-stage | partial via-mod | partial via-mod | yes freq-env | yes DX | no | no | no | yes pitch-macro | partial sweep | no | yes multi-point | yes depth | partial mod-EG | yes | no | no | no | no | ? | ? |
| filter models | no none | no none | partial crossover | yes output-RC | yes 4-pole | yes 14-families | yes 8-models | yes 5-kinds | partial LPG | partial two-zero | partial EQ | partial treble | partial chip-filters | no | partial DC | yes 2-pole-multimode | yes per-op | yes 11-types | yes 5-models | partial EQ/wah | partial EQ | ? | ? | partial SID-filter | yes measured |
| modulation sources and the matrix | partial 1-LFO | partial tremolo | partial scanner+rotor | partial arpeggio | partial LFO+wheel | yes 12-LFO+MSEG | yes 8-LFO+matrix | partial fixed | partial CV | partial LFO+CC | partial MIDI-CC | partial LFO | yes macros+LFO | partial vibrato+seq | partial chip-LFO | yes matrix | yes matrix | yes 3EG+3LFO | partial LFO | partial morph | ? | yes drawbar-mods | partial AT-rotor | partial 2-LFO+EG | yes matrix+steps |
| voice modes — poly, mono, legato, glide | partial poly | partial poly | partial poly | partial mono-retrigger | yes mono+portamento | yes full | yes legato+glide | yes poly/mono/legato | no glide | ? | partial poly | partial poly | partial effects | yes legato+arp+glide | partial portamento | yes poly/mono/glide | partial poly-glide | ? | yes legato+glide | partial mono | ? | ? | partial split | partial glide | yes mono/retrig/glide |
| polyphony | yes 16 | yes 16 | yes all-keys | no 1 | yes 16 | yes 64 | yes 32 | yes 60 | partial 2 | ? | yes all-keys | yes 32 | partial per-channel | yes 1-64 | yes multichip | yes 64 | yes 32 | yes 32 | yes 32 | yes adjustable | ? | ? | yes 147 | yes per-slot | yes 1-6 |
| velocity | partial global | yes level+index | no organ | no | yes per-op | partial via-mod | partial vel-track | yes per-voice | yes accent | yes gain-table | no organ | yes layers+curve | ? | yes linear | yes offset | yes per-op | ? | ? | yes per-op | yes curves | yes curve | ? | ? | yes vel | yes vel-track |
| MPE and aftertouch | no seam | no seam | no seam | no seam | yes MPE | yes MPE+polyAT | yes MPE | ? | no | partial AT | no | no | no | no | partial pressure | partial AT | partial AT | ? | partial note-bend | partial polyAT | ? | ? | partial channel-AT | ? | ? |
| microtuning | no 12TET | no 12TET | no gear-fixed | no period | yes scl+MTS | yes scl+MTS | partial scl | partial scl | no | partial Hz | partial gear/ET | no 12TET | ? | no 12TET | ? | partial stretch | partial MTS | ? | yes scl | yes scala | partial tune | ? | partial A-ref | yes scala | yes scala |
| macro controls | no | no | no | no | no | yes 8 | yes 4 | ? | yes 3-knobs | no | no | no | no | no | no | partial morph-XY | yes 4 | ? | partial racks | partial morphing | yes macros | yes macros | ? | no | partial CC-matrix |
| effects in the instrument | no | partial tremolo+pan | yes scanner+rotary+drive | no | no filter-only | yes 16-slots | yes 9 | yes 7-types | no | no | yes 5 | yes 3 | ? | no | no | yes 12 | yes 4-slots | yes 30-types | partial shaper | yes 10+reverb | yes 13+amp | yes 13 | yes 5 | yes reverb+delay | yes chorus+reverb+delay |
| oversampling and aliasing | no none | no none | no none | no naive | no none | yes 2x | yes 1-8x | yes harmonic-cut | partial 4x-2op | no | partial band-limited | ? | yes blip_buf | no naive | ? | yes high-res | ? | ? | yes antialias | partial 192k | ? | ? | ? | partial claimed | ? |
| sample playback and zones | no | no | no | no DPCM | no | no | partial 1-sample | no | no | partial rawwaves | no | yes keygroups | yes DPCM | no | no | no | no | no | no | no | no | no | no | partial DMC+SFZ | yes DAC-slicer |
| drum synthesis models and layering | no | no | no | partial noise | no | no | no | partial kits | yes 3-models | no | no | no | yes fixed-arp | partial noise | yes GM-drums | no | no | no | no | no | no | no | no | yes drum-kits | partial PSG-noise |
| vocal formant and glottal models | no | no | no | no | no | partial vocoder | yes formant | yes formant | yes speech | no | no | no | yes Klattsch | no | no | partial talkwah | no | no | no | no | no | no | no | no | partial ch3-retrig |
| the mechanism modelled (tine, wheel, chip) | no | partial pickup | partial foldback+perc+click | yes period+mixer | partial chip-engines | no | no | no | no | partial FM-only | yes leakage+click | partial sampled | yes registers | partial approximate | yes cycle-cores | no | partial DAC-modes | no | no | yes hammer+tine+damper | yes hammer+tine+pickup | yes leakage+click | yes leakage+busbars | partial period-quantised | yes bit-accurate |
| sustain pedal | no unwired | no unwired | no | no | ? | ? | ? | ? | no | no | ? | yes sustain+sostenuto | ? | ? | ? | ? | ? | ? | ? | yes progressive | ? | ? | yes pedal | ? | ? |
| step sequences and arpeggio | no | no | no | partial 3-step-arp | no | partial step-LFO | no | no | no | no | no | no | yes macros | yes vol/pitch/duty | no | yes 32-step-arp | yes arp+seq | yes motion-seq+arp | no | no | no | yes step-seq | no | yes wave-seq | yes steps |
| patch import | no | no | no | no | yes DX7-sysex | no | no | no | yes DX7-bank | no | no | no | ? | no | yes banks | yes DX7/DX7II | yes DX7-sysex | yes DX7-sysex | no | no | no | no | no | no | ? |
| programs | no | no | no | no | ? | yes 2800+ | yes 75+ | yes 1100+ | yes 32/bank | no | yes 110 | yes 5 | ? | ? | yes banks | yes 960+ | yes 432+ | yes hundreds | ? | ? | yes 50+ | yes 50 | ? | ? | ? |
| stereo image | no mono | yes autopan | yes rotary | no mono | no mono | yes scenes | yes spread | yes unison-pan | no | no | yes rotary | yes key-pan | partial per-chip | ? | partial pan | yes per-op-pan | ? | ? | yes spread | yes 8-mics | yes stereo-vibrato | ? | partial reverb | yes pan | partial hard-pan |
| CPU per voice | mid est | low est | mid fixed | low est | ? | ? | low stated | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | low stated | ? | ? |

## Lacking

Answering round 0's word — stand beside the open and the popular instruments, family by family; learn the methods; what many need, the library gives once.

### Every instrument (shared — a `core/` matter)

- microtuning — all four: a per-key tuning table from Scala `.scl`/`.kbm` in `CORE::PHASE` (Dexed, Surge XT, Ableton Operator, Pianoteq, chipsounds, chipsynth MD; MTS-ESP in Dexed and Surge XT). drawbar keeps its gear ratios as one table among them (setBfree's gear/equal switch); chip quantises the tuned pitch to its period after.
- programs — all four: a bank of our own per instrument on the PROGRAM event (Dexed carts, FM8 960+, DX7 V 432+, setBfree 110, Stage-73 V 50+, B-3 V2 50).
- anti-aliasing — operator and tine: their phase modulation and the tine's pickup shaper run unoversampled; FM8 has a high-res mode, Surge XT runs at 2x, Ableton Operator has an antialias mode, Plaits runs its 2-op engine at 4x. One `CORE::SHAPER` oversampler (the subtractive survey asks the same) serves both. chip wants band-limited steps instead (below).
- sustain pedal — operator and tine: `CORE::VOICE` already holds `hold`, but no row reaches it. A `Hold` row, as polymod has, is the honest route while CONTROLLER events carry rows (mda ePiano, Pianoteq, VB3-II).
- MPE and aftertouch — operator and tine: per-note pressure and bend (Dexed MPE, Surge XT, Pianoteq poly-AT, FM8 AT). The seam has no pressure event, so this is a finding for an engine order first.
- programs and patches as files — operator: DX7 sysex import (Dexed, FM8, DX7 V, opsix, Plaits) needs a file to reach a plugin; that is a seam question, recorded, not designed here.

### operator

- keyboard level and rate scaling: a breakpoint with a curve per side and a rate offset by key (Dexed, FM8, DX7 V, Ableton Operator, Plaits).
- pitch envelope: four rates and four levels on the voice's pitch (Dexed, FM8, DX7 V, Ableton Operator, Plaits).
- per-operator velocity sensitivity in place of the one global `Velocity` depth (Dexed, FM8, Ableton Operator).
- per-operator fixed-frequency mode beside the ratio (Dexed, STK, Plaits, Surge XT's absolute modulators).
- per-operator waveforms beyond the sine (FM8 32, DX7 V 25, opsix 21, Ableton Operator 10+), built band-limited per table.
- LFO sensitivity per operator (amplitude) and per voice (pitch), and a small modulation matrix (FM8, DX7 V, opsix).
- a filter after the operators riding `CORE::FILTER` (Dexed, FM8, DX7 V, opsix, Ableton Operator).
- voice-mode rows: mono, legato and glide; `CORE::VOICE` already has them (Dexed, FM8, Ableton Operator).
- unison with detune and spread (FM8, DX7 V); `CORE::VOICE` already stacks.
- stereo out: per-operator pan or unison spread; it writes one sample to every lane (FM8, Ableton Operator).
- effects in the instrument: chorus, delay, reverb over `CORE::LINE` and `CORE::REVERB` (FM8 12, DX7 V 4 slots, opsix 30 types).
- macros: brightness (the modulators' levels together) and envelope time (Plaits, DX7 V, FM8 morph).

### tine

- key-dependent timbre: the modulation index and the bell falling with key (STK Rhodey, Pianoteq, Stage-73 V). Today only the decay tracks key.
- the mechanism: hammer and release noise and the damper's stop (Pianoteq, Stage-73 V); a modal tone bar is a later step (Faust `physmodels.lib`).
- a reed mode: one pair at a fixed frequency gives the reed piano's body (STK Wurley, Pianoteq reeds).
- velocity curve and a velocity-driven darkening low-pass (mda ePiano, Pianoteq).
- stereo by key: notes panned across the keyboard (mda ePiano); the tremolo row already autopans.
- effects in the instrument: drive before the tremolo, chorus or phaser (mda ePiano, Stage-73 V 13).
- microtuning and the Hold row (shared, above).

### drawbar

- tonewheel leakage: neighbouring wheels bleeding in at a low level through a per-key matrix (setBfree, B-3 V2, VB3-II).
- key click per busbar contact, the nine contacts closing at slightly different times (setBfree, VB3-II). Today one noise burst serves every key.
- a lower manual and pedals with their own drawbars, by key split (setBfree, B-3 V2, VB3-II).
- the scanner as a delay line swept by a shaped table per depth, not one triangle LFO on a single tap (setBfree).
- the rotary's mic and cabinet: direct and reflected paths and mic distance (setBfree, VB3-II), and spin-up and spin-down per rotor.
- a preamp drive whose bias sags with level (setBfree, VB3-II) in place of the plain soft clip.
- tonewheel loading: the level drops a little as more keys are held (setBfree).
- reverb in the instrument (setBfree, VB3-II), riding `CORE::REVERB`.
- a gear/equal tuning switch (setBfree) under the shared microtuning.

### chip

- band-limited steps: each amplitude change written as a band-limited step (Furnace, Game_Music_Emu, chipsounds). Today the pulses, the triangle and the noise alias at the host rate.
- the sweep unit on the pulses (Furnace, Game_Music_Emu, the NESdev reference).
- step sequences for volume, duty, pitch and arpeggio, each with a loop and a release point (Furnace macros, Magical 8bit Plug 2, chipsounds). Today it has the decay envelopes and a 3-step arpeggio.
- vibrato with delay, and portamento (Magical 8bit Plug 2, Furnace).
- velocity to level (Magical 8bit Plug 2, chipsounds).
- poly mode spreading notes over the channels (Magical 8bit Plug 2, chipsounds per slot).
- a noise drum mode: a fixed arpeggio on the noise period by note (Furnace).
- the sample channel: delta-modulated playback by note (Furnace, chipsounds, chipsynth MD). It waits on a sample source reaching a plugin, so it is a seam question first.
- a console clock switch, since the noise rates and the periods differ by region (NESdev, chipsounds).
- stereo pan per channel (chipsounds).

## Methods

| Method (own words) | Source and file | Licence | Lands |
|---|---|---|---|
| four-stage rate/level envelope: a rising segment closes a fraction of the remaining distance (it slows near the top), a falling one steps evenly in the log domain; key adds to the rate; the step rescales to the host rate | Dexed `source/msfa/env.cc` | Apache-2.0 (file), GPL-3.0 (repo) | core/envelope `Segments` (operator) |
| operator gain summed in the log domain: output level, key level scaling (a breakpoint with a linear or exponential curve per side) and velocity times a per-operator sensitivity, then one exponential lookup | Dexed `source/msfa/dx7note.cc` | Apache-2.0 | core/envelope, operator rows |
| an algorithm as one flag set per operator (input bus, output bus, add or overwrite, feedback in/out) walked by a renderer choosing among three kernels | Dexed `source/msfa/fm_core.cc` | Apache-2.0 | operator `voice.algorithms.cpp` |
| algorithms compiled at create into chains of specialised renderers, picked by chain length, source and additive flag, so the inner loop has no branch | Plaits `plaits/dsp/fm/algorithms.h` | MIT | operator `voice.algorithms.cpp` |
| pitch envelope: four segments moving evenly in a level index read through a curve that is slow near centre and fast at the ends; holds at the third stage until the lift | Dexed `source/msfa/pitchenv.cc` | Apache-2.0 | core/envelope (operator) |
| mono hand-over: the new note inherits the old voice's whole state, its signal only, its phase only, or a reset; portamento by rate with a glissando option | Dexed `source/msfa/dx7note.cc` | Apache-2.0 | core/voice `Glide` (operator, chip) |
| feedback from the mean of the last two outputs (ours already does it), with gain ramped linearly across the block | Dexed `source/msfa/fm_op_kernel.cc`; Plaits `plaits/dsp/fm/operator.h` | Apache-2.0; MIT | core/oscillator `Operator` |
| modulator depth on a cubic curve, ramped per sample; carrier increment ramped across the block against block-rate sidebands; oscillators at 2x | Surge XT `src/common/dsp/oscillators/FM3Oscillator.cpp`, `src/common/globals.h` | GPL-3.0 | core/oscillator (operator's 16-frame retune) |
| 2-op FM at 4x behind FIR decimation, with the index cut as pitch rises | Plaits `plaits/dsp/engine/fm_engine.cc` | MIT | core/shaper `Oversampler` (operator, tine) |
| ratio or absolute frequency per operator; a negative ratio read as its reciprocal for sub-harmonics; the patch's sign picks fixed frequency | Surge XT `FM3Oscillator.cpp`; Plaits `plaits/dsp/fm/voice.h`; STK `src/FM.cpp` | GPL-3.0; MIT; MIT | operator (a Fixed switch per operator) |
| brightness macro: offset only the modulators' levels, clamped to each operator's headroom; one envelope-time macro scaling attack and decay | Plaits `plaits/dsp/fm/voice.h` | MIT | operator |
| sine variants by folding, zeroing or rectifying quadrants of a sine/cosine pair | Surge XT `src/common/dsp/oscillators/SineOscillator.cpp` | GPL-3.0 | core/oscillator `Table` (operator waveforms) |
| waveforms built in the frequency domain with the harmonics above Nyquist zeroed per note | ZynAddSubFX `src/Synth/OscilGen.cpp` | GPL-2.0+ | core/oscillator `Table` (operator) |
| PM adds the modulator to the read phase; FM integrates it first; depth damped as the base frequency rises | ZynAddSubFX `src/Synth/ADnote.cpp` | GPL-2.0+ | core/oscillator (operator, tine) |
| tine by two FM pairs: carriers near the fundamental, one modulator on a high partial with a far shorter decay; velocity sets the modulation depth | STK `src/Rhodey.cpp` | STK (MIT-like) | tine (confirms ours; adds key-dependent index) |
| reed piano: one pair held at a fixed frequency gives a constant body formant; a short envelope gives the bark | STK `src/Wurley.cpp` | STK (MIT-like) | tine (a reed mode) |
| velocity through an adjustable curve to level and to a per-voice darkening low-pass; decay faster up the keyboard; notes panned by key about the middle | mda ePiano `src/mdaEPiano.cpp` (LV2 port) | GPL-3.0+ (port); MIT or GPL-2.0 (original) | tine |
| one LFO row sweeping from autopan through zero to tremolo | mda ePiano `src/mdaEPiano.cpp` | GPL-3.0+ (port) | tine (`Depth` and `Stereo` folded) |
| a fixed pseudo-random detune per key for a repeatable vintage spread | mda ePiano `src/mdaEPiano.cpp` | GPL-3.0+ (port) | core/voice (tine, drawbar) |
| a tone bar as a modal resonator bank struck by an exciter | Faust `physmodels.lib` | LGPL-2.1+ with exception | core/filter (tine, later) |
| tonewheels as band-limited loops shared by reference; every key sounds without a voice limit; level drops as keys pile on | setBfree `src/tonegen.c` | GPL-2.0+ | drawbar `voice.wheels.cpp` |
| leakage built once into a per-key matrix: compartment neighbours, transformer and terminal-strip spill, busbar bleed fading with distance; tiny terms pruned | setBfree `src/tonegen.c` | GPL-2.0+ | drawbar |
| key click per busbar: each contact's attack envelope chosen from a few shapes, the contacts closing at slightly random times | setBfree `src/tonegen.c` | GPL-2.0+ | drawbar `Click` |
| percussion re-armed only once every upper key is up; its harmonic's bus muted while it is on | setBfree `src/tonegen.c` | GPL-2.0+ | drawbar (confirms ours) |
| scanner: a ring delay whose write head is pushed ahead or behind a steady read head by a phase-driven shaped table per depth, written fractionally into two slots; chorus mixes dry and wet at equal power | setBfree `src/vibrato.c` | GPL-2.0+ | drawbar `voice.scanner.cpp`, core/line |
| rotary: each rotor writes into a delay ring at a position from the rotor-to-mic distance by angle (the Doppler); direct and reflected paths; short angle-indexed colour; loudness swing emerges; spin-up and spin-down per rotor | setBfree `b_whirl/whirl.c` | GPL-2.0+ | drawbar `voice.rotary.cpp`; effect/rotary |
| preamp: an asymmetric biased soft clip whose bias follows a leaky level follower (sag), with feedback around the clip | setBfree `b_overdrive/overdrive.c` | GPL-2.0+ | core/shaper (drawbar `Drive`, tine pickup) |
| band-limited steps: a channel reports only its level changes with their clock times; each change is added as a band-limited step whose kernel phase is picked by the fractional time; reading integrates and removes DC through a leaky high-pass | blip_buf `extern/blip_buf/blip_buf.c` (Furnace tree); Game_Music_Emu `gme/Blip_Buffer.h` | LGPL-2.1+ | core/oscillator (a step buffer: chip; later the square banks of hat and cymbal) |
| channels run event to event and emit edges only; the triangle holds still at ultrasonic periods | Game_Music_Emu `gme/Nes_Oscs.cpp` | LGPL-2.1+ | chip |
| sweep: the target period is the period plus or minus a shifted copy; the first pulse negates by ones' complement; overflow or a tiny period mutes | NESdev wiki APU Sweep; Game_Music_Emu `gme/Nes_Oscs.cpp` | no licence stated (facts); LGPL-2.1+ | chip |
| macro interpreter: each macro holds position, delay, speed, loop point and release point (jump or wait), shaped as a sequence, an ADSR or an LFO, with a changed flag per tick | Furnace `src/engine/macroInt.cpp`, `doc/4-instrument/README.md` | GPL-2.0+ | chip (a stepped sequence core if a second instrument asks) |
| pitch as one fractional note: key, bend, decaying sweep, decaying portamento, delayed vibrato and the pitch sequence summed, then converted once | Magical 8bit Plug 2 `Source/TonalVoice.cpp` | GPL-3.0 | chip `voice.pitch.cpp` |
| mono arpeggio: held notes kept sorted, stepped each frame interval or note division; lifts removed only at the next step | Magical 8bit Plug 2 `Source/TonalVoice.cpp` | GPL-3.0 | chip |
| a note map for the sample channel: per note a sample, a rate index and a starting level | Furnace `doc/4-instrument/nes.md` | GPL-2.0+ | chip (after the seam) |
| two-group nonlinear mix and the console's two high-pass and one low-pass output stages (ours already does both); a second console has one high-pass | NESdev wiki APU Mixer | no licence stated (facts) | chip (a console row) |
| velocity mapped to operator attenuation by selectable historical volume models | libADLMIDI README (via ADLplug) | GPL-3.0 (engine); BSL-1.0 (ADLplug's own) | operator (velocity curve) |

No line, table, constant or coefficient is taken from any of these; each row is the idea in this survey's words.

## Sources

| Name | Licence | Home | Read |
|---|---|---|---|
| Dexed | GPL-3.0 (msfa files Apache-2.0) | https://github.com/asb2m10/dexed | README; LICENSE; feature page asb2m10.github.io/dexed; `source/msfa/env.cc`, `dx7note.cc`, `fm_core.cc`, `fm_op_kernel.cc`, `pitchenv.cc`; `source/PluginProcessor.cpp/.h`, `PluginFx.cpp`, `Tuning.cpp` |
| Surge XT | GPL-3.0 | https://github.com/surge-synthesizer/surge | LICENSE; manual-xt; `FM2Oscillator.cpp`, `FM3Oscillator.cpp`, `SineOscillator.cpp`, `globals.h` |
| Vital | GPL-3.0 | https://github.com/mtytel/vital | README; LICENSE; vital.audio; `synth_oscillator.cpp`, `synth_constants.h`, `synth_parameters.cpp`, `synth_strings.h` |
| ZynAddSubFX | GPL-2.0+ | https://github.com/zynaddsubfx/zynaddsubfx | COPYING; home page; `ADnote.cpp`, `ADnoteParameters.h`, `globals.h`, `Envelope.cpp`, `OscilGen.cpp`, `Microtonal.cpp` |
| Mutable Instruments eurorack (Plaits) | MIT | https://github.com/pichenettes/eurorack | README; Plaits manual; `six_op_engine.cc/.h`, `fm_engine.cc`, `fm/algorithms.h`, `fm/voice.h`, `fm/envelope.h`, `fm/operator.h`, `sine_oscillator.h` |
| STK | STK licence (MIT-like) | https://github.com/thestk/stk | LICENSE; `Rhodey.cpp`, `Wurley.cpp`, `BeeThree.cpp`, `FM.cpp`, `TubeBell.cpp` |
| setBfree | GPL-2.0+ | https://github.com/pantherb/setBfree | README; COPYING; setbfree.org; `src/tonegen.c`, `src/vibrato.c`, `b_whirl/whirl.c`, `b_overdrive/overdrive.c` (part), `pgm/default.pgm` (program list) |
| mda ePiano | MIT or GPL-2.0 (original); GPL-3.0+ (LV2 port read) | https://sourceforge.net/projects/mda-vst | project page; elk-audio/mda-vst3 README; gitlab drobilla/mda-lv2 `src/mdaEPiano.cpp` |
| Faust libraries | LGPL-2.1+ with exception | https://github.com/grame-cncm/faustlibraries | `physmodels.lib` (model list; no electric piano or organ there) |
| Furnace | GPL-2.0+ | https://github.com/tildearrow/furnace | README; LICENSE; `doc/7-systems/nes.md`, `doc/4-instrument/nes.md`, `README.md`, `fm-opn.md`, `klattsch.md`; `src/engine/macroInt.cpp`, `src/engine/platform/nes.cpp` |
| blip_buf | LGPL-2.1+ | https://github.com/tildearrow/furnace/tree/master/extern/blip_buf | `blip_buf.c`, `blip_buf.txt` |
| Game_Music_Emu | LGPL-2.1+ | https://github.com/libgme/game-music-emu | README; license.txt; `gme/Blip_Buffer.h`, `gme/Nes_Oscs.cpp`, `gme/Nes_Apu.cpp` |
| Magical 8bit Plug 2 | GPL-3.0 | https://github.com/yokemura/Magical8bitPlug2 | README; LICENSE; `PluginProcessor.cpp`, `TonalVoice.cpp`, `BaseVoice.cpp`, `FrameSequenceParser.cpp`, `NoiseVoice.cpp`, `TriangleVoice.cpp` |
| ADLplug / libADLMIDI | BSL-1.0 (own code); GPL-3.0 (engine, JUCE) | https://github.com/jpcima/ADLplug | README; LICENSE; `sources/opl3/parameter_block.h`; libADLMIDI README |
| NESdev wiki | none stated | https://www.nesdev.org/wiki/APU | APU, APU Mixer, APU Sweep, APU Noise, APU DMC; About page |
| FamiStudio | MIT | https://github.com/BleuBleu/FamiStudio | README (licence, emulation) |
| Native Instruments FM8 | commercial | https://www.native-instruments.com/en/products/komplete/synths/fm8/ | product and feature pages; FM8 manual |
| Arturia DX7 V | commercial | https://www.arturia.com/products/software-instruments/dx7-v/overview | product page; KVR forum post (tuning) |
| Korg opsix native | commercial | https://www.korg.com/us/products/software/opsix_native/ | product pages; Attack Magazine news |
| Ableton Operator | commercial | https://www.ableton.com/en/manual/live-instrument-reference/ | Live manual (Operator; tuning systems) |
| Modartt Pianoteq (electric pianos) | commercial | https://www.modartt.com/electric_pianos | electric pianos page; Pianoteq overview; user manual |
| Arturia Stage-73 V | commercial | https://www.arturia.com/products/software-instruments/stage-73-v/overview | product page |
| Arturia B-3 V2 | commercial | https://www.arturia.com/products/software-instruments/b-3-v/overview | product page |
| GSi VB3-II | commercial | https://www.kvraudio.com/product/vb3-ii-by-gsi/details | KVR details (the maker's pages refused the reader) |
| Plogue chipsounds | commercial | https://www.plogue.com/products/chipsounds.html | product page; KVR details; chipsounds guide |
| Plogue chipsynth MD | commercial | https://www.plogue.com/products/chipsynth-md.html | product page; MD manual |
| Plogue chipsynth PortaFM, OPS7 | commercial | https://www.plogue.com/products/ | product pages (not a column: too thin) |

## Moved

none
