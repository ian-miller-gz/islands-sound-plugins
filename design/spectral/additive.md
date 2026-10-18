# additive — round 1

Gains its re-founding on the cores (first, before any gain), 32 partials with a per-note Nyquist fade, spectral shaping rows, a spectral filter swept by its own contour, pitch rows, one LFO, velocity to brightness — matrix lacks 2, 70, 71, 73, 72 (in part: an envelope on the spectrum), 17, 11 (in part: one LFO), 4, 37, 14. Later rounds: unison and a stereo out (26, 5: after core/stereo stands), the PADsynth bandwidth table (41), a second spectrum and a morph (72's rest), a filter after the sum (16), effects (10), programs (3), macros (8), microtuning (7).

## Rows

The thirteen standing rows keep their indices, units, ranges and rests; the eight harmonic rows lose the run-time "Harmonic N" label for the sheet's literal `HarmonicN`. New rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Gain | | 0 | 1 | 0.30 | 0 | | |
| 1 | Attack | s | 0.001 | 4.001 | 0.011 | 0 | | |
| 2 | Decay | s | 0.001 | 4.001 | 0.1946 | 0 | | |
| 3 | Sustain | | 0 | 1 | 0.70 | 0 | | |
| 4 | Release | s | 0.001 | 4.001 | 0.251 | 0 | | |
| 5–12 | Harmonic1 … Harmonic8 | | 0 | 1 | 1/n: 1, 0.5, 0.3333, 0.25, 0.2, 0.1667, 0.1429, 0.125 | 0 | | renamed |
| 13 | Partials | | 1 | 32 | 32 | 31 | | new |
| 14 | Tilt | dB | -12 | 12 | 0 | 0 | | new |
| 15 | Balance | | -1 | 1 | 0 | 0 | | new |
| 16 | Stretch | ct | -100 | 100 | 0 | 0 | | new |
| 17 | Centre | | 1 | 32 | 6 | 0 | | new |
| 18 | Breadth | | 0.5 | 16 | 3 | 0 | | new |
| 19 | Focus | | 0 | 1 | 0.5 | 0 | | new |
| 20 | Cutoff | Hz | 20 | 20000 | 2500 | 0 | | new |
| 21 | Resonance | | 0 | 1 | 0.3 | 0 | | new |
| 22 | Keyboard | | 0 | 1 | 0.5 | 0 | | new |
| 23 | Envelope | oct | 0 | 5 | 2 | 0 | | new |
| 24 | Sweep | s | 0.001 | 4.001 | 0.011 | 0 | | new |
| 25 | Close | s | 0.001 | 4.001 | 0.6 | 0 | | new |
| 26 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 27 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 28 | Rate | Hz | 0.05 | 20 | 0.5 | 0 | | new |
| 29 | Wave | | 0 | 5 | 0 | 5 | Sine, Triangle, Saw, Square, Sample, Random | new |
| 30 | Vibrato | st | 0 | 2 | 0 | 0 | | new |
| 31 | Scan | | 0 | 16 | 2 | 0 | | new |
| 32 | Tune | ct | -200 | 200 | 0 | 0 | | new |
| 33 | Bend | st | -12 | 12 | 0 | 0 | | new |
| 34 | Glide | s | 0 | 3 | 0 | 0 | | new |

Chain: the harmonic additive voice (a bank of sine partials at integer multiples, a spectral envelope over them, an amplitude ADSR) as Plaits' additive model and the Harmor/Razor partial filter publish it; every new range is ours. Per partial n (1 … Partials): series(n) = Harmonic n for n ≤ 8, else Harmonic8 × 8/n (a saw's 1/n at rest); tilt n^(Tilt/6.02) (dB per octave of n); Balance −1 silences the evens, +1 the odds above the fundamental, linear between; frequency n × f0 raised by Stretch × log2(n) cents; bump (1 − Focus) + Focus × exp(−((n − Centre)/Breadth)²); filter 1/√(1 + r⁸) × (1 + 4 × Resonance × exp(−(log2 r / ⅓)²)), r = fₙ / cut, cut = Cutoff × 2^(Keyboard × (key − 60)/12 + Envelope × contour + Touch × (velocity − 1)); Nyquist fade: full below 0.40 × rate, silent above 0.45 × rate, linear between. The sum is divided by Σ series × tilt × balance × bump (filter and fade darken, never renormalise). The contour is an ADSR of attack Sweep, decay and release Close, sustain 0. Scan swings Centre by ± Scan partials with the LFO. At rest: a 32-partial saw with a soft peak at the sixth partial, bright on the strike and closing toward 2.5 kHz within about half a second.

## Chain

