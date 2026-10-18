# drums — round 1

## Matrix

A cell is `yes` / `no` / `partial` and one word; `?` is unverified. The CPU row reads `low` / `mid` / `high` and how it is known (`est` = estimated from the code's per-sample work, `stated` = the maker's own words). Ours are read from this worktree (`instrument/<name>/`, `core/percussion/`). Open: Geonkick, Plaits (Mutable eurorack), ChowKick, RipplerX, LMMS Kicker, DrumGizmo, sfizz, Hydrogen, drumkv1. Popular: Battery 4, Addictive Drums 2 (AD2), Superior Drummer 3 (SD3), Roland Cloud TR-808/909 (TR), Microtonic, KICK 3, XO, Ableton Drum Rack with Drum Sampler and the DS devices (Drum Rack). Rows past the commission's list (pitch envelope, velocity, accent, choke, chromatic pitch, articulations by note, per-drum controls, humanize and swing, sequencer, stereo and outputs, mic bleed, programs, noise) are the family's own.

| Capability | clap | clave | conga | cowbell | cymbal | hat | kick | kit | machine | percussive | rhythm | snare | tom | Geonkick | Plaits | ChowKick | RipplerX | Kicker | DrumGizmo | sfizz | Hydrogen | drumkv1 | Battery 4 | AD2 | SD3 | TR | Microtonic | KICK 3 | XO | Drum Rack |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| oscillator kinds | partial noise | partial resonators | partial swept-resonator | partial 2-squares | yes 6-squares | yes 6-squares | partial resonator | yes 9-circuits | yes swept-tri+loop | partial sine+noise | partial 4-circuits | partial 2-resonators+noise | partial swept-resonator | yes 4-waves+sample | yes resonators+sines | partial pulse+resonator | yes 9-resonators | partial sine+noise | no samples | partial generators | no samples | no samples | no samples | no samples | no samples | yes ACB-circuits | yes 3-waves+noise | yes sub+9-harmonics | no samples | partial DS-devices |
| unison and detune | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | partial split | no | no | partial stack | no | no | no | no | no | no | no | ? | partial pitch-spread | no |
| wavetable morphing | no | no | no | no | no | no | no | no | no | no | no | no | no | no | yes 8x8 | no | no | no | no | partial sample-table | no | no | no | no | no | no | partial patch-morph | ? | no | no |
| FM algorithms and operators | no | no | no | no | no | no | no | no | no | no | no | no | no | partial osc1-to-osc2 | partial attack+self | no | no | no | no | partial FM-pairs | no | no | no | no | no | no | partial LFO-pitch | ? | no | partial DS-FM |
| filter models | partial bandpass | partial one-pole | partial one-pole | partial bandpass | partial 2-bandpass | partial band+high | partial one-pole | partial per-circuit | partial one-poles | partial one-pole | partial per-circuit | partial band+high | partial one-pole | yes SVF-each | partial SVF | yes nonlinear-2pole | partial mallet-band | no | no | yes 20+types | partial resonant-LP | yes 4-types | yes 7-modes | partial cuts | ? | partial circuit | yes LP/BP/HP | partial 1-filter | partial tilt+cut | yes 4-types |
| modulation sources and the matrix | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | partial 8-envelopes | partial CV | ? | partial velocity | partial fixed | no | yes matrix | partial ADSR | yes 3EG+LFO | yes 2LFO+8slots | partial 2-envelopes | partial envelopes | no | partial env+LFO | partial curves | no | partial 1-source |
| voice modes — poly, mono, choke | partial mono | partial mono | partial mono | partial mono | partial mono | partial mono | partial mono | partial 9-mono | partial 5-mono | partial 5-mono | partial 4-mono | partial mono | partial mono | yes cut/note-off | partial mono | ? | partial poly | yes poly | yes voice-limit | yes groups+stealing | yes max-notes | partial fixed-pool | yes 128-groups | ? | ? | partial 11-voices | partial 8-channels | ? | partial mono/poly | partial per-pad |
| MPE and aftertouch | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | no seam | ? | no | ? | ? | ? | ? | partial polyAT | ? | yes poly-pressure | partial AT | yes AT-choke | ? | ? | ? | ? | ? | partial slide |
| microtuning | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | yes scl | yes MTS-ESP | ? | no | yes scl | no | yes scl | no | no | no | no | ? | ? | no | no bypassed |
| macro controls | no | no | no | no | no | no | no | no | no | no | no | no | no | no | yes 4-knobs | ? | ? | no | no | ? | ? | ? | ? | no | yes 100 | no | yes morph | yes 4 | no | yes 16 |
| effects in the instrument | no clip-only | no clip-only | no clip-only | no clip-only | no clip-only | no clip-only | partial drive | partial bus-clip | partial bus-clip | no | partial bus-clip | no clip-only | no clip-only | yes distortion+limiter | partial saturation | ? | ? | partial distortion | no | yes 13 | partial LADSPA | yes 6+comp | yes cell+bus+master | yes inserts+sends | yes 35 | partial gain-drive | partial EQ+drive | yes 15 | partial 2-sends+master | yes rack+returns |
| oversampling and aliasing | partial unoversampled-clip | partial unoversampled-clip | partial unoversampled-clip | partial blep | partial blep | partial blep | no unoversampled-drive | partial mixed | partial naive-tri | no truncated-table | partial mixed | partial unoversampled-clip | partial unoversampled-clip | no naive | no none | ? | ? | no | partial resampler | yes sinc | partial Hermite | partial cubic | ? | ? | ? | ? | yes oversampled | ? | ? | ? |
| sample playback and zones | no | no | no | no | no | no | no | no | partial built-loop | no | no | no | no | partial osc-sample | no | no | partial mallet-sample | no | yes power-select | yes SFZ-zones | yes layers+RR | partial 1-per-key | yes 128-layers+RR | yes layers+RR | yes 24-layers | no | no | yes sample-layers | partial 1-per-slot | yes 128-pads |
| drum synthesis models and layering | yes 4-burst | yes bar+rim | yes swept+slap | yes 2-square | yes ride/crash | yes closed/open | yes bridged | yes 9-circuits | yes 5-hybrid | partial 5-recipes | partial 4-circuits | yes 2-mode+noise | yes swept+skin | yes 3x3-layers | yes 6-drums | partial kick-only | yes 9-models | partial kick-only | no | partial region-stack | partial components | no | no | no | no | yes 11-circuits | yes 8-voices | yes 5-layers | no | yes 8-DS |
| vocal formant and glottal models | no | no | no | no | no | no | no | no | no | no | no | no | no | no | yes speech | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no |
| CPU per voice | low est | low est | low est | low est | mid est | mid est | low est | high est | mid est | low est | mid est | low est | low est | low pre-rendered | ? | ? | ? | low est | partial streaming | ? | ? | ? | partial DFD | ? | ? | partial low-CPU-mode | low stated | ? | ? | ? |
| pitch envelope | no | no | partial Bend-row | no | no | no | partial fixed | partial fixed | partial fixed | partial fixed | partial fixed | no | partial Bend-row | yes points | partial attack-FM | ? | no | yes start/end+slope | no | yes EG | no | partial LFO | yes pitch-env | yes pitch-env | yes pitch-env | partial circuit | yes decay-mod | yes Bezier | no | yes Drum-Sampler |
| velocity response | partial level | partial level | yes slap | partial level | partial level | partial level | partial level | partial 2-step | partial 2-step | partial level | partial level | partial level | partial level | partial level | yes accent | ? | yes 5-targets | ? | yes curve | yes curves | yes layers | partial curve | yes vol+pitch | yes curves | yes curve+gate | partial accent | yes 3-targets | ? | partial vol+start | partial volume |
| accent | no | no | no | no | no | no | no | yes row | yes row | no | no | no | no | no | yes | ? | no | no | no | no | no | no | no | partial beat-accents | ? | yes total | yes per-step | no | yes accentuator | no |
| choke groups | no | no | no | no | no | yes open/closed | no | partial hat-only | partial hat-only | yes switch | partial hat-only | no | no | yes groups | no | no | no | no | yes groups+directed | yes off_by | yes mute-groups | yes groups | yes excl | partial cymbal | partial cymbal | ? | yes groups | no | yes mute-group | yes 16 |
| chromatic pitch by note | no | no | no | no | no | no | no | no | no | no | no | no | no | ? | yes V/oct | yes note+glide | yes keyed | partial note-ends | no | yes keyrange | no | partial per-key | partial per-cell | no | no | no | partial pitched | yes keytrack | no | partial Simpler |
| articulations by note | no | yes clave/rim | yes 3-heights | no | yes ride/crash | yes closed/open | no | yes GM-map | yes GM-map | partial 5-notes | yes GM-map | no | yes 3-heights | partial key-per-drum | no | no | no | no | yes kit-map | yes keyswitch | yes per-instrument | yes per-key | yes 10-articulations | yes stroke-types | yes articulations | partial 11-keys | partial 8-keys | no | partial 8-slots | yes 128-pads |
| per-drum controls | yes 4-rows | partial 3-rows | yes 4-rows | partial 3-rows | yes 5-rows | yes 5-rows | yes 6-rows | partial tune+level | partial tune+level | partial level+decay | partial level | yes 5-rows | yes 5-rows | yes full-synth | yes 4-knobs | ? | yes many | yes 11-rows | partial mixer | partial per-region | yes per-instrument | yes per-element | yes per-cell | yes per-piece | yes per-piece | yes per-voice | yes per-channel | yes per-layer | yes per-slot | yes per-chain |
| humanize and swing | no | no | no | no | no | no | no | no | no | no | no straight | no | no | partial humanizer | no | no | no | no | yes timing+stamina | partial random | yes gaussian+swing | no | yes sound+time | yes transformer | yes humanize | yes shuffle+flam | yes swing | no | yes grooves+nudge | no host |
| sequencer and patterns | no | no | no | no | no | no | no | no | no | no | yes 16-presets | no | no | no | no | no | no | no | no | no | yes song-mode | no | no | yes grid+library | yes song-creator | yes 16x8-variations | yes 12-patterns | no | yes 8x16 | no host |
| stereo, pan and outputs | no mono | no mono | no mono | no mono | no mono | no mono | no mono | no mono | no mono | no mono | no mono | no mono | no mono | yes 16-outs | partial out+aux | ? | ? | no mono | yes per-mic | yes pan+width | yes JACK-outs | partial pan+width | yes direct-outs | yes 18-outs | yes 16-stereo | yes pan+11-outs | yes pan+8-outs | ? | yes pan+multi | yes chains-to-tracks |
| mic bleed and rooms | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | no | yes multi-mic | no | no | no | no | yes OH+room | yes 11-mics | no | no | no | no | no |
| programs (kits, presets) | no | no | no | no | no | no | no | no | no | no | no | no | no | partial kit-files | no | ? | ? | ? | yes 6-kits | no format | yes kits+repo | partial pack | yes 129 | yes hundreds | yes 7-kits | yes 128/bank | yes thousands | yes 540+ | yes 240+ | ? |
| noise models | partial white | no | partial white | no | no | no | partial white | partial white | partial white | partial LCG | partial white | partial white | partial white | yes white+brown+density | yes white+clocked | partial level-follow | yes noise-exciter | partial white | no | yes uniform+gauss | no | no | no | partial recorded | no | yes circuit | yes filtered-stereo | ? | no | yes white/pink |

