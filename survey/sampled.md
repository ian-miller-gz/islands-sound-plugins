# sampled — round 1

## Matrix

A cell is `yes` / `no` / `partial` and one word; `?` is unverified. The CPU row reads `low` / `mid` / `high` and how it is known (`est` = estimated from the code's per-sample work, `stated` = the maker's or the project's own words). Ours is read from this worktree (`instrument/sampler/`). Open: sfizz, Shortcircuit XT (SCXT), FluidSynth, samplv1, LinuxSampler, DrumGizmo, LMMS AudioFileProcessor (AFP). Freeware: Decent Sampler. Popular: Kontakt 8, Omnisphere 3, Pianoteq 9, Ableton Sampler (Live 12; Simpler noted in its cells), Logic Sampler (Quick Sampler noted), HALion 7, Falcon 3, TAL-Sampler. Pianoteq is modelled, not sampled: it is the sampled piano's rival on realism at a sliver of the size, so the commission lists it here. The rows after the commission's list are the family's own: sources and loading, disk streaming, velocity layers, round robin, crossfades, loop modes, loop crossfade, release triggers, interpolation, start and end, reverse, stretch and slicing, granular, envelopes, choke, polyphony and stealing, stereo and pan, programs, scripting.

| Capability | sampler | sfizz | SCXT | FluidSynth | samplv1 | LinuxSampler | DrumGizmo | LMMS AFP | Decent Sampler | Kontakt 8 | Omnisphere 3 | Pianoteq 9 | Ableton Sampler | Logic Sampler | HALion 7 | Falcon 3 | TAL-Sampler |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| oscillator kinds | partial one-clip | partial samples+generators | partial samples | partial samples | partial one-sample | partial samples | partial samples | partial one-sample | partial samples | yes samples+WT+FM | yes samples+638-waves | yes modelled | partial samples+modosc | partial samples | yes VA+WT+FM+spectral | yes ~20-oscillators | partial samples |
| unison and detune | no | partial osc-multi | partial unison-variant | no | no | no | no | no | ? | ? | yes unison | no | no | ? | ? | ? | ? |
| wavetable morphing | no | partial sample-as-table | no | no | no | no | no | no | no | yes WT-mode | yes wavetable | no | no | no | yes 1024-waves | yes 500+-tables | no |
| FM algorithms and operators | no | partial osc-FM | ? | no | no | no | no | no | no | partial FM-osc | yes FM+ring | no | partial FM/AM-osc | no | yes 8-op | yes 4-op | no |
| filter models | no none | yes 2+3EQ | yes dozens | partial LP-biquad | yes 4-types | yes 5-types | no | no host | yes ? | yes many | yes 69 | partial EQ | yes morph+circuits | yes 2-filters | yes morphing | yes +ladder | yes ZDF+Butterworth |
| modulation sources and the matrix | no fixed-AR | yes matrix | yes 18-rows | yes SF2-modulators | partial fixed-routes | partial NKSP-scripts | no | no | yes bindings | partial per-module | yes 48-slots | partial vibrato | partial aux-env+3LFO | yes 20-routings | yes matrix | yes 13-generators | yes 15-slots |
| voice modes — poly, mono, legato, glide | partial poly-only | partial legato-trigger | yes poly/mono/legato+glide | yes legato+porta | yes mono+legato | partial ? | no | no | yes mono+glide | yes ? | yes ? | no | yes glide+porta | yes poly/mono/legato | ? | ? | ? |
| MPE and aftertouch | no seam | partial bend | yes MPE+polyAT | partial AT+bend | partial AT+bend | partial bend | no | no host | partial ? | partial polyAT | yes MPE | partial ? | yes MPE | ? | yes MPE | yes MPE | yes MPE |
| microtuning | no 12TET | yes Scala+stretch | yes Scala?+MTS | yes MTS | yes Scala | partial GS-sysex | no | partial host | partial ? | partial script | yes .tun | yes Scala+MTS | yes Live-tunings | ? | yes Lua/Scala | yes scl/kbm/tun | yes tun+MTS |
| macro controls | no | partial CC | yes 16 | partial NRPN | yes MIDI-learn | yes CC | ? | no | yes UI | yes UI | yes ? | no | partial Racks | partial Smart-Controls | yes macro-page | yes ? | ? |
| effects in the instrument | no | yes 11 | yes 10-bus | yes reverb+chorus | yes 7 | partial sends | no | no | yes many | yes many | yes 93 | yes 9 | no | no | yes 70+ | yes 100+ | yes 5 |
| oversampling and aliasing | no linear-unfiltered | partial sinc-72 | yes auto-2x | no fixed-kernel | partial octave-tables | no | no | partial libsamplerate | ? | yes HQI | ? | no modelled | partial Best | ? | ? | ? | partial reconstruction |
| sample playback and zones | partial one-clip-whole-keyboard | yes SFZ | yes zones+groups | yes SF2-zones | partial key-range | yes dimensions | partial velocity-power | partial one-sample | yes groups | yes groups+zones | yes import | no modelled | yes key/vel/select | yes zones+groups | yes layers | yes keygroups | yes 4-layers |
| drum synthesis models and layering | no | partial choke+RR | partial exclusive+RR | partial exclusive-class | no | partial key-groups | yes multi-mic | no | partial tags | partial voice-groups | ? | no | partial Drum-Rack | ? | ? | ? | partial mute-groups |
| vocal formant and glottal models | no | no | no | no | no | no | no | no | no | partial Tone-Machine | ? | no | no | no | ? | partial VOSIM | no |
| sources and loading | partial session-clips-at-create | yes SFZ+WAV/FLAC | yes SF2/SFZ/GIG/EXS-import | yes SF2/SF3 | partial one-file | yes GIG/SFZ/SF2 | yes multichannel-kits | partial one-file | yes dspreset | yes NKI+REX | yes WAV/AIFF-import | no modelled | yes REX+multisample | yes EXS/SF2/DLS/GIG | yes ? | ? | yes SFZ/SF2-import |
| disk streaming | no RAM-copy | yes preloaded-heads | ? RAM | no RAM | no RAM | yes ring-buffers | yes chunk-cache | no RAM | yes stream-mode | yes DFD | ? | no modelled | ? | ? | yes streaming | ? | ? |
| velocity layers | no | yes | yes | yes | no | yes | yes power-matched | no | yes | yes | ? | no continuous | yes | yes | yes | ? | yes |
| round robin and random | no | yes seq+random | yes 16-variants | no | no | yes | yes no-repeat | no | yes | yes | ? | no | yes 4-modes | yes cycles | ? | ? | ? |
| zone and velocity crossfades | no | yes xfin/xfout | yes key+vel | no | no | yes | no | no | ? | yes | ? | no | yes constant-power | yes key+vel | ? | ? | ? |
| loop modes | partial wrap-to-start | yes sustain+continuous | yes counted+pingpong | yes until-release | yes | yes | no | yes pingpong | yes | yes | ? | no | yes sustain+release | yes fwd/rev/alt | ? | ? | yes fwd+pingpong |
| loop crossfade | no | yes S-curve | yes curve | no | yes second-head | ? | no | no | yes | ? | ? | no | yes constant-power | yes | ? | ? | yes |
| release triggers | no | yes | yes | partial on-release | no | yes held-scaled | no | no | yes | yes | ? | partial modelled-dampers | partial release-loop | yes | ? | ? | ? |
| interpolation | partial linear | yes 11-tiers | yes sinc-16 | yes sinc-25 | partial cubic | partial cubic | partial resampler | yes libsamplerate | ? | yes HQI-3 | ? | no modelled | yes 3-tiers | ? | ? | ? | yes 6-DAC-modes |
| start and end points | partial start-only | yes offset+mod | yes start-mod | yes generators | yes offset | ? | no | yes start/end | ? | yes | ? | no | yes | yes | ? | ? | yes modulatable |
| reverse | no | yes | yes | no | yes | ? | no | yes | ? | yes | ? | no | yes | partial loop-only | ? | ? | yes |
| time-stretch, pitch-shift, slicing | no | no | partial slice-fields | no | partial pitch-shift | no | no | partial stutter | yes tempo-slices | yes Time-Machine | ? | no | partial Simpler-slice | partial Quick-slice | yes spectral-warp | yes IRCAM-stretch | yes vintage-stretch |
| granular | no | no | no | no | no | no | no | no | ? | partial Tone-Machine | yes | no | no | no | yes | yes | yes 24-grains |
| envelopes | partial linear-AR | yes DAHDSR+multi | yes 5 | yes 2 | yes 3 | yes 3 | no | no host | yes curves | yes | yes 12 | partial attack | yes 4 | yes 5 | yes | yes | yes 3-AHDSR |
| choke and exclusive groups | no | yes off_by | yes exclusive | yes exclusive-class | no | yes key-groups | yes hihat-groups | no | yes tags | yes voice-groups | ? | no | no | ? | ? | ? | yes mute-groups |
| polyphony and stealing | partial 8-quietest | yes 3-policies | yes 512 | yes score-256 | partial 64-drop | yes oldest | yes per-instrument | ? | ? | yes ? | ? | ? | yes 32 | ? | ? | ? | ? |
| stereo and pan | partial clip-lanes | yes pan+width | yes equal-power | yes pan | ? | ? | yes multichannel | ? | ? | yes | ? | yes mic-positions | yes | yes | ? | ? | ? |
| programs | partial clip-by-program | yes .sfz | yes ? | yes SF2-banks | no | yes .gig | yes kits | no | yes presets | yes 900+ | yes library | yes editions | yes packs | yes library | yes library | yes library | yes presets |
| scripting | no | no | no | no | no | yes NKSP | no | no | no | yes KSP | no | no | partial Max | partial Scripter | yes Lua | yes Lua | no |
| CPU per voice | low est | ? quality-tiers | ? 2x-when-pitched | low est | low stated | low stated | partial per-channel | low est | ? | partial HQI-costs | ? | low stated | partial Best-costs | ? | ? | ? | ? |

## Lacking

Answering round 0's word: stand beside the open and the popular instruments, family by family; learn the methods; what many need, the library gives once. The family is one instrument, so a core need below is one the other families' surveys state too.

### Shared (a `core/` matter)

- band-limited reading at a fractional place: a 4-point cubic read by default; a windowed-sinc read whose kernel stretches, and cutoff drops, when the step passes one; or a 2x path entered only when the step passes a threshold (sfizz, SCXT, FluidSynth, Kontakt HQI, Ableton Best). Today the clip is read linearly and nothing is filtered when it is pitched up, so every note above Root aliases. The spectral survey asks for the same cubic read for `CORE::OSCILLATOR` tables, and `CORE::LINE`'s reads are the third user. Whether that is a new `core/` family or a `read` in `core/oscillator` is the design stage's call.
- microtuning: a per-key table behind `CORE::PHASE::Tuning`, from Scala or MTS (sfizz, SCXT, samplv1, Pianoteq, Falcon, TAL-Sampler). Every family's survey asks for it.
- MPE, aftertouch and bend: the seam comes first. There is no pressure event, and sound sends no `BEND` to a plugin (379 "The seam"). That is a finding for an engine order.
- a live clip list: the sampler copies every audio clip of the inventory once, at `create`. A clip recorded later is unseen until the plugin is seated again, and every instance holds its own copy of all the session's audio. The `material` DATA ports it declares are "carried by nothing yet" (the seam's own doc), so a refreshed or shared clip is a host matter. That is a finding for an engine order, not a lane.