| Stage | Rides |
|---|---|
| the sheet: `additive.indices.hpp` (indices), `additive.hpp` (`ROWS`, `SHEET`, `WAVES`); `params.cpp`, `voice.params.cpp`, the hand-written `SURFACE` and all three `switch`es leave | `core/table` |
| allocation: eight voices, oldest stolen; Glide (slide `ALWAYS`) | `core/voice` |
| loudness ADSR (linear, reset on strike — today's shape), depth Velocity; the contour ADSR (exponential, reset) | `core/envelope` (`weigh` stands) |
| one LFO of Wave at Rate: Vibrato semitones on pitch, Scan partials on Centre | `core/modulator` `Lfo` |
| per voice, once per stride (16 frames): pitch = glided key + Tune/100 + Bend + Vibrato × lfo; per partial its step and its gain (the law above), in `voice.spectrum.cpp` | `core/phase` (`hertz`, `step`); NEEDS core/block — `Stride` (see design/subtractive/acid.md) |
| per sample: each partial's phase read from one sine table, linearly interpolated, its gain ramped from the last stride's value across the stride; × loudness | `core/oscillator` `Table` (`build(table)`, `sine` stand; the 4096-point truncating table and `steps` vector leave) |
| Gain; one sample to every lane; flush, cursor, meter | `core/block` |

No core beyond `Stride` (already named) is needed: the partial bank has one rider and stays in the instrument.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; every sample written, one sample to every lane (mono, as today).
- voicing: `poly`. Meter: `CORE::BLOCK::Meter`.
- Units after: `additive.hpp`, `additive.indices.hpp`, `voice.internal.hpp`, `voice.cpp`, `voice.render.cpp`, `voice.notes.cpp`, `voice.settle.cpp`, `voice.spectrum.cpp`, `voice.tone.cpp` — the shape of instrument/wavetable.

## Laws

- 379: 1 tree, 2 includes (`../../core/<family>/<family>.hpp` only; `<xmmintrin.h>`, `<threads.hpp>`, `<cstdio>` leave the instrument), 3 `SOUND::PLUGINS::ADDITIVE`, 4 the sheet is the core's (rows held as `Float rows[PARAMETERS]`), 5 names (no `440.0f`, `69.0f`, `4294967296.0f`: `CORE::PHASE`), 6 honesty, 7 cores (allocator, envelopes, LFO, meter never re-rolled).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings (no composed labels); no `switch` — event kinds by `CORE::VOICE::apply`, waves by the core's table; no magic literals (0.40, 0.45, 6.02, 4, ⅓, 8, 60, 16 named).

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::ADDITIVE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree; `grep -c switch instrument/additive/*` 0 in every unit.
- Heard: silent before a note; a chord sounds, bright on each strike and closing within half a second (Envelope, Close); Partials 8 is duller than 32; Stretch +60 turns it bell-like; Balance −1 hollow; a C7 stays clean, no fold-back whistle; a soft note darker than a hard one (Touch).

## Learned

| Method | Source | Licence | Here |
|---|---|---|---|
| bump spectral envelope: centre, width, gains normalised to a fixed sum | Plaits `plaits/dsp/engine/additive_engine.cc` | MIT | this round |
| organ subset: the bump read at drawbar-like harmonics | Plaits `additive_engine.cc` | MIT | later |
| sine bank with each gain ramped across the block | Plaits `plaits/dsp/oscillator/harmonic_oscillator.h` | MIT | this round (the ramp; the recurrence later) |
| Nyquist fade per partial | Plaits `harmonic_oscillator.h`; ZynAddSubFX `src/Synth/OscilGen.cpp` | MIT; GPL-2.0+ | this round |
| harmonic filter: each harmonic's gain times a filter curve | ZynAddSubFX `OscilGen.cpp` | GPL-2.0+ | this round |
| harmonic position by a power law (stretch) | ZynAddSubFX `src/Params/PADnoteParameters.cpp` | GPL-2.0+ | this round |
| spectral generator rebuilt only when a row moves | Tunefish 4 `src/tunefish4/Source/synth/tf4.cpp` | GPL-3.0+ | this round (per stride) |
| coupled two-state resonators; rotation-form sines | SuperCollider `OscUGens.cpp` (Klang); Faust `oscillators.lib` | GPL-3.0; LGPL-2.1+ w/ exception | later |
| PADsynth bandwidth table | ZynAddSubFX `PADnoteParameters.cpp`, `PADnote.cpp` | GPL-2.0+ | later (41) |
| noise through a bandpass per harmonic | ZynAddSubFX `src/Synth/SUBnote.cpp` | GPL-2.0+ | later |
| table rebuilds off the audio thread | Yoshimi | GPL-2.0+ | later |
| time-varying partials for resynthesis | Loris | GPL-2.0+ | after seam S2 |

Cores: core/block