## Lacking

Answering round 0's word — stand beside the open and the popular instruments, family by family; learn the methods; what many need, the library gives once.

### Every instrument (shared — a `core/` matter)

- velocity to timbre: `CORE::PERCUSSION::struck` clamps velocity to a level and nothing else (conga's slap alone reaches the body). Velocity to tone, decay and pitch-sweep depth, through a curve (Microtonic three targets, RipplerX five, Plaits accent, Battery vol+pitch, AD2 and DrumGizmo curves). One `struck` with a curve and a timbre depth in `core/percussion` serves all thirteen.
- the pitch envelope as rows: kick's sweep is fixed (depth and 30 ms time are constants), machine's bends are constants, tom and conga have a Bend depth but no time. A depth, time and curve per swept drum (Kicker, KICK 3, Geonkick, Microtonic, Battery, AD2, the DS devices); `CORE::PERCUSSION::Sweep` gains the time and the curve once.
- anti-aliasing of the nonlinear stages: every circuit ends in `SHAPER::soft`, kick drives it up to nine times, all at the host rate. `CORE::SHAPER::Oversampler` stands and no drum uses it (Befaco Kickall runs 8x, Microtonic oversamples).
- stereo: every drum writes one mono sample to every lane. A Pan row per drum in kit, machine, rhythm and percussive, and a stereo noise/metal spread (TR pan per voice, Microtonic pan, XO Haas width, AD2 width). Separate outputs per drum (TR 11, Microtonic 8, Geonkick 16, AD2 18) need several AUDIO outs — a seam question for an engine order, not designed here.
- programs: no kit or preset anywhere (TR 128 kits a bank, Microtonic, KICK 3 540+, XO 240+, Geonkick kit files); a bank of our own per instrument on the PROGRAM event.
- choke groups beyond the hat: a group row per drum in kit and machine with a short fade, not a cut (Battery excl, Drum Rack 16, Microtonic, Geonkick, DrumGizmo, sfizz off_by, FluidSynth exclusive class).
- chromatic play: kick, tom, conga, clave and cowbell ignore the note's pitch; a Key switch so the note sets the tune about a root (KICK 3 keytrack, ChowKick with glide, Plaits V/oct, RipplerX), with microtuning riding it later (ChowKick, RipplerX).
- MPE and aftertouch: aftertouch choking a cymbal (AD2, sfizz poly-AT, drumkv1); the seam has no pressure event, so this is a finding for an engine order first.
- a sample layer under the synthesis (KICK 3 layers, Geonkick sample oscillator, Battery) waits on a sample reaching a plugin — a seam question, recorded.

### clap

- a Tail tone apart from the bursts' tone and a randomised burst spacing (DS Clap Sloppy, Faust `sy.clap`, the 808 clap's reverb tail).
- burst count as a row (four is a constant) and velocity to spread.
- a resonant low-pass after the bandpass and a noise-colour row (Microtonic noise filter modes, Faust `sy.clap`).

