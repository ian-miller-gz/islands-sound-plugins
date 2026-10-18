# vocal — round 1

## Matrix

A cell is `yes` / `no` / `partial` and one word; `?` is unverified. The CPU row reads `low` / `mid` / `high` and how it is known (`est` = estimated from the per-sample work in the code, `stated` = the maker's or the project's own words). Ours are read from this worktree (`instrument/choir`, `instrument/vowel`, `instrument/whisper`, and `core/glottis` and `core/filter`'s formant bank under them). The open columns are Pink Trombone (PT), VocalTractLab (VTL), the Faust physmodels vocal section, eSpeak NG, klatt-syn, Plaits (its speech engine), ZynAddSubFX (Zyn), Vital, and OpenUtau with WORLD. The popular columns are Synthesizer V Studio 2 (SynthV 2), Vocaloid 6, Chipspeech, Alter/Ego, Hollywood Choirs (HC, standing for the sampled choir), Omnisphere 3 and Delay Lama. After the commission's own rows come the family's: glottal source through programs.

| Capability | choir | vowel | whisper | PT | VTL | Faust vocal | eSpeak NG | klatt-syn | Plaits | Zyn | Vital | OpenUtau+WORLD | SynthV 2 | Vocaloid 6 | Chipspeech | Alter/Ego | HC | Omnisphere 3 | Delay Lama |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| oscillator kinds | partial glottal-table | partial glottal-table | partial four-noises | partial LF-pulse | partial glottis-models | partial impulse/saw | yes harmonic+Klatt | yes three-sources | yes 16-engines | yes Add/Sub/PAD | yes WT+sample | no resynthesis | no neural | no neural/concat | yes LPC/formant/VOSIM | partial resynthesis | no samples | yes samples+synth | partial one-wave |
| unison and detune | yes 8-singers | no | no | no | no | no | no | no | no | yes per-voice | yes stacks | no | no | partial doubling | ? | no | no | yes unison | no |
| wavetable morphing | no fixed-Open | partial Open-frames | no | no | no | no | no | no | yes 8x8 | no | yes spectral | no | no | no | no | no | no | yes 638-waves | no |
| FM algorithms and operators | no | no | no | no | no | no | no | no | partial 2-op | partial FM/RM/PM | partial warp | no | no | no | no | no | no | yes FM | no |
| filter models | partial formant-only | partial formant-only | partial formant-only | partial waveguide | partial tube | yes FOF+bandpass | yes Klatt-resonators | yes poles+zeros | partial LPG | yes analog+SVF+formant | yes 6+formant | partial LPF-expression | no | no | partial internal | ? | ? | yes many | yes formant |
| modulation sources and the matrix | partial fixed-LFOs | partial vibrato+ADSR | partial ADSR-only | no | partial gestural-score | no | no | no | no CV | partial per-parameter | yes matrix | partial curves | partial curves | partial curves | partial CC | partial CC | partial engine | yes matrix | partial XY |
| voice modes — poly, mono, legato, glide | partial poly-6 | partial poly-8 | partial poly-8 | partial mono-glide | no | partial MIDI-poly | no | no | no mono | yes poly/mono/legato/porta | yes poly/legato/glide | partial mono-track | partial mono-track | partial mono-track | ? | yes poly | partial legato | yes poly/mono/glide | ? mono |
| MPE and aftertouch | no seam | no seam | no seam | no | no | no | no | no | no | ? | yes MPE | no | ? | ? | ? | ? | ? | yes MPE | no |
| microtuning | no 12TET | no 12TET | no 12TET | no | no | no | no | no | no | yes Scala | yes scl/kbm | partial pitch-curves | ? curves | ? | ? | ? | ? | yes .tun | no |
| macro controls | no | no | no | no | no | no | partial voice-files | no | yes 3-knobs | ? | yes 4 | no | no | no | no | partial CC | ? | yes global | no |
| effects in the instrument | yes ensemble+room | no | no | no | no | no | partial echo | no | no | yes 8+ | yes 9 | no | ? | yes 11 | ? | ? | yes mixer | yes 93 | yes delay |
| oversampling and aliasing | no table-unlimited | no table-unlimited | partial unpitched | partial tract-2x | ? | no impulse-train | ? | no | partial BLEP | ? | yes 2x | ? offline | ? | ? | ? | ? | ? samples | ? | ? |
| sample playback and zones | no | no | no | no | no | no | partial stored-consonants | no | no | no | partial one-sample | yes voicebanks | no | partial concatenative | partial segments | partial voicebank | yes multimic | yes zones | no |
| drum synthesis models and layering | no | no | no | no | no | no | no | no | yes 3-engines | no | no | no | no | no | no | no | no | ? | no |
| vocal formant and glottal models | partial 5-band+pulse | partial 5-band+pulse | partial 5-band+noise | yes LF+waveguide | yes articulatory | yes FOF+types | yes Klatt | yes Klatt | yes formant+LPC | partial formant-filter | partial formant-filter | yes WORLD | yes neural | yes neural+concat | yes 10-methods | yes resynthesis | partial sampled | ? vowel-filter | partial formant |
| glottal source | partial Rosenberg-table | partial Rosenberg-table | no noise | yes LF-tenseness | yes 3-models+LF | no impulse | partial Klatt-natural | yes KLGLOTT88+tilt | yes pulse | no | no | yes WORLD-pulse | yes Voicing | partial neural | yes pulse/LPC | partial impulses | no | no | ? pulse |
| aspiration and breath | yes pulsed-Breath | yes pulsed-Breath | yes all-noise | yes cycle-shaped | yes glottal-noise | partial noise-mix | yes | yes open-phase | partial LPC-noise | partial noise | no | yes aperiodicity | yes Breathiness | yes Air | ? | ? | no | no | ? |
| vocal tract | partial 5-bandpass | partial 5-bandpass | partial 5-bandpass | yes KL-waveguide | yes 3D-tube | yes formant-bank | yes cascade/parallel | yes cascade/parallel | yes formant+LPC | yes formant-bank | partial formant-filter | yes spectral-envelope | yes neural | yes neural | yes formant/LPC/waveguide | yes resynthesis | partial sampled | partial samples+filter | partial formant |
| vowel set and morph | partial 5-adjacent+LFO | partial 5-adjacent | partial 5-adjacent | yes continuous | yes articulatory | yes 5-morph | yes per-language | partial parameters | yes 2-D | yes sequence | yes 2-D-pad | yes phonemes | partial MouthOpening | no | partial phonemes | partial phonemes | yes vowel-patches | ? vowel-filter | yes 5-XY |
| consonants | no | no | no | partial fricatives+clicks | yes full | partial noise | yes | yes frication | partial word-banks | no | no | yes | yes | yes | yes | yes | yes WordBuilder | no | no |
| nasals | no | no | no | yes velum | yes nasal+sinuses | no | yes pole/zero | yes pole/zero | ? | no | no | yes phonemes | yes phonemes | yes phonemes | ? | ? | partial WordBuilder | no | no |
| lyrics and phonemes | no | no | no | no | partial SAMPA | no | yes TTS | no | partial word-banks | no | no | yes phonemizers | yes 6-languages | yes 3-languages | yes typed | yes lyrics | yes WordBuilder | no | no |
| voice type and formant shift | partial Sex+Spread | partial Sex | partial key-Tracking | ? tract-length | yes anatomy | yes 5-types | yes voice-files | partial parameters | yes species | partial octave-shift | yes shift+spread | yes GEN | yes Gender+ToneShift | yes Character | partial per-voice | yes helium/fem | partial male/female | no | no |
| vibrato, jitter, drift | partial vibrato+detune | partial vibrato+Delay | no | yes vibrato+wobble | partial flutter | partial vibrato | partial flutter | partial flutter | no | yes LFO | yes LFO+random | yes editor | yes curves | yes VibratoTool | partial ? | yes 9-parameters | partial vib/non-vib | yes LFO | ? |
| ensemble and choir | yes 8-singers+scatter | no | no | no | no | no | no | no | no | yes PAD+unison | partial unison | no | partial choir-banks | partial doubling | ? | partial poly | yes 2-sections | partial choir-samples | no |
| articulations | no | no | partial whisper | partial tenseness | ? | no | no | no | no | no | no | partial expressions | yes vocal-modes | partial styles | partial growl/talk | partial whisper | yes legato/staccato | no | no |
| stereo | yes seat-pans | no mono | no mono | no | no | no | no | no | no | yes | yes | no | ? | ? | ? | ? | yes mics | yes | yes delay |
| programs | no | no | no | no | partial speaker-files | no | yes voice-files | no | no | yes banks | yes presets | yes voicebanks | yes voicebanks | yes voicebanks | yes voices | yes voicebanks | yes patches | yes library | no |
| CPU per voice | high est | mid est | low est | low est | high est | low est | low est | low est | low stated | partial PAD-cheap | partial SIMD | high offline | ? offline | ? | ? | ? | ? | ? | ? |

## Lacking

This answers round 0's word: stand beside the open and the popular instruments, family by family; learn their methods; what many instruments need, the library gives once. All three instruments ride `CORE::FILTER::Formant`, and two of them ride `CORE::GLOTTIS`, so most of what follows is a core gain first. The neural and concatenative singers (SynthV 2, Vocaloid 6, Alter/Ego, OpenUtau) are a rival class, not a target: they render offline from lyrics. Our place is the real-time, playable source-filter voice, where PT, Klatt, Plaits, Faust and Delay Lama stand.

### Every instrument (shared — a `core/` matter)

- voice types in the formant core: several register sets (bass, tenor, alto, soprano), blended bilinearly by a register place and a vowel place, so a voice row picks a register and Sex stays a fine shift (Faust 5 types, Plaits and Braids voice × vowel grid, eSpeak voice files). Today there is one tenor set, and every voice type is that set scaled by one ratio.
- vowel targets in our own terms: the core's five-vowel table matches the Csound manual's tenor appendix (A, E, I and U value for value; O in centres and levels). The design stage re-derives the targets from phonetics literature, stated in our own words (391: no constant table taken).
- free vowel morph: any two vowels, or a 2-D vowel plane with a vowel at each corner, interpolated in log frequency (Vital's pad, Odin 2, Kilohearts, Delay Lama's XY). Today Morph runs only toward the next entry of A→E→I→O→U.
- formant smoothing: coefficients glided per sample or crossfaded per block, so turning Vowel or Morph does not zipper (Zyn's slowness, Faust's fofCycle, PT's block crossfade). Today a CONTROLLER event re-settles the biquads in a single step.
- a width scale and level normalisation in the formant core: wider bands for whisper and breath, and a band gain that holds level as Q changes (whisper's fixed ×8 makeup is a guess).
- pitch-aware formants: F1 bent toward a high fundamental, and the upper formants raised with effort (velocity and pitch), so high soprano notes stay sung (Faust autobend and vocalEffort, after chant-lib).
- a band-limited glottal pulse: correct the closure edge with BLEP, or keep a table level per octave (Plaits BLEP; the spectral survey asks for per-octave tables in `CORE::OSCILLATOR`). Today the table aliases at high pitch.
- a tension-driven glottal model: one tension control shapes the pulse from lax to pressed, LF style; a one-pole spectral tilt; breath only in the open phase (PT, klatt-syn). This replaces the fixed 0.6 Open that choir uses and the bare Open row.
- jitter, shimmer and drift: small cycle-to-cycle pitch and level wander, plus slow smooth-noise wobble in place of a pure sine (PT). Today vibrato is a single sine and nothing wanders.
- a consonant layer: noise through its own bands for fricatives (S, Sh, F, H), a short burst for plosives, and a nasal pole and zero (Klatt's parallel branch and nasal pair, PT's turbulence and velum, Faust's fricative). All three instruments need it, so it is a core.
- voice modes: Poly / Mono / Legato with Glide over `CORE::VOICE`, which already holds MONO and glide (PT, Zyn, Vital, HC legato). A sung line is monophonic and glides; no instrument here has a row for it.
- microtuning: a per-key table from Scala or MTS behind `CORE::PHASE::Tuning` (Zyn, Vital, Omnisphere). Every family's survey asks for it.
- MPE, aftertouch and bend: the seam comes first, with no pressure event and no BEND reaching a plugin (379 "The seam"). Until then, a Bend row (379's word: bend is a row).
- lyrics: no text or phoneme event reaches a plugin, so typed words are a finding for an engine order. A fixed syllable row is the stand-in.
- programs: a bank of our own per instrument on PROGRAM (every popular column).

### choir

- a Voice row for sections: Bass / Tenor / Alto / Soprano / Mixed over the core's register sets; Mixed seats singers of several registers across the keyboard (HC male/female, Faust 5 types).
- a Singers row (1–8) so a part's cost is a choice; today it is fixed at eight singers × five biquads per note.
- a Tension row (a fixed Open today) and a vibrato Delay row (vowel has one, choir does not).
- per-singer jitter and slow drift over and above the static detune seat (PT wobble). A choir lives on unsynchronised wander.
- a Target vowel or 2-D place for Motion to travel to, in place of "toward the next table vowel".
- syllables: onset consonants from the shared layer (Hollywood WordBuilder, ACE's choir); whole words wait on the seam.
- Mono/Legato for a choir line (HC legato), over `CORE::VOICE`.
- Room Size and Decay rows (constants today).
- cost: measure the voice first. Then consider fewer bands for the back singers, or a PADsynth-style ensemble table under the near singers (Zyn PAD). 6 notes × 8 singers × 5 biquads is the family's heaviest load.
- polyphony: 6 notes; a Voices row once the cost is known.

### vowel

- a Voice row (register) beside Sex, over the core's register sets.
- a Target vowel or 2-D place in place of next-entry Morph, with Motion (an LFO on the place) as choir has it.
- Mode, Glide and Voices rows over `CORE::VOICE`.
- a Tension row with Tilt in place of Open, and a Drift row for jitter and wobble.
- an Effort row: velocity and pitch lift the upper formants (Faust vocalEffort).
- a Consonant row of onset steps (None, S, Sh, F, H, T, K, M, N) drawn from the shared layer (Braids random consonant on strike, Klatt frication).
- stereo: Pan or a light ensemble; one mono sample goes to every lane today.
- cost: pitch is recomputed per sample per voice (`PHASE::hertz`, a pow) for vibrato; choir already does this at control rate.

### whisper

- a Width row over the core's width scale (whispered formants are wide), and a measured level in place of the fixed ×8 makeup.
- Voice (register) and Sex rows, as vowel has; Tracking stays.
- a Voicing row: from whisper through breathy voice, a faint glottal pulse mixed under the noise (SynthV Voicing, Faust excitation mix, Klatt aspiration).
- fricatives and plosives from the shared consonant layer (Klatt frication, PT turbulence): whisper is the instrument most of it.
- a Motion row (an LFO on the vowel place) and a Target vowel, as for vowel.
- stereo: Width, a seeded pan per voice.
- Mode and Glide rows over `CORE::VOICE`.

## Methods

| Method (own words) | Source and file | Licence | Lands |
|---|---|---|---|
| LF glottal pulse driven by one tenseness value: a rising sine under a growing exponential for the open phase, an exponential return, solved to join; reshaped once per period with pitch and tension interpolated | pink-trombone-mod `src/Glottis.ts` | MIT | core/glottis |
| aspiration noise scaled down as tension rises and shaped by the glottal phase | pink-trombone-mod `src/Glottis.ts` | MIT | core/glottis |
| pitch wobble: vibrato plus several slow smooth-noise sources at unrelated rates, and its own slow noise on tension | pink-trombone-mod `src/Glottis.ts` | MIT | core/modulator (a drift source); choir, vowel |
| Kelly-Lochbaum tract: a row of cylinder cells with waves travelling both ways, junction reflection from neighbouring areas, sign-inverting lips, a partly reflecting glottis, slight loss per cell, run at twice the rate | pink-trombone-mod `src/Tract.ts`; Voc `tract.w` | MIT; MIT (generated C public domain) | a new core/tract (a later tract mode or instrument) |
| nasal coupling: the velum opening sets the third area of a three-way junction; lip and nose outputs summed | pink-trombone-mod `src/Tract.ts` | MIT | core/tract |
| turbulence: noise injected where the tract is narrow, scaled by narrowness and airflow, gated by the glottis, split across the two nearest cells; plosives as short impulses into both wave directions | pink-trombone-mod `src/Tract.ts` | MIT | core/tract; the consonant layer |
| reflection coefficients recomputed per block and crossfaded per sample | pink-trombone-mod `src/Tract.ts` | MIT | core/filter (formant smoothing); core/tract |
| Klatt cascade and parallel: the cascade runs a nasal zero, a nasal pole, then the oral resonators in series; the parallel path takes frication into F2 and up, alternating signs; parameters change at period boundaries | klatt-syn `src/Klatt.ts`; eSpeak NG `klatt.c` | MIT; GPL-3.0+ (read for ideas) | core/filter (nasal pair, parallel bands); the consonant layer |
| Klatt sources: impulsive, natural flow derivative with an open-quotient control, or noise; a one-pole tilt; breath only in the open phase, reduced late in the period | klatt-syn `src/Klatt.ts` | MIT | core/glottis |
| voice-type × vowel grid: a 2-D set of formant targets indexed by register and vowel, blended bilinearly, with register acting as the formant shift | Plaits `dsp/speech/naive_speech_synth.cc`; Braids `digital_oscillator.cc` (vowel FOF) | MIT | core/filter (the Voice row) |
| 2-D vowel square: a vowel at each corner, bilinear blend, centres interpolated in note units (log frequency); spread about a centre note and a transpose | Vital `src/synthesis/filters/formant_filter.cpp` | GPL-3.0 | core/filter |
| vowel glide along the geometric mean of the two centres, a cheap log interpolation | Odin 2 `Source/audio/Filters/FormantFilter.cpp` | GPL-3.0+ | core/filter |
| vowel sequence: the control in octaves, stretched and wrapped over a user sequence; a clearness curve holds each vowel before the blend; every formant parameter one-pole smoothed | Zyn `src/DSP/FormantFilter.cpp` | GPL-2.0+ | core/filter |
| pulse-reset sine formants: each formant a sine restarted by every glottal pulse under a per-period window; a BLEP-corrected pulse train; pitch clamped under Nyquist | Plaits `dsp/speech/sam_speech_synth.cc` | MIT | core/glottis (BLEP); a cheap formant voice for choir's back singers |
| LPC lattice: a tenth-order reflection lattice fed by pulse or noise, consecutive frames blended (pitch, voiced and unvoiced energy, coefficients one by one), clock rate as formant shift | Plaits `dsp/speech/lpc_speech_synth.cc`, `lpc_speech_synth_controller.cc` | MIT | a later speech mode (our own frames; no word bank taken) |
| FOF: a damped-sine grain launched per period, skirt from a shaped rise, decay from bandwidth, drawn from an overlap pool | Csound `Opcodes/ugens7.c` | LGPL-2.1+ | core/filter (an alternative formant voice) |
| FOF as a filter: several sample-and-hold FOFs taken in turn so a moving vowel does not zipper | Faust `physmodels.lib` (fofCycle) | LGPL-2.1+ with exception | core/filter (smoothing) |
| autobend and vocal effort: F1 and F2 pulled toward a high f0; upper formant gains raised at high pitch and effort | Faust `physmodels.lib` (after chant-lib) | LGPL-2.1+ with exception | core/filter; vowel Effort row |
| PADsynth ensemble: every harmonic spread into a Gaussian band that widens with harmonic number, random phases, one inverse FFT, played as a seamless loop | PADsynth algorithm page (public domain per its author); Zyn `PADnote` | public domain; GPL-2.0+ | choir (a back-section layer over `CORE::OSCILLATOR` tables) |
| unison: sub-voices with seeded phases, their own vibrato, stereo spread and phase inversion | Zyn `ADnote`; Vital `synth_oscillator.h` | GPL-2.0+; GPL-3.0 | choir (has it); vowel over `CORE::VOICE` unison |
| WORLD: pulses placed from a fractional F0 phase, per pulse an interpolated envelope made minimum-phase via the cepstrum, aperiodic noise mixed by band, overlap-add | WORLD `src/synthesis.cpp` | BSD-3-Clause (modified) | later; seam-gated (a recording must reach a plugin) |
| self-oscillating triangular glottis and a time-domain branched tube solved per sample | VocalTractLab `TriangularGlottis.cpp`, `TdsModel.cpp` | GPL-3.0+ | none now (high cost); idea only |

No line, table, constant or coefficient is taken from any of these; each row is the idea in this survey's words.

## Sources

| Name | Licence | Home | Read |
|---|---|---|---|
| Pink Trombone | MIT (per the port's LICENSE.md; the original page shows none) | https://dood.al/pinktrombone/ | the page (title only); chdh/pink-trombone-mod `src/Glottis.ts`, `src/Tract.ts`, `LICENSE.md` |
| Voc (P. Batchelor) | MIT (CWEB source); generated C public domain | https://github.com/PaulBatchelor/Voc | README, `tract.w` (not a column) |
| VocalTractLab | GPL-3.0+ | https://www.vocaltractlab.de | about page; TUD-STKS VocalTractLab-dev; backend `src/` listing, `TdsModel.cpp`, `TriangularGlottis.cpp`, `VocalTractLabApi.cpp` |
| Faust libraries (physmodels vocal) | LGPL-2.1+ with the GRAME exception | https://faustlibraries.grame.fr/libs/physmodels/ | docs page; `physmodels.lib` |
| eSpeak NG | GPL-3.0+ | https://github.com/espeak-ng/espeak-ng | `src/libespeak-ng/klatt.c`, `wavegen.c` |
| klatt-syn | MIT | https://github.com/chdh/klatt-syn | README; `src/Klatt.ts` |
| rsynth / parwave | unclear ("believed public domain"; Debian files it non-free): refused | — | README via klatt-syn's lineage (not a column) |
| SAM ports | none (abandonware; SoftVoice holds copyright): refused | https://github.com/s-macke/SAM | both READMEs (not a column) |
| Csound fof / fof2 / fog | LGPL-2.1+ | https://csound.com | `Opcodes/ugens7.c`, `Opcodes/ugensa.c`; fof manual page; formant appendix `MiscFormants.html` (no licence shown) (not a column) |
| SuperCollider Formant, Formlet, VOSIM | GPL-2.0+ (headers); project GPL-3.0 | https://supercollider.github.io | help pages; `server/plugins/OscUGens.cpp`; sc3-plugins `VOSIM.cpp` (not a column) |
| Mutable Instruments Plaits and Braids | MIT | https://github.com/pichenettes/eurorack | Plaits manual; `plaits/dsp/engine/speech_engine.cc`, `plaits/dsp/speech/{naive,sam,lpc}_speech_synth.cc`, `lpc_speech_synth_controller.cc`; `braids/digital_oscillator.cc` |
| ZynAddSubFX / Yoshimi | GPL-2.0+ | https://zynaddsubfx.sourceforge.io | `src/DSP/FormantFilter.cpp`; PADsynth algorithm page; Yoshimi AddSynth docs; feature lists |
| Surge XT | GPL-3.0+ (sst-filters GPL-3.0) | https://surge-synthesizer.github.io | manual-xt; `TwistOscillator.cpp`; sst-filters README (not a column: Twist wraps Plaits) |
| Vital | GPL-3.0 | https://github.com/mtytel/vital | `src/synthesis/filters/formant_filter.cpp`, `vocal_tract.cpp` (a pass-through stub), `synth_oscillator.h`; reviews |
| Odin 2 | GPL-3.0+ | https://github.com/TheWaveWarden/odin2 | `Source/audio/Filters/FormantFilter.cpp`; reviews (not a column) |
| LMMS | GPL-2.0+ | https://github.com/LMMS/lmms | `include/BasicFilters.h` (formant filters) (not a column) |
| WORLD | BSD-3-Clause (modified) | https://github.com/mmorise/World | README; `src/synthesis.cpp` |
| OpenUtau | MIT | https://github.com/stakira/OpenUtau | README; wiki "Expressions (Flags)" |
| Sinsy | BSD-3-Clause (modified) | https://sinsy.sourceforge.net | home page (not a column) |
| NNSVS | MIT | https://github.com/nnsvs/nnsvs | README (not a column) |
| Dreamtonics Synthesizer V Studio 2 Pro | commercial | https://dreamtonics.com/synthesizerv/ | Sound On Sound review; Avid and Plugin Boutique feature pages (the product page refused the fetch) |
| Yamaha Vocaloid 6 | commercial | https://www.vocaloid.com/en/vocaloid6/ | product page; learn pages ln6212, ln6214; FAQ |
| ACE Studio 2 | commercial | https://acestudio.ai | search excerpts of SOS and MusicTech (not a column; its AI choir noted under choir) |
| Plogue Chipspeech | commercial | https://www.plogue.com/products/chipspeech.html | product page; Wikipedia; KVR; Synthtopia (the manual link was dead) |
| Plogue Alter/Ego | freeware, closed | https://www.plogue.com/products/alter-ego.html | product page; Bedroom Producers Blog |
| EastWest Hollywood Choirs | commercial | https://www.soundsonline.com/hollywood-choirs | product page; Epic Composer, MusicRadar and ask.audio reviews |
| Spectrasonics Omnisphere 3 | commercial | https://www.spectrasonics.net/products/omnisphere/ | SOS review; Omnisphere 2 filter manual page; the sampled survey's reads |
| Output Exhale | commercial (Kontakt) | https://output.com/products/exhale | SOS review; Production Expert (the product page loaded empty) (not a column: a vocal sample player) |
| Delay Lama (AudioNerdz) | freeware, closed | https://www.kvraudio.com/product/delay_lama_by_audionerdz | KVR; Synth Anatomy on its 2026 remake |
| Emvoice One | commercial (cloud) | gearnews.com/?p=54380 | gearnews; KVR (not a column: cloud-rendered typed singer) |
| Xfer Serum 2 formant filters | commercial | https://xferrecords.com | a third-party filter guide (not a column) |
| Kilohearts Formant Filter | commercial | https://kilohearts.com/products/formant_filter | search excerpt (not a column; its 2-D vowel pad) |

## Moved

none
