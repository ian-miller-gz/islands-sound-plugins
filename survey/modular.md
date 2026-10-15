# modular — round 1

## Matrix

A cell is `yes` / `no` / `partial` and one word. `no job` means the row is not that module's job. `?` means unverified. The CPU row reads `low` / `mid` / `high` with `est` (estimated from the per-sample work in the code; nothing was measured) or `rank` (the maker's own relative ranking). Ours are read from this worktree. Open: VCV Fundamental, Bogaudio, Befaco, Surge XT Rack, Mutable Instruments, the Faust libraries, SuperCollider, Pure Data, Bespoke. Popular: Reaktor Blocks, Phase Plant, Bitwig's Grid, Voltage Modular, Softube Modular.

| Capability | amplifier | envelope | lfo | mixer | noise | oscillator | resonator | shaper | slew | Fundamental | Bogaudio | Befaco | Surge Rack | Mutable | Faust | SuperCollider | Pd | Bespoke | Blocks | Phase Plant | Grid | Voltage | Softube |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| oscillator kinds | no job | no job | partial 6-waves | no job | partial colours | yes 5-waves | no job | no job | no job | yes 4+table | yes 4-modules | yes 3-VCOs | yes 11 | yes 16-engines | yes many | yes many | partial table+ramp | yes 6 | yes morph+Primes | yes 5-generators | yes 12 | yes 4+ | partial 4-waves |
| oscillator anti-aliasing | no job | no job | no slow-only | no job | no job | yes polyBLEP | no job | no job | no job | yes minBLEP | yes residual+OS | yes DPW+OS | yes BLIT/DPW | yes polyBLEP | yes DPW/BLEP | yes DSF | no naive | no naive | yes stated | yes bandlimit | partial some | ? unstated | ? unstated |
| FM, sync, ring | no 2-quadrant | no job | no | no job | no job | no inputs | no linear-FM | no job | no job | yes lin/exp+sync | yes TZFM+sync | yes TZ+ring | yes FM+sync | yes PM+sync+ring | partial sync+PM | yes patched | partial patched | yes sync+PM | partial FM | yes all | yes all | yes lin/exp+sync | partial sync+ring |
| FM algorithms and operators | no job | no job | no job | no job | no job | no | no job | no job | no job | no | partial FMOp | no | yes FM2/FM3 | partial 2-op | partial feedback-PM | partial patched | partial patched | partial fmsynth | no | partial PM | partial Phase-1 | no | no |
| unison and detune | no job | no job | no job | no job | no job | partial Super-7 | no job | no job | no job | no | yes module | no | yes 64 | partial supersaw | no | partial patched | no | yes width | no | yes 7-modes | yes stack | yes module | no |
| wavetable morphing | no job | no job | no | no job | no job | no | no job | no job | no job | yes WTVCO | no | no | yes | yes 8x8 | partial tables | ? unread | partial tabread | no | partial shape-morph | yes 256-frames | yes | no | no |
| filter models | no job | no job | no job | no job | no job | no job | partial SVF-only | no job | no job | partial ladder | partial Butterworth | partial SVF+cascade | yes many | partial LP2/LP4/BP | yes ladders+K35+SK | yes MoogFF+RLPF | partial one-pole | partial biquad | yes 8+ | yes 2-kinds | yes 14 | yes ladder+SVF | partial ladder |
| filter drive and self-oscillation | no job | no job | no job | no job | no job | no job | partial linear | no job | no job | yes tanh+2x | no linear | no | partial per-type | yes OTA | partial cubicnl | ? undocumented | no | no | partial Driver | yes nonlinear | ? unstated | partial self-osc | ? unstated |
| envelope stages and curves | no job | partial ADSR/AD/AR | no job | no job | no job | no job | no job | no job | no job | partial ADSR-exp | yes DADSRH+power | yes shaped | yes DAHDSR | yes segments | yes AHDSR+exp | yes curvature | partial linear | yes multistage | partial ADSR | yes DAHDSR+curves | yes segments | partial ADSR | partial ADSR |
| envelope loop, retrigger, end-of-cycle | no job | partial retrigger | no job | no job | no job | no job | no job | no job | no job | partial retrigger-in | yes AD-loop | yes cycle | yes EOC | yes loop | partial retrigger | yes loopNode | partial patched | ? unread | ? unstated | yes Curve-loop | yes loop+ping-pong | partial retrigger | yes retrigger-in |
| LFO waves, reset, polarity | no job | no job | partial 6-waves+phase | no job | no job | no job | partial sine-only | no job | no job | yes reset+uni/bi | yes 6+reset | partial cycle | yes many+EOC | yes shape+slope | partial reset | partial noises | partial patched | yes offset | partial 100Hz | yes custom+uni/bi | yes curves+retrigger | yes 6-outs+polarity | yes reset |
| modulation sources and the matrix (CV per row) | partial gain | no notes-only | no | no | no | no notes-only | partial cutoff | no | no | yes attenuverters | yes | partial | yes 4-per-knob | yes everything | yes signals | yes everything | yes everything | yes everything | partial 2-per-block | yes lanes+remap | yes ports | yes attenuators | yes cables |
| voice modes — poly, mono, legato, glide | no single-lane | partial mono-legato | no single | no single | no single | partial mono+glide | no single | no single | no single | yes 16-channel | yes 16-channel | yes poly | yes poly | partial chords | yes nvoices | yes nodes | partial clone | yes poly | no mono | yes poly+legato+glide | yes poly+mono | yes 16+glide | partial 4-voice |
| MPE and aftertouch | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | yes MPE | no | no | ? unstated | no | partial architecture | partial touch | partial touch | yes MPE | partial block | yes MPE | yes MPE | partial paid | partial ROLI |
| microtuning and quantizer | no job | no job | no job | no job | no job | no 12TET | no job | no job | no job | partial 12TET-quantizer | no | no | ? unstated | partial Marbles-scales | no | yes Tuning | partial mtof | yes scl+MTS | partial quantizer | no | partial Micro-pitch | partial paid-scl | no |
| macro controls | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | ? unread | yes snapshots | yes 8 | partial remotes | partial perform | partial perform |
| effects in the instrument | no | no | no | no | no | no | no | partial drive | no | no | no | yes reverb | yes FX | partial Warps | yes libraries | yes UGens | partial patched | yes many | yes Rounds | yes snapins | yes FX | yes many | partial delay |
| oversampling and aliasing (nonlinear stages) | no job | no job | no job | no job | no job | no job | no none | partial 2x | no job | yes 2x-VCF | partial VCO-OS | yes x16+ADAA | partial ADAA | yes Warps | yes ADAA | no | no | partial drums | ? unstated | yes automatic | yes 4x-always | ? unstated | partial internal-rate |
| VCA response | partial linear | no job | no job | partial linear | no job | no job | no job | no job | no job | yes lin/exp | yes dB/lin | yes log↔exp | yes lin↔cubed | yes lin↔exp | partial multiply | partial multiply | partial multiply | partial gain | ? unstated | ? unstated | yes dB/vol | yes lin/exp | yes lin/exp |
| mixer: channels, pan, mute, CV | no job | no job | no job | partial 4+pan | no job | no job | no job | no job | no job | partial 4+CV | yes pan+mute+CV | partial 4-knobs | yes mute/solo | partial attenuverters | no | partial Mix/Pan2 | partial patched | partial panner+muter | partial 3+invert | yes lanes+mute | yes 6+pan | yes solo/mute | yes 3-kinds+pan |
| noise colours and random sources | no job | no job | partial hold+random | no job | partial 4-colours | no job | no job | no job | no job | yes 7+random | yes 5+walk+S&H | yes Plethora | partial S&H-osc | yes Marbles | yes colored+velvet | yes 5+dust | partial white | partial uniform | partial S&H | yes slope+random | partial white/pink | partial white/pink+S&H | yes A-118+S&H |
| slew: shapes, rise and fall | no job | no job | no job | no job | no job | partial glide | no job | no job | partial linear | partial one-rate | yes shape+rise/fall | yes lin↔exp | no | partial one-pole | yes lag-ud | yes Slew/LagUD/VarLag | yes slop~ | partial smoother | partial CVP | no | partial Lag | partial glide | yes module |
| waveshaping, folding, crushing | no job | no job | no job | no job | no job | no job | no job | yes 5-curves | no job | no | partial clip | yes folder | yes many | yes Warps | yes fold+crush | yes Fold+Decimator | partial clip~ | yes expression | partial DWG | yes distortion | yes 10+ | partial fold | no |
| clock and tempo sync | no seam | no seam | no seam | no job | no seam | no job | no seam | no job | no job | yes clock | partial reset | partial cycle | yes BPM-CV | yes clock | no | yes TempoClock | partial metro | yes interval | yes Util-Clock | yes synced | yes Transport | yes divider | yes DAW-sync |
| sample playback and zones | no | no | no | no | no | no | no | no | no | no | no | no | no | no | partial soundfile | yes PlayBuf | yes tabplay~ | yes sampler | partial ? | yes sampler | yes Sampler | yes Sampler-I | no |
| drum synthesis models and layering | no | no | no | no | no | no | no | no | no | no | no | partial Kickall | no | yes Plaits-drums | partial physmodels | partial Membrane | no | partial drumsynth | ? unstated | no | no | partial drum-osc | no |
| vocal formant and glottal models | no | no | no | no | no | no | no | no | no | no | no | no | partial Twist | yes speech | partial formants | partial Formant | no | no | no | no | yes Vowels | no | no |
| stereo and per-lane image | partial per-lane | no copied | no copied | yes pan | yes decorrelated | no copied | partial per-lane | partial per-lane | partial per-lane | partial poly-cables | yes pan | partial | yes stereo | partial | yes | yes | yes | yes width | partial | yes | yes stereo-always | yes | yes |
| CPU per voice | low est | low est | low est | low est | low est | low est | low est | mid est | low est | ? | ? | ? | low–high rank | ? | ? | ? | ? | partial lite-mode | ? | ? | ? | ? | ? |