### sampler

- the sheet over `CORE::TABLE`, the event cursor and meter over `CORE::BLOCK`, the allocator over `CORE::VOICE`, the envelope over `CORE::ENVELOPE` and the cents over `CORE::PHASE::ratio` (379 law 4). Today it keeps its own `Row` table, its own denormal flush and ping-pong meter, its own quietest-steal seat and a hand-built table of cents. This comes before any gain below, as the subtractive and drums surveys say of their founding exemplars.
- zones: several clips over the keyboard, each with a root and a key range, so a multisample plays each note from its nearest clip (every column but samplv1, AFP and DrumGizmo). Rows are a fixed sheet, so this means a fixed count of zone rows (for example eight zones × Stock, Root, Low, High).
- velocity layers with a crossfade between them, equal-power (sfizz, SCXT, Kontakt, Ableton Sampler, Logic Sampler).
- round robin and random choice among a zone's clips, avoiding a repeat (sfizz, SCXT, DrumGizmo, Ableton Sampler, Logic Sampler, Kontakt).
- loop points and modes: loop start and end rows; Off / Forward / Ping-pong / Sustain (loop while held, then play out); a loop crossfade row. Today Loop wraps from the end back to Start, so the loop is the whole clip and clicks at the seam (sfizz, SCXT, samplv1, Ableton Sampler, Logic Sampler, TAL-Sampler).
- an End row and Reverse (sfizz, SCXT, samplv1, AFP, Ableton Sampler, TAL-Sampler).
- an ADSR with exponential curves over `CORE::ENVELOPE`, plus a second envelope for pitch or cutoff, in place of the linear attack and release (every column with envelopes).
- a filter over `CORE::FILTER` (multimode variable-state and ladder) with its own envelope, key tracking and velocity to cutoff (every popular column, sfizz, SCXT, samplv1).
- an LFO over `CORE::MODULATOR` to pitch, cutoff and level (every popular column, sfizz, SCXT, samplv1).
- voice modes: Poly / Mono / Legato with Glide and a Voices row, all over `CORE::VOICE` (SCXT, FluidSynth, samplv1, Logic Sampler, Ableton Sampler).
- choke: a same-note retrigger cuts its old voice, and a choke-group row serves hat-style kits (sfizz, SCXT, FluidSynth, DrumGizmo, TAL-Sampler).
- pitch rows: Coarse ±24 st beside Tune, a Track switch (Off plays every key at the clip's own pitch, for one-shots), and Bend once the seam carries it. Today only Root and a ±50 ct Tune move pitch.
- velocity: a sensitivity row and a curve instead of a level linear in velocity (sfizz curves, FluidSynth modulators).
- Pan and Width rows; a mono clip is copied to every lane today.
- start modulation: start pushed by velocity or a random amount (sfizz, SCXT).
- release trigger: a clip played on lift, scaled by how long the note was held (sfizz, SCXT, LinuxSampler, Kontakt, Logic Sampler).
- slicing: a Slice mode cutting the clip at equal divisions or at detected onsets, one slice per key (Simpler, Quick Sampler, Decent Sampler). Onsets could ride `CORE::DYNAMICS`'s detector.
- a granular mode: grains over the clip, after `CORE::CLOUD`, which today reads only a `CORE::LINE` ring (Omnisphere, HALion, Falcon, TAL-Sampler).
- effects in the instrument: a vintage converter colour (rate and bit reduction with its reconstruction filter, over `CORE::SHAPER::Crusher`), then chorus and reverb over `CORE::LINE` and `CORE::REVERB` (TAL-Sampler, Kontakt's vintage modes, sfizz, Decent Sampler).
- programs: today `PROGRAM` picks a clip; a program should set the whole sheet (sfizz, FluidSynth, Kontakt, Decent Sampler).
- polyphony: 8 → 16 or 32 voices once the voice is costlier (Ableton Sampler 32, FluidSynth 256, SCXT 512).

## Methods

| Method (own words) | Source and file | Licence | Lands |
|---|---|---|---|
| windowed-sinc kernel built once per point count; the fractional place picks the per-tap weights; one quality number per region picks nearest, linear, Hermite or a sinc length, with a heavier choice for offline rendering | sfizz `src/sfizz/Interpolators.hpp`, `Voice.cpp`, `Synth.cpp` | BSD-2-Clause | core (the read; sampler's quality row) |
| band-limited interpolation: one wing of a windowed sinc stored finely per zero crossing, the two sides of the read point summed separately; when the step passes one, the walk through the table slows so the kernel stretches and its cutoff drops | J. O. Smith, *Digital Audio Resampling* (Implementation page) | text (no code read) | core (the read) |
| polyphase sinc: either a direct table per output phase or a finer table read cubically, whichever is smaller; quality levels trade length and window; the cutoff scales down and the filter lengthens when the rate drops | Speex `libspeexdsp/resample.c` | BSD-3-Clause | core (the read) |
| per-voice 2x only when the playback step passes a threshold, then a half-band decimation; never for a held-sample read | Shortcircuit XT `src/scxt-core/voice/voice.cpp` | MIT (source; AGPL-3.0+ binaries) | sampler (rides `CORE::SHAPER::Oversampler`) |
| a 4-point cubic read through one point before and two after, with guard points past the ends | Pure Data `src/d_array.c`; Puckette, *Theory and Techniques* ch. 2 | BSD-3-Clause | core (the read) |
| guard points past a loop's end filled from its start, so the interpolator never reads outside; a has-looped flag picks what lies before the start | FluidSynth `src/rvoice/fluid_rvoice_dsp.cpp` | LGPL-2.1+ | sampler (loop) |
| loop crossfade: near the loop end, the tail fades out while the audio before the loop start fades in, on a chosen curve | sfizz `src/sfizz/Voice.cpp` | BSD-2-Clause | sampler (loop crossfade row) |
| loop crossfade with a second read head one loop length behind, faded linearly; loop points snapped to zero crossings | samplv1 `src/samplv1_sample.cpp` | GPL-2.0+ | sampler (loop) |
| ping-pong crossfades centred on each turnaround; loop, direction and channel flags as a small index into specialised generators | Shortcircuit XT `src/scxt-core/dsp/generator.cpp` | MIT (source) | sampler (loop, reverse) |
| loop held only while the amplitude envelope is before release, then play out to the end | FluidSynth `src/rvoice/fluid_rvoice.c` | LGPL-2.1+ | sampler (Sustain loop mode) |
| stealing by score: released, sustained, age and level each weigh in; the lowest score is released quickly rather than cut | FluidSynth `src/synth/fluid_voice.c`, `fluid_synth.c` | LGPL-2.1+ | core/voice (a scored steal) |
| envelope-and-age stealing: skip voices younger than a fraction of the oldest; take the oldest whose group power is under a fraction of the mean | sfizz `src/sfizz/VoiceStealing.cpp` | BSD-2-Clause | core/voice |
| exclusive class: a new voice gives same-class voices from another note a very fast release after a minimum length | FluidSynth `fluid_synth.c`, `fluid_voice.c` | LGPL-2.1+ | core/voice (choke; drums kit too) |
| within one note, later groups choke earlier ones; legato voices parked and re-attacked rather than freed | Shortcircuit XT `src/scxt-core/engine/engine_voice_responder.cpp` | MIT (source) | core/voice |
| round robin by a per-layer counter wrapped by the sequence length | sfizz `src/sfizz/Layer.cpp` | BSD-2-Clause | sampler (zones) |
| sample choice by a cost: distance from the target loudness, how recently it played, and a random term; the search walks out from the target and stops when distance alone loses | DrumGizmo `src/sample_selection.cc` | LGPL-3.0+ | sampler (random choice); drums kit |
| alternating sources stepped once per note, so layered zones stay in step | Shortcircuit XT `src/scxt-core/modulation/voice_matrix.h` | MIT (source) | sampler (round robin across layers) |
| modulator model: each target is its base value plus the sum of sources × secondary sources × amounts, each through a curve; a controller change recomputes only its targets | FluidSynth `src/synth/fluid_voice.c` | LGPL-2.1+ | sampler (a small fixed matrix) |
| curves: a few user points filled into a fixed table at load, so playback is one lookup | sfizz `src/sfizz/Curve.cpp` | BSD-2-Clause | sampler (velocity curve) |
| early end: the clip's loop peak scanned once; the voice ends when envelope × peak sinks under the noise floor | FluidSynth `src/synth/fluid_voice.c` | LGPL-2.1+ | sampler (CPU) |
| resonant filter with a cutoff ramp lengthened at high Q, gain set back by the root of Q, cutoff clamped under Nyquist | FluidSynth `src/rvoice/fluid_iir_filter_impl.cpp` | LGPL-2.1+ | core/filter (ramped settle) |
| release clip level lowered in proportion to how long the note was held | LinuxSampler `src/engines/common/AbstractVoice.cpp` | GPL-2.0+ with a no-commercial-use exception | sampler (release trigger) — idea only |
| pre-pitched octave tables built at load, the nearest chosen from the note and read cubically | samplv1 `src/samplv1_sample.cpp` | GPL-2.0+ | sampler (an alternative to the sinc read) |
| disk streaming: each file's head kept in memory, a worker fills the rest ahead of the read; a refill thread serves the emptiest stream first | sfizz `src/sfizz/FilePool.cpp`; DrumGizmo `src/audiocache.h`; LinuxSampler `DiskThreadBase.h` | BSD-2-Clause; LGPL-3.0+; GPL+exception | host (after the seam carries files) |
| converter emulation: selectable reconstruction modes (sample-and-hold, linear, clean, vintage converters) and a variable rate | TAL-Sampler (product page) | commercial (feature only) | sampler (a colour row over `CORE::SHAPER::Crusher`) |
| spectral time-stretch and pitch-shift: blocks remapped from input to output frequency, length by differing buffer sizes, formant shifted apart | Signalsmith Stretch (README) | MIT | sampler (a later stretch mode) |

No line, table, constant or coefficient is taken from any of these; each row is the idea in this survey's words.

## Sources

| Name | Licence | Home | Read |
|---|---|---|---|
| sfizz | BSD-2-Clause (archived 2026-06-21) | https://github.com/sfztools/sfizz | README; `src/sfizz` tree; `Interpolators.hpp`, `FilePool.{h,cpp}`, `PolyphonyGroup.h`, `Oversampler.h`, `Synth.cpp`, `VoiceStealing.cpp`, `Voice.cpp`, `Config.h`, `Layer.cpp`, `Curve.cpp`, `Tuning.h`, `modulations/ModId.cpp`; sfz.tools opcode status page |
| Shortcircuit XT | MIT source outside `libs/` since 2026-01-18; combined binaries AGPL-3.0+ | https://github.com/surge-synthesizer/shortcircuit-xt | README; LICENSE; `doc/VoiceRouting.md`; `voice/voice.cpp`, `engine/zone.h`, `engine/group.h`, `engine/engine_voice_responder.cpp`, `engine/bus_effect.h`, `dsp/resampling.h`, `dsp/generator.cpp`, `dsp/processor/processor.h`, `modulation/voice_matrix.h`, `sample/sample.h` |
| FluidSynth | LGPL-2.1+ | https://github.com/FluidSynth/fluidsynth | LICENSE; README; wiki; settings API page; `fluid_rvoice_dsp.cpp`, `fluid_rvoice_dsp_sinc.hpp`, `fluid_rvoice.c`, `fluid_iir_filter*.c/cpp`, `fluid_voice.c`, `fluid_synth.c`; tag v2.4.0 `fluid_rvoice_dsp.c`, `gentables/gen_rvoice_dsp.c` |
| samplv1 | GPL-2.0+ (pitch-shifter part under the Wide Open License) | https://github.com/rncbc/samplv1 | repo page; LICENSE; ChangeLog; `samplv1.cpp`, `samplv1_sample.{h,cpp}`, `samplv1_pshifter.cpp` |
| LinuxSampler | GPL-2.0+ with an exception barring commercial use (not OSI-open; read for ideas only) | https://www.linuxsampler.org | features page; NKSP docs; svn trunk README, `DiskThreadBase.h`, `Resampler.h`, `AbstractVoice.cpp`, `EngineBase.h` (part) |
| DrumGizmo | LGPL-3.0+ | https://drumgizmo.org | wiki, features and file-format pages; man page; `src/sample_selection.cc`, `src/audiocache.h` (Debian's 0.9.20 copy; the upstream git host failed TLS) |
| LMMS (AudioFileProcessor, Sf2 Player) | GPL-2.0+ | https://github.com/LMMS/lmms | LICENSE.txt; `plugins/AudioFileProcessor/AudioFileProcessor.cpp`, `plugins/Sf2Player/Sf2Player.cpp`, `src/core/Sample.cpp` |
| Decent Sampler | freeware, closed | https://www.decentsamples.com/product/decent-sampler-plugin/ | product page; developer guide (index, groups, modulators) |
| Speex DSP resampler | BSD-3-Clause | https://github.com/xiph/speexdsp | `libspeexdsp/resample.c` (header and structure; not a column) |
| Digital Audio Resampling (J. O. Smith, CCRMA) | text; the page lists LGPL, BSD and other free packages | https://ccrma.stanford.edu/~jos/resample/ | home, Implementation and Free Resampling Software pages (not a column) |
| libsamplerate | BSD-2-Clause | https://github.com/libsndfile/libsamplerate | repo page; FAQ (not a column; AFP's converter) |
| Signalsmith Stretch | MIT | https://github.com/Signalsmith-Audio/signalsmith-stretch | README (not a column) |
| Pure Data vanilla | BSD-3-Clause | https://github.com/pure-data/pure-data | `tabread4~` docs and course notes as search excerpts; `d_array.c` as cited by the spectral survey (not a column) |
| Niemitalo, *Polynomial Interpolators for High-Quality Resampling of Oversampled Audio* | paper | yehar.com (deip.pdf) | search excerpts only; FluidSynth's cubic is drawn after it (not a column) |
| Native Instruments Kontakt 8 | commercial | https://www.native-instruments.com/en/products/komplete/samplers/kontakt-8/ | product page; manual (source module, options) as search excerpts; KSP version history; TapeOp review |
| Spectrasonics Omnisphere 3 | commercial | https://www.spectrasonics.net/products/omnisphere/ | product page and specs; manual page on scales |
| Modartt Pianoteq 9 | commercial | https://www.modartt.com/pianoteq_overview | overview and features pages; forum threads on tuning |
| Ableton Sampler and Simpler | commercial | https://www.ableton.com/en/manual/live-instrument-reference/ | Live 12 manual; MPE FAQ; Live 12 release notes |
| Apple Logic Sampler and Quick Sampler | commercial | https://support.apple.com/guide/logicpro/ | Logic Pro guide pages on Sampler and Quick Sampler |
| Steinberg HALion 7 | commercial | https://www.steinberg.net/halion/ | Sound On Sound review; HALion Script developer docs; retailer pages (the product page did not read) |
| UVI Falcon 3 | commercial | https://www.uvi.net/falcon | MusicTech review; UVI support articles; retailer listings (the product page did not read) |
| TAL-Sampler | commercial | https://tal-software.com/products/tal-sampler | product page and changelog |
| Native Instruments Battery 4 | commercial | https://www.native-instruments.com/en/products/komplete/drums/battery-4/ | product page (the drums family's; not a column) |
| Spitfire LABS | freeware | https://labs.spitfireaudio.com | redirects to Splice INSTRUMENT (not a column: no import, no edit) |

## Moved

none