### clave

- a Tone row (the rim's crack filter is a constant) and a Rim tune apart from the bar's.
- velocity to brightness of the strike, a stick model's one-pole on the excitation (STK Modal, Faust `strikeModel`).

### conga

- a Mute (hand-damp) articulation by note, not by velocity alone (TR conga/tom switch, Battery articulations).
- a Tone row and a Bend time row; more body modes with higher ones decaying faster (Faust djembe, RipplerX membrane).

### cowbell

- a Tone row (the bandpass centre is a constant) and the two squares' interval as a row (TR CB tuning; the 808 cowbell's own trimmers).
- velocity to the clank's share.

### cymbal

- three bands with their own envelopes, as the circuit has (Werner et al.'s 808 cymbal), in place of two tails; a choke by note and by aftertouch (AD2, SD3).
- a high-pass Tone row apart from the band balance, and a bell articulation (SD3, AD2 ride bell).
- noise in the wash (Plaits hi-hat noisiness, Microtonic noise).

### hat

- a pedal articulation (44) apart from closed, and a half-open by velocity (AD2, SD3 stroke types).
- a noise share row and a ring-modulated pair voicing (Plaits hi-hat both sources).
- a two-stage decay and a swing-style VCA (Plaits hi-hat, the circuit).

### kick

- the pitch envelope as rows (shared, above) and a Click kind (noise, pulse, sample) in place of the one noise snap (KICK 3, Kicker, DS Kick).
- the circuit's own sigh: the retrigger pulse leaking into the bridged-T after the attack, and the pulse shaper's asymmetric clip (Werner et al. 808 bass drum, ChowKick).
- oversampled drive (shared) and a post-drive EQ (KICK 3 two bands and shelves).

### kit

- per-drum Decay and Tone rows beside Tune and Level — every drum's own circuit rows already exist; the kit hides them (TR, Microtonic, Battery, Drum Rack).
- velocity beyond two steps: `accented` throws the value away below 0.75 (shared velocity line).
- maracas and the second-row drums of the GM map (STK PhISEM shakers; TR's CP/maracas switch).
- CPU: all nine circuits tick every frame, six without a silence check; a sounding test per drum as hat, cymbal and cowbell have (Geonkick renders once and reads).
- pan, choke groups and programs (shared).

### machine

- per-drum Decay, Tone and Attack rows (TR 909 knobs: BD Attack/Decay, SD Tone/Snappy, HH decays).
- the cymbals the second machine carries (crash and ride, its rimshot); today five drums.
- the hat loop's colour and length as rows (it is drawn once at create); a Tone row on it.
- velocity, pan, choke, programs (shared).

### percussive

- the founding exemplar stands outside the library's laws: its own row table (no `CORE::TABLE` sheet, 379 law 4), its own meter and denormal flush, a truncated sine-table read. Re-found on `core/percussion` and `core/table`.
- tune, tone and bend rows per slot (its recipes are constants); velocity beyond level.

### rhythm

- swing: `CORE::CLOCK` holds it and rhythm settles it straight (TR shuffle, Microtonic, Hydrogen, XO grooves).
- the host's tempo: no TRANSPORT reaches a plugin, so it carries its own Tempo and Run rows (379 "The seam") — a finding, not this lane's.
- accent steps and pattern variations (TR A–H, weak steps, flams; Microtonic accent per step).
- more voices: toms, clap, rimshot, cowbell from the circuits that stand (the preset boxes carry them).
- humanize: timing and velocity jitter and stamina (DrumGizmo, Hydrogen); the `humanizer` note effect stands beside but cannot reach inside the box.

### snare

- the snare's own pitch drop and a rattle with a hold before decay (Plaits synthetic snare, Microtonic).
- a Tone mode row crossfading two-mode body into a bright multi-mode body (Plaits analog snare); noise through a pitch-tracking bandpass.
- rimshot and side-stick articulations by note (AD2, SD3, the GM map 37/40).

### tom

- a Bend time row and a pitch drop shape (shared pitch envelope); a mute/flam articulation.
- more body modes with higher ones decaying faster (Faust `additiveDrum`, RipplerX drumhead).
- chromatic play for melodic toms (shared).

## Methods

| Method (own words) | Source and file | Licence | Lands |
|---|---|---|---|
| bass drum as a resonant bandpass pinged by a short shaped pulse; a second short pulse pushes the frequency up during the attack; the filter's own low-passed output feeds back into its frequency (self-FM) for the sigh; decay maps exponentially to resonance; tone is a low-pass plus click leakage | Plaits `plaits/dsp/drums/analog_bass_drum.h` | MIT | core/percussion `Kick` (kick, kit, rhythm) |
| synthetic bass drum: a saturated-triangle sine whose phase increment is scaled by a decaying FM envelope; dirt (phase noise and shaping) eased off as pitch rises; click and filtered noise; a transistor-VCA soft clip before the tone filter | Plaits `plaits/dsp/drums/synthetic_bass_drum.h` | MIT | core/percussion `HYBRID::Kick` (machine) |
| snare body as five bandpass modes on a fixed inharmonic series, upper ones damped harder; tone crossfades a two-mode voicing into the bright multi-mode one; noise half-wave rectified, through a bandpass that tracks pitch; snappy sets both the balance and the noise decay | Plaits `plaits/dsp/drums/analog_snare_drum.h` | MIT | core/percussion `Snare` (snare, kit, rhythm) |
| synthetic snare: two distorted sines coupled by phase reset at a fixed ratio, with a pitch drop; noise through a pitch-tracking low-pass and high-pass with a hold-then-decay envelope; snappy as an equal-power mix | Plaits `plaits/dsp/drums/synthetic_snare_drum.h` | MIT | core/percussion `HYBRID::Snare` (machine) |
| hi-hat source as six inharmonic squares or three square-times-saw ring pairs; a bandpass whose centre follows tone; clocked random noise crossfaded in; a swing (asymmetric, saturating) or linear VCA with an optional two-stage decay; a high-pass at the band's frequency last | Plaits `plaits/dsp/drums/hi_hat.h` | MIT | core/percussion `Hat`, `Metal` (hat, cymbal, kit, machine) |
| harmonics knob split into three ranges: attack FM, then self-FM, then overdrive | Plaits `plaits/dsp/engine/bass_drum_engine.cc` | MIT | kick (a Punch macro) |
| 808 bass drum excitation as a positive pulse, a delayed negative pulse and a short DC plateau, then a brief pitch lift | Peaks `peaks/drums/bass_drum.cc` | MIT | core/percussion `Kick` |
| a high-Q hat bandpass run twice per sample for stability; a half-wave rectifying VCA | Peaks `peaks/drums/high_hat.cc` | MIT | core/percussion `Hat` |
| the bass drum circuit partitioned into blocks: accent-scaled trigger, a shelf-plus-diode pulse shaper, a bridged-T bandpass in an op-amp's feedback whose effective resistance an envelope lowers for the first milliseconds (the pitch jump), a retrigger pulse whose leakage makes the sigh, a feedback buffer carrying decay with op-amp clipping, then tone and level; retriggering before the tail dies is modelled, not reset | Werner, Abel, Smith, DAFx-14, "TR-808 Bass Drum Circuit" | paper (DAFx-14 proceedings) | core/percussion `Kick` (kick, kit, rhythm) |
| the metal voices share one bank of six square oscillators; two bandpasses sum clusters of it; three bands each with an envelope, a swing VCA and a high-pass; a tone stage mixes the bands; alias components left to the filters | Werner, Abel, Smith, ICMC/SMC 2014, "The TR-808 Cymbal" | CC-BY-3.0 | core/percussion `Cymbal` (three bands), `Metal` (one shared bank in kit) |
| pulse shaper as a component-level RC stage with its resistances mapped from sustain and decay; the resonant filter with a selectable nonlinearity; noise level following the drum's own level raised to a decay exponent | ChowKick `src/dsp/PulseShaper.cpp`, `ResonantFilter.cpp`, `Noise.cpp` | BSD-3-Clause | kick |
| the whole kick voice behind an 8x oversampler; pitch envelope with a cubic bend over a decay time added to a base tune | Befaco Kickall `src/Kickall.cpp` | GPL-3.0-or-later | core/shaper `Oversampler` (every drum's clip), core/percussion `Sweep` |
| sine swept from a start to an end frequency on a power law of normalised time; amplitude one minus time to a slope power; noise scaled by the envelope squared so it dies first; click as the starting phase; drive threshold interpolated across the hit | LMMS Kicker `plugins/Kicker/KickerOsc.h`, `Kicker.cpp` | GPL-2.0-or-later | kick (Start, End, Slope rows) |
| the hit rendered whole into a buffer whenever a row changes; a note only reads it | Geonkick `src/dsp/src/synthesizer.c` | GPL-3.0-or-later | machine (as its hat loop does), kit CPU |
| envelopes as point lists with linear or cubic Bezier segments, the Bezier solved for time by bisection; frequency envelope applied in the log domain | Geonkick `src/dsp/src/envelope.c`, `oscillator.c` | GPL-3.0-or-later | core/envelope (pitch and amp curves) |
| density noise: each sample is zero or random with a probability an envelope sets; brown noise as a bounded walk | Geonkick `src/dsp/src/oscillator.c` | GPL-3.0-or-later | core/noise (clap, hat grit) |
| clap as noise times four staggered envelopes, a resonant low-pass, a tanh | Faust `synths.lib` (`sy.clap`) | LGPL-2.1+ with exception | clap (confirms ours; adds the low-pass) |
| strike as a noise burst through a high-pass and low-pass whose cutoff the strike position sets; a mode bank whose higher modes decay sooner | Faust `physmodels.lib` (`strikeModel`, `djembeModel`), `synths.lib` (`additiveDrum`) | LGPL-2.1+ with exception | core/percussion `Resonator` banks (tom, conga, clave) |
| shakers: shake energy decays; each sample a collision fires with a probability set by the bead count and adds to a sound level that decays; that level times noise drives a few resonators jittered per collision | STK `src/Shakers.cpp`, `include/Shakers.h` | STK (MIT-like) | core/percussion (a shaker for kit's maracas) |
| one resonator per mode at a ratio and radius; the excitation passed through a one-pole whose pole the strike strength sets, so harder is brighter | STK `src/Modal.cpp` | STK (MIT-like) | core/percussion `Resonator` strike (velocity to brightness) |
| partials stretched by an inharmonicity amount, decay divided by a power law of frequency, gain following a sine of partial index times hit position (a comb) | RipplerX `src/dsp/Partial.cpp` | GPL-3.0 | tom, conga, clave (Position and Material rows) |
| membrane as a triangular waveguide mesh masked to a circle; tension sets junction admittance, loss scales outputs | SuperCollider sc3-plugins `source/MembraneUGens/Membrane.cpp` | GPL-2.0-or-later | tom, conga (later; high CPU) |
| velocity remapped through a monotone cubic Hermite curve (slopes limited so it never reverses) with an optional flat shelf | DrumGizmo `src/powermap.cc`, `src/powermapfilter.cc` | LGPL-3.0-or-later | core/percussion `struck` (all) |
| timing humanizer: a laid-back offset plus a drift that takes a Gaussian step each hit and relaxes with rest, clamped | DrumGizmo `src/latencyfilter.cc` | LGPL-3.0-or-later | rhythm, notes/humanizer |
| stamina: velocity times a per-drum factor that shrinks each hit and recovers linearly with rest | DrumGizmo `src/staminafilter.cc` | LGPL-3.0-or-later | rhythm, kit |
| choke by group: a new hit shortens the release of the group's other voices (fast, timed, or normal) and spares the same note | sfizz `src/sfizz/Voice.cpp`, `Synth.cpp` | BSD-2-Clause | core/percussion (a choke fade: kit, machine) |
| exclusive class: the choked voice gets a very short release tuned on hats, then a note-off | FluidSynth `src/synth/fluid_voice.c` | LGPL-2.1-or-later | core/percussion (choke fade) |
| humanize as Gaussian offsets to velocity, timing and pitch; swing delays the off-beat eighths | Hydrogen `src/core/Basics/Note.cpp` | GPL-2.0-or-later | rhythm |
| velocity squared then raised to a curve exponent | drumkv1 `src/drumkv1.cpp` | GPL-2.0-or-later | core/percussion `struck` |

No line, table, constant or coefficient is taken from any of these; each row is the idea in this survey's words.

## Sources

| Name | Licence | Home | Read |
|---|---|---|---|
| Geonkick | GPL-3.0-or-later | https://github.com/Geonkick-Synthesizer/geonkick | README; `src/dsp/src/oscillator.c`, `envelope.c`, `synthesizer.c`, `filter.c`, `distortion.c` |
| Mutable Instruments eurorack (Plaits, Peaks) | MIT | https://github.com/pichenettes/eurorack | `plaits/dsp/drums/analog_bass_drum.h`, `synthetic_bass_drum.h`, `analog_snare_drum.h`, `synthetic_snare_drum.h`, `hi_hat.h`; `plaits/dsp/engine/bass_drum_engine.cc`, `hi_hat_engine.cc`; `peaks/drums/bass_drum.cc`, `snare_drum.cc`, `high_hat.cc`, `fm_drum.cc` |
| ChowKick | BSD-3-Clause | https://github.com/Chowdhury-DSP/ChowKick | README; `src/dsp/PulseShaper.cpp`, `ResonantFilter.cpp`, `Noise.cpp` |
| Befaco (VCV Rack) Kickall | GPL-3.0-or-later (code) | https://github.com/VCVRack/Befaco | LICENSE.md; `src/Kickall.cpp` |
| RipplerX | GPL-3.0 | https://github.com/tiagolr/ripplerx | README; `src/dsp/Partial.cpp`, `Resonator.cpp`, `Mallet.cpp`, `Voice.cpp` |
| LMMS (Kicker) | GPL-2.0-or-later | https://github.com/LMMS/lmms | `plugins/Kicker/Kicker.cpp`, `KickerOsc.h`; `include/DspEffectLibrary.h` |
| Faust libraries | LGPL-2.1+ with exception | https://github.com/grame-cncm/faustlibraries | `physmodels.lib` (most), `synths.lib`; synths docs page |
| STK | STK licence (MIT-like) | https://github.com/thestk/stk | LICENSE; `src/Shakers.cpp`, `include/Shakers.h`, `src/ModalBar.cpp`, `src/Modal.cpp`, `src/BandedWG.cpp`, `src/Drummer.cpp` |
| SuperCollider, sc3-plugins | GPL-3.0 (core); GPL-2.0-or-later (files read) | https://github.com/supercollider/supercollider | `server/plugins/OscUGens.cpp` (Klank), `SCClassLibrary/Common/Audio/FSinOsc.sc`, DynKlank help; sc3-plugins `source/MembraneUGens/Membrane.cpp`, `Membrane_shape.c` |
| DrumGizmo | LGPL-3.0-or-later | https://drumgizmo.org | wiki kits and docs pages; Debian's copy of 0.9.20 `src/sample_selection.cc`, `powerlist.cc`, `powermap.cc`, `powermapfilter.cc`, `inputprocessor.cc`, `latencyfilter.cc`, `staminafilter.cc`, `velocityfilter.cc`, `drumgizmo.cc`, COPYING |
| sfizz | BSD-2-Clause | https://github.com/sfztools/sfizz | sfz.tools opcode and engine pages; `src/sfizz/Layer.cpp`, `Synth.cpp`, `Voice.cpp`, `VoiceStealing.cpp`, `PolyphonyGroup.cpp`, `SfzFilter.h`, `Interpolators.h` |
| Hydrogen | GPL-2.0-or-later | https://github.com/hydrogen-music/hydrogen | COPYING; `src/core/Sampler/Sampler.cpp`, `src/core/Basics/Note.cpp`, `Note.h`, `Instrument.cpp` |
| drumkv1 | GPL-2.0-or-later | https://drumkv1.sourceforge.io | home page; `src/drumkv1.cpp`, `src/drumkv1_sample.h` |
| Drumlabooh | GPL-3.0 | https://github.com/psemiletov/drumlabooh | README; `Source/kits.cpp`, `fx-resofilter.cpp`, `dsp.cpp` (not a column: a kit player like DrumGizmo) |
| FluidSynth | LGPL-2.1-or-later | https://github.com/FluidSynth/fluidsynth | `src/synth/fluid_voice.c`, `fluid_synth.c` (part), `src/rvoice/fluid_rvoice_dsp.cpp` |
| Werner, Abel, Smith — TR-808 bass drum | paper (DAFx-14) | https://dafx14.fau.de/papers/dafx14_kurt_james_werner_a_physically_informed,_ci.pdf | pages 1–4 (block structure) |
| Werner, Abel, Smith — TR-808 cymbal | CC-BY-3.0 | https://speech.di.uoa.gr/ICMC-SMC-2014/images/VOL_2/1453.pdf | pages 1–2 (block structure) |
| Werner, Abel, Smith — TR-808 cowbell | paper (AES 137) | https://ccrma.stanford.edu/papers/more-cowbell-physically-informed-circuit-bendable-digital-model-of-tr-808-cowbell | citation page only |
| Sound On Sound, Synth Secrets (drums) | commercial | https://www.soundonsound.com/techniques/synthesizing-cowbells-claves | search summaries only |
| Native Instruments Battery 4 | commercial | https://www.native-instruments.com/en/products/komplete/drums/battery-4/ | online manual (main, modulation, setup, cell matrix, master, editor, effects pages); product page |
| XLN Audio Addictive Drums 2 | commercial | https://www.xlnaudio.com/products/addictive_drums_2 | AD2 manual (PDF, 2024); product page |
| Toontrack Superior Drummer 3 | commercial | https://www.toontrack.com/product/superior-drummer-3/ | product page; 3.1.0 release notes; Sound On Sound review; Toontrack forum (the manual is behind a login) |
| Roland Cloud TR-808 / TR-909 | commercial | https://www.roland.com/us/products/rc_tr-909/ | TR-909 owner's manual (Roland Cloud PDF); TR-808 owner's manual (third-party copy); both product pages |
| Sonic Charge Microtonic | commercial | https://soniccharge.com/microtonic | product page; Sonic State, CDM, KVR, MusicRadar; vendor forum (the manual PDF did not read) |
| Sonic Academy KICK 3 | commercial | https://www.kvraudio.com/product/kick-3-by-sonic-academy/details | MusicTech review; KVR news and details (the maker's page refused the reader) |
| XLN Audio XO | commercial | https://www.xlnaudio.com/products/xo | XO getting-started guide (PDF); product page; Attack Magazine review |
| Ableton Drum Rack, Drum Sampler, DS devices | commercial | https://www.ableton.com/en/manual/instrument-drum-and-effect-racks/ | Live 12 manual: racks, instrument reference, Max for Live devices, tuning systems, MPE |

## Moved

none