## Lacking

Answering round 0's word: stand beside the open and the popular instruments, family by family; what many need, the library gives once.

### Every module (shared — a `core/` matter or a seam finding)

- CV on the rows: only `amplifier` (gain) and `resonator` (cutoff) take a `cv` in. Every competitor's module takes CV on most knobs (Fundamental and Bogaudio attenuverters, Surge Rack's four mod inputs per knob, Voltage, Softube). Under 381 laws 4–5 this is a `cv` AUDIO in and an amount row per module; envelope, lfo, mixer, noise, oscillator, shaper and slew need it. One shared helper reads in *i* lane *c* and scales it onto a row (core/block or a new core).
- polyphonic cables: all nine are one voice on one lane set, and oscillator, envelope and lfo copy one value into every lane. VCV, Voltage and Grid carry 16 voices per cable. This is a seam finding (the host's lane layout, 381 law 4) before it is a module's.
- clock and tempo sync: lfo and a looping envelope wait on TRANSPORT to plugins (379m). Every popular competitor syncs.
- microtuning: oscillator reads 12-TET `CORE::PHASE::hertz`. A per-key table from `.scl`/`.kbm` (Bespoke, SuperCollider, Voltage's module) is shared with every family (core/phase `Tuning`).
- MPE and aftertouch: the seam has no pressure or per-note event. Fundamental's MIDI-CV, Phase Plant, Grid and Bespoke carry MPE. Finding for an engine order.
- gate CV: envelope and oscillator are played by NOTES only. A gate or trigger read from a `cv` in (a rising edge) would let lfo, noise and slew drive them, as Fundamental ADSR and Softube A-140 do.

### amplifier

- a Response row morphing linear to exponential (Veils, Voltage Lin/Expo, Bogaudio dB/LIN, Surge Rack linear↔cubed, Softube A-132-3).
- a four-quadrant mode: negative CV inverts the signal (ring modulation, Blinds, Befaco A*B+C). Today the gain is clamped at zero.
- a low-pass-gate mode: one CV opens level and brightness together (Plaits LPG, Blocks Primes LPG).

### envelope

- delay and hold stages (Bogaudio DADSRH, Surge Rack DAHD, Phase Plant DAHDSR, Faust ahdsr).
- a curve amount per stage in place of the one fixed exponential ratio (Stages, SuperCollider curvature, Phase Plant, Bogaudio power curve).
- a loop/cycle mode, so the envelope runs as a shaped LFO (Stages, Tides, Befaco Rampage, Bogaudio AD loop, Grid AD loop).
- a gate in from CV beside NOTES, and time rows taking CV (Fundamental ADSR, Softube A-140).

### lfo

- a reset/sync from a `cv` edge or a struck note (Fundamental, Bogaudio, Voltage, Softube A-147, Grid retrigger).
- tempo sync (the seam first; see above).
- a shape or skew row: saw↔triangle↔ramp and pulse width (Tides shape/slope, Bogaudio, Phase Plant custom).
- rate under CV and a range reaching audio rate (Blocks 100 Hz, Tides audio rate). Ours stops at 20 Hz.
- Random with its own smoothness and step modes, apart from Rate (Fundamental Random: stepped/linear/smooth/exp; Marbles spread and bias).

### mixer

- a mute per in (Bogaudio Mix4, Shades, Voltage, Phase Plant lanes).
- equal-power pan (Bogaudio): ours is a linear balance at 0 dB centre.
- CV per level, read from a further in (Fundamental VCA Mix).
- attenuverting levels (negative level inverts) and a dB taper (Shades, Blocks Mix invert).

### noise

- more colours: violet, grey, red, velvet, crackle, and a continuous colour slope (Fundamental 7 colours, Faust colored_noise and velvet, SuperCollider Gray/Crackle/Dust, Phase Plant slope).
- sample-and-hold and random-walk outputs over the colours (Bogaudio Walk and S&H, Softube A-118, Marbles). `CORE::NOISE::Hold` stands unused by the family.
- a stepped or keytracked rate (Phase Plant stepped noise).

### oscillator

- a pitch CV in (exponential, octaves) and a linear through-zero FM in, both as `cv` lanes (Fundamental, Bogaudio, Befaco PonyVCO, Voltage). Today it hears NOTES only.
- a hard-sync in, riding `CORE::OSCILLATOR::Sync`, which stands unused (Fundamental hard and soft sync, Plaits, Voltage).
- a separate Detune row for Super: Width does both jobs today. Plus a wave-morph row (Fundamental WTVCO, Plaits variable shape, Blocks Bento morph).
- pulse-width CV (every competitor VCO) and a sub output (Voltage, Softube).
- microtuning (see shared).

### resonator

- a Model row riding the cores that stand: `Ladder`, `Diode`, Sallen-Key (Grid 14 models, Surge Rack many, Voltage ladder, Fundamental ladder).
- drive and nonlinear feedback with 2x oversampling (Fundamental VCF tanh stages, Ripples OTA limiting, Phase Plant Nonlinear). Ours is linear.
- a 24 dB slope beside the 12 dB SVF (Ripples LP4, Voltage 12/24).
- cutoff CV read per sample, or interpolated: the 16-frame stride holds back audio-rate FM. Linear FM too (Fundamental, Blocks SVF FM).

### shaper

- CV on Drive and on a fold amount (Warps, Befaco Chopping Kinky, Grid ports).
- a Bias/symmetry row: Tube's bias is fixed (Faust cubicnl offset, Befaco).
- antiderivative anti-aliasing for Hard and Fold, or a quality row reaching 4x–16x (Faust aanl, Befaco x16 and ADAA, Grid 4x). Ours is fixed at 2x.
- a Chebyshev curve: `CORE::SHAPER::Chebyshev` stands, and the module does not expose it (Grid Chebyshev).

### slew

- a Shape row from linear through exponential to logarithmic (Befaco Slew Limiter and Rampage, Bogaudio Slew, SuperCollider VarLag). Ours is linear only.
- CV on Rise and Fall (Bogaudio, Befaco).
- a nonlinear mode: small moves follow a one-pole, large moves are rate-limited (Pd slop~).

## Methods

Ideas only; no line, table or coefficient is taken. A permissive source (MIT, BSD) may be cited by name in a `///` contract. GPL/LGPL trees and manuals are read for the method alone.

| Method (our words) | Source and file | Licence | Lands |
|---|---|---|---|
| minimum-phase BLEP built once by the cepstral method, mixed at the sub-sample crossing; its integral for triangle corners and pulse/sync jumps | VCV Fundamental `src/VCO.cpp` | GPL-3.0+ | core/oscillator (a quality option) |
| hard sync placed at the fractional reset with a BLEP and BLAMP; saw↔square↔triangle morph from one phase | Mutable `plaits/dsp/oscillator/variable_shape_oscillator.h`; stmlib `dsp/polyblep.h` | MIT | core/oscillator `Sync` → oscillator |
| through-zero linear FM: the phase increment may go negative, and the BLEP direction follows | Bogaudio `src/dsp/oscillator.cpp`; Befaco `src/PonyVCO.cpp` | GPL-3.0+ | core/oscillator → oscillator |
| ladder of four saturating one-poles, linearised each sample around the last output, 2x polyphase half-band, a whisper of noise so it self-oscillates from silence | VCV Fundamental `src/VCF.cpp` | GPL-3.0+ | core/filter `Ladder` → resonator |
| OTA soft-limiting inside the resonance loop; LP2/LP4/BP taken from one core | Mutable Ripples manual | MIT (docs) | core/filter `Variable` → resonator |
| low-pass gate: one control drives a gain stage and an SVF low-pass, with a bleed blending VCA↔VCF | Mutable `plaits/dsp/fx/low_pass_gate.h` | MIT | amplifier (a mode), core/filter |
| VCA response: a lin↔exp morph (Veils); a dB table with a linear ramp near silence (Bogaudio); a blend of linear and cubed (Surge Rack) | Veils manual; Bogaudio `src/dsp/signal.cpp`; surge-rack `src/EGxVCA.h` | MIT (docs); GPL-3.0+; GPL-3.0+ | amplifier, mixer (level taper) |
| equal-power pan from quarter-offset sine gains | Bogaudio `src/dsp/signal.cpp` | GPL-3.0+ | mixer |
| segment curve as a rational warp centred on linear, mirrored for log and exp; a segment chain gives ADSR, loop and slew | Mutable `stages/segment_generator.cc` | MIT | core/envelope (a curve row) → envelope |
| envelope curvature as one signed number per segment, with loop and release nodes | SuperCollider `Env` (class docs) | GPL-3.0 | core/envelope `SEGMENTS` → envelope |
| power-curve stages with a linear switch; delay and hold stages | Bogaudio `src/dsp/envelope.cpp` | GPL-3.0+ | core/envelope → envelope |
| each stage approaches a target set past its end, handing off at full scale | VCV Fundamental `src/ADSR.cpp` | GPL-3.0+ | core/envelope (compare with ours) |
| shape and slope from two stacked curve families interpolated; smoothness as cascaded one-poles one way and a fold the other | Mutable `tides2/ramp_shaper.h`, `tides2/poly_slope_generator.h` | MIT | core/modulator → lfo |
| random with stepped/linear/smooth/exponential interpolation between draws | VCV Fundamental `src/Random.cpp`; SuperCollider `server/plugins/NoiseUGens.cpp` (LFNoise0/1/2) | GPL-3.0+; GPL-2.0+ | core/modulator `Lfo` → lfo |
| spread and bias as a beta distribution read through interpolated inverse-CDF grids; a loop of recent draws | Mutable `marbles/random/distributions.h` | MIT | core/noise or core/modulator → lfo, noise |
| weighted-degree quantizer with hysteresis between tiers | Mutable `marbles/random/quantizer.cc` | MIT | notes/quantizer, core/phase |
| pink by Voss row updates chosen by counter bit changes; blue and violet by first differences; red by stacking | VCV Fundamental `src/Noise.cpp`; Bogaudio `src/dsp/noise.hpp`; SuperCollider `NoiseUGens.cpp` | GPL-3.0+; GPL-3.0+; GPL-2.0+ | core/noise → noise |
| coloured noise of any spectral slope; velvet noise as sparse signed impulses | Faust `noises.lib` (colored_noise, velvet) | STK-4.3 / LGPL per function | core/noise → noise |
| slew shape by crossfading a constant rate (linear), a distance-proportional rate (exponential) and its inverse (log) | Befaco `src/Rampage.cpp`, `src/SlewLimiter.cpp` | GPL-3.0+ | core/modulator `Slew` → slew |
| separate up/down one-pole lags; a smoother morphing exponential to linear | Faust `signals.lib` (lag_ud, smoothq); SuperCollider `LagUD`, `VarLag` (docs) | per function; GPL-3.0 | core/modulator → slew |
| nonlinear slew: a one-pole whose large excursions are rate-limited, up and down apart | Pure Data `src/d_filter.c` (slop~) | BSD-3-Clause | core/modulator → slew |
| first- and second-order antiderivative anti-aliasing for clippers and folders, with a fallback when successive inputs nearly match | Faust `aanl.lib`; sst-waveshapers `include/sst/waveshapers/ADAA.h` | MIT; GPL-3.0+ (header misnamed; check LICENSE) | core/shaper → shaper |
| selectable oversampling x1–x16 behind a high-order Butterworth around a folder | Befaco `src/ChoppingKinky.cpp`, `src/PonyVCO.cpp` | GPL-3.0+ | core/shaper `Oversampler` → shaper |
| folder over the sum plus the cross-product of two signals, oversampled; diode ring model | Mutable `warps/dsp/modulator.cc` | MIT | shaper, amplifier (ring) |
| CV on every knob: a few modulation inputs, each with a depth per knob | Surge XT Rack (manual; `src/` module headers) | GPL-3.0+ | a shared CV-row helper for all nine |
| 16 voices per cable, each module processing per channel | VCV Rack (engine; `src/core/MIDI_CV.cpp`) | GPL-3.0+ | the seam (an engine order) |

## Sources

| Name | Licence | Home | What was read |
|---|---|---|---|
| VCV Rack, Fundamental | GPL-3.0+ (panels CC BY-NC-ND) | https://github.com/VCVRack/Fundamental | `plugin.json`; `VCO.cpp`, `WTVCO.cpp`, `VCF.cpp`, `ADSR.cpp`, `LFO.cpp`, `VCA.cpp`, `VCMixer.cpp`, `Noise.cpp`, `Random.cpp`, `Process.cpp`, `Quantizer.cpp`; Rack `src/core/MIDI_CV.cpp` |
| Bogaudio modules | GPL-3.0+ (graphics CC BY-SA 4.0) | https://github.com/bogaudio/BogaudioModules | README, LICENSE.md; `VCO.cpp`, `vco_base.cpp`, `VCF.cpp`, `Slew.cpp`, `dsp/oscillator.cpp`, `dsp/signal.cpp`, `dsp/envelope.cpp`, `dsp/noise.hpp` |
| Befaco | GPL-3.0+ | https://github.com/VCVRack/Befaco | README, `plugin.json`; `Rampage.cpp`, `SlewLimiter.cpp`, `ChoppingKinky.cpp`, `EvenVCO.cpp`, `PonyVCO.cpp`, `NoisePlethora.cpp`, `Mixer.cpp` |
| Surge XT Rack, Surge, sst-waveshapers | GPL-3.0+ (res/xt CC BY-NC-SA) | https://github.com/surge-synthesizer/surge-rack | README; manual https://surge-synthesizer.github.io/rack_xt_manual; `Waveshaper.h`, `VCF.h`, `EGxVCA.h`; Surge `ClassicOscillator.cpp`, `ModernOscillator.cpp`; sst-waveshapers `ADAA.h` |
| Mutable Instruments eurorack, stmlib | MIT | https://github.com/pichenettes/eurorack | manuals (Plaits, Stages, Marbles, Tides 2, Veils, Ripples, Warps, Blinds, Shades); `stmlib/dsp/polyblep.h`; `plaits/dsp/oscillator/variable_shape_oscillator.h`, `plaits/dsp/fx/low_pass_gate.h`, `stages/segment_generator.cc`, `tides2/ramp_shaper.h`, `tides2/poly_slope_generator.h`, `marbles/random/distributions.h`, `marbles/random/quantizer.cc`, `warps/dsp/modulator.cc` |
| Faust libraries | per function (MIT, STK-4.3, LGPL) | https://github.com/grame-cncm/faustlibraries | docs for oscillators, vaeffects, envelopes, noises, signals, misceffects; `aanl.lib`, `noises.lib` |
| SuperCollider | GPL-3.0 (UGens GPL-2.0+) | https://github.com/supercollider/supercollider | `server/plugins/NoiseUGens.cpp`, `OscUGens.cpp`; class docs VarLag, Env, MoogFF, Slew, LagUD, Decimator, Scale, Saw |
| Pure Data vanilla | BSD-3-Clause | https://github.com/pure-data/pure-data | LICENSE.txt; `src/d_filter.c`, `src/d_osc.c` |
| Bespoke Synth | GPL-3.0 | https://github.com/BespokeSynth/BespokeSynth | docs https://www.bespokesynth.com/docs/; `Source/Oscillator.cpp`, `Source/Scale.cpp` |
| Native Instruments Reaktor 6 Blocks | commercial | https://www.native-instruments.com/en/products/komplete/synths/reaktor-6/blocks/ | product pages (Reaktor 6, Blocks, Blocks Primes); Blocks quickstart (modulation); Sound On Sound and MusicTech reviews and tutorial |
| Kilohearts Phase Plant | commercial | https://kilohearts.com/products/phase_plant | manual: phase_plant, modulation, basic_usage, curves_lfos_remaps |
| Bitwig Studio — The Grid | commercial | https://www.bitwig.com/the-grid/ | product page; user guide: grid_modules, welcome_to_the_grid, on_grid_signals |
| Cherry Audio Voltage Modular | commercial | https://cherryaudio.com/products/voltage-modular | product page; Core module list; user guide (poly modules); module docs: oscillator, amplifier, envelope generator, LFO, filter, glide, noise generator, six-input mixer |
| Softube Modular | commercial | https://www.softube.com/modular | product page; user manual https://www.softube.com/user-manuals/modular |

## Moved

none
