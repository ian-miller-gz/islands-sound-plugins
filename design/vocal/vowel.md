# vowel — round 1

Gains derived vowel targets in place of the table taken from Csound, plus a register Voice, a free Target vowel with Motion, Tension in place of Open, Effort, Drift, voice modes and a stereo place, all settled at control rate. Answers matrix lacks 1, 29, 30, 31, 33, 34, 9, 12, 5 and 14. Later rounds: the consonant layer (35), band-limited glottal tables (18), microtuning (7), programs (3), Bend (S1), lyrics (S6).

## Rows

The 13 standing rows keep their indices. Row 4 is re-founded from Open (0.3–0.9, resting 0.6) to Tension (0–1); Tension 0.5 gives today's open quotient of 0.6. Morph now moves toward Target, where it used to move toward the next vowel; Target resting on E keeps today's resting path. New rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Gain | | 0 | 1 | 0.5 | 0 | | |
| 1 | Vowel | | 0 | 4 | 0 | 4 | A, E, I, O, U | |
| 2 | Morph | | 0 | 1 | 0 | 0 | | toward Target |
| 3 | Sex | | -1 | 1 | 0 | 0 | | |
| 4 | Tension | | 0 | 1 | 0.5 | 0 | | re-founded (was Open) |
| 5 | Breath | | 0 | 1 | 0.1 | 0 | | |
| 6 | Rate | Hz | 0.1 | 12 | 5.5 | 0 | | |
| 7 | Depth | ct | 0 | 100 | 25 | 0 | | |
| 8 | Delay | s | 0 | 4 | 0.4 | 0 | | |
| 9–12 | Attack, Decay, Sustain, Release | s, s, —, s | 0.001, 0.001, 0, 0.001 | 4, 4, 1, 4 | 0.05, 0.2, 0.8, 0.3 | 0 | | |
| 13 | Voice | | 0 | 3 | 1 | 3 | Bass, Tenor, Alto, Soprano | new |
| 14 | Target | | 0 | 4 | 1 | 4 | A, E, I, O, U | new |
| 15 | Motion | Hz | 0.01 | 2 | 0.1 | 0 | | new |
| 16 | Effort | | 0 | 1 | 0.5 | 0 | | new |
| 17 | Drift | ct | 0 | 20 | 4 | 0 | | new |
| 18 | Mode | | 0 | 2 | 0 | 2 | Poly, Mono, Legato | new |
| 19 | Glide | s | 0 | 2 | 0.08 | 0 | | new |
| 20 | Voices | | 1 | 8 | 8 | 7 | | new |
| 21 | Width | | 0 | 1 | 0.3 | 0 | | new |

The chain is the source–filter voice: a glottal pulse into the parallel formant branch of the published cascade/parallel synthesizer (Klatt 1980, JASA 67(3)), with formant targets derived from the quarter-wave tube (Fant 1960). Every target below is ours, derived by rule; none is a measured table. The ranges and the step words are ours, and the Mode words match the sampler's. With Voice on Tenor, Target on E and Tension at 0.5, the instrument rests on today's register, path and pulse. Drift and Width rest non-zero so the demo can hear them.

How the rows act:
- Morph sets the place between Vowel and Target as Morph × (1 + sway)/2, where sway is a sine at the Motion rate (choir's rule).
- Effort × velocity lifts the upper formants.
- Drift sets a slow wander in cents, plus a jitter at a fifth of that depth.
- Width scales a seeded place per voice slot.

## Chain

| Stage | Rides |
|---|---|
| allocation: Voices 1–8, oldest voice stolen. Mode maps through a table: Poly → `POLY`; Mono → `MONO` with legato off; Legato → `MONO` with legato on and slide `LEGATO`. Glide applies only in Mono and Legato (time 0 in Poly); `VOICE::settle` runs at the stride rate | `core/voice` (stands) |
| control once per stride (16 frames): per voice, cents = vibrato + Drift × (wobble + jitter ÷ 5), and pitch = glided key + cents ÷ 100, passed to `GLOTTIS::tune` (today's per-sample `hertz` leaves). The tract is aimed by Voice, Vowel, Target and the Morph part, bent by pitch and by Effort × velocity, then glided; a struck note (not a tied one) jumps straight to its vowel | NEEDS core/block — `Stride` (see design/subtractive/acid.md); NEEDS core/modulator — `Drift` (see design/subtractive/dco.md), two per voice (wobble 0.4 s, jitter 0.01 s), seeded per slot; NEEDS core/filter — `Tract` and `Formant`, defined below |
| source: the glottal pulse shaped by Tension, with its tilt, and Breath only in the open phase | NEEDS core/glottis — tension, defined below |
| the five-band formant bank, Sex as the fine shift (±400 ct, as today) | `core/filter` (as below) |
| loudness ADSR, velocity at full depth (as today) | `core/envelope` (stands) |
| each voice placed at its seeded place × Width and summed per side | NEEDS core/stereo — the equal-power pan law (see design/subtractive/pads.md) |
| Gain; the lanes; flush, cursor, and a meter over the loudest lane | NEEDS core/stereo — `pour`, defined below; `core/block` |

**NEEDS core/filter — `Tract` and `Formant` re-founded (lacks 1, 29–33; riders: choir, vowel, whisper).**
- The five-vowel table in `filter.formant.cpp` leaves.
- A vowel is a place (open, front). A register is (length cm, pharynx, ring). Both are `constexpr` tables of ours, with values in the targets table below.
- `aim(tract, register, vowel, target, part)`:
  - The register is a place 0–3, blending the constants of adjacent registers.
  - The vowel and the target are places 0–4; a fraction blends adjacent vowels. Their (open, front) pairs are blended by `part`.
  - Rn = (2n − 1) × 35 000 ÷ (4 × length).
  - Centres: F1 = R1 × (0.55 + 0.95 open); F2 = R2 × (0.55 + 0.95 front) × (1 + pharynx × front); F3 = R3 × (0.95 + 0.2 front); F4 = R4; F5 = R5. Then F3–F5 are drawn toward their mean by `ring`.
  - Widths: 40 Hz + 0.04 × Fn.
  - Levels: −6 dB per octave of Fn ÷ F1, so F1 sits at 0 dB.
- `bend(tract, hertz, effort)`:
  - F1 is lifted to at least 1.15 × hertz, and F2 to at least 2.3 × hertz. Hertz 0 leaves the tract as it is.
  - The F3–F5 levels gain 12 dB × effort.
- `Formant` holds the current tract and `breadth` (0.5–4, resting 1), which scales every width.
- `glide(formant, tract, pole, shift, rate) -> Flag`:
  - Called once per stride. Centres, widths and levels move toward the tract by the one-pole `pole`.
  - The five `BAND` biquads re-settle (centre × shift, Q = centre ÷ (width × breadth)) only when a centre has moved more than a named 0.1 %.
- `jump(formant, tract, shift, rate)` lands on the tract at once.
- `makeup(formant, rate) -> Float` = 1 ÷ √(Σ gainₙ² × π × widthₙ × breadth ÷ rate), so white noise leaves the bank at its own RMS.
- The standing `settle(formant, vowel, shift, rate)` stays, meaning: aim Tenor from `vowel` toward the next vowel, then jump. That keeps the tree building between shifts. The last of the three vocal orders to land deletes it if no caller is left.

**NEEDS core/glottis — tension (lack 34; riders: choir, vowel, whisper).**
- `settle(source, tension, breath, rate)`:
  - The open quotient = `WIDEST` − tension × (`WIDEST` − `CLOSEST`); tension 0.5 gives today's 0.6.
  - A `FILTER::Pole` low-pass on `Source` acts as the spectral tilt, with corner 500 Hz × 2^(4 × tension): a lax voice is dark, a pressed one bright.
  - Breath sounds only in the open phase (the closed-phase gate drops from 0.25 to 0) and is scaled by 1 − 0.5 × tension.
- The standing `settle(source, open, breath)` stays as it is (no tilt, today's gate) until the last vocal order lands.

**NEEDS core/stereo — `pour(lanes, channels, frame, pair)` (riders: choir, vowel, whisper).**
- Lifted from choir's `pour`.
- Lane 0 is left and lane 1 is right; further lanes take the mean, and a single lane takes the mean.

Targets (ours; the library writes them as `constexpr`):

| Vowel | open | front | | Register | length cm | pharynx | ring |
|---|---|---|---|---|---|---|---|
| A | 1 | 0.25 | | Bass | 18.5 | 0 | 0.5 |
| E | 0.45 | 0.8 | | Tenor | 17.5 | 0 | 0.4 |
| I | 0 | 1 | | Alto | 15.5 | 0.1 | 0 |
| O | 0.4 | 0.05 | | Soprano | 14.5 | 0.15 | 0 |
| U | 0 | 0 | | | | | |

For example, Tenor A gives centres 750, 1181, 2900, 3500 and 4100 Hz; Tenor I gives 275 and 2250 Hz.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. The instrument is a source: it declares no AUDIO in and writes every sample. It becomes stereo: lane 0 left, lane 1 right, with further lanes and a single lane taking the mean.
- voicing: `poly` (Mode Mono plays one voice of a polyphonic instrument, as dco does).
- Meter: `CORE::BLOCK::Meter` over `CORE::BLOCK::loudest`.

## Laws

- 379: law 1 (tree), law 2 (includes: core aggregates only), law 3 (context `SOUND::PLUGINS::VOWEL`), law 4 (the sheet; new rows after Release), law 5 (names), law 6 (honesty), law 7 (cores: the tract, the tension and the pan law are the cores'; nothing is re-rolled here).
- 381: law 1; law 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`); law 3 (files under instruments/poly); law 4 (no AUDIO in).
- Size: files ≤ 100 lines, functions ≤ 25.
  - `vowel.hpp` stands at 57 lines. Its words (`VOWELS`, `VOICES`, `MODES`) and range constants move to `vowel.words.hpp`.
  - The per-stride steering goes to a new `voice.steer.cpp`; `voice.notes.cpp` stands at 62 lines.
- Code rules:
  - No comments and no natural-language strings.
  - Mode maps through a table, never a switch.
  - No magic literals: 16, 0.4, 0.01, 5 and 100 are named.

## Gate

The build waits until the library shift has landed `Tract`/`Formant`, the glottis tension, `Drift`, `Stride` and core/stereo.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::VOWEL::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- What the reviewer must hear:
  - silence before the note, then a sung "ah" on the note;
  - a chord spread left and right (Width);
  - Voice Soprano sounding smaller and higher;
  - Morph 1 landing on "eh" without zipper;
  - Tension 0 dark and breathy, Tension 1 bright and pressed;
  - a hard note brighter than a soft one (Effort);
  - Mode Legato gliding between two held notes.

## Learned

| Method | Source | Licence | Here |
|---|---|---|---|
| one tension value shapes the pulse from lax to pressed | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) | this round (core/glottis) |
| aspiration lowered as tension rises, gated by the glottal phase | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) | this round |
| a one-pole spectral tilt; breath only in the open phase | klatt-syn `src/Klatt.ts` | MIT | this round |
| pitch wobble: vibrato plus slow smooth noises at unrelated rates | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) | this round (Drift) |
| a voice type × vowel grid, blended bilinearly, with register acting as the shift | Plaits `dsp/speech/naive_speech_synth.cc`; Braids `digital_oscillator.cc` | MIT | this round (Voice; our targets are derived, not tabled) |
| a 2-D vowel square with centres in log frequency | Vital `src/synthesis/filters/formant_filter.cpp` | GPL-3.0 | read; our place is (open, front) |
| a vowel glide on the geometric mean of the centres | Odin 2 `Source/audio/Filters/FormantFilter.cpp` | GPL-3.0+ | read |
| every formant parameter one-pole smoothed | ZynAddSubFX `src/DSP/FormantFilter.cpp` | GPL-2.0+ | this round (`glide`) |
| coefficients per block, crossfaded per sample | pink-trombone-mod `src/Tract.ts` | MIT (the port's) | read; one-pole per stride chosen |
| autobend and vocal effort | Faust `physmodels.lib` (after chant-lib) | LGPL-2.1+ with exception | this round (`bend`, Effort) |
| unison sub-voices with seeded places | ZynAddSubFX `ADnote`; Vital `synth_oscillator.h` | GPL-2.0+; GPL-3.0 | this round in part (a seeded place per slot) |
| Klatt cascade and parallel, frication, the nasal pair | klatt-syn `src/Klatt.ts`; eSpeak NG `klatt.c` | MIT; GPL-3.0+ | round 2 (consonant) |
| a BLEP-corrected pulse train | Plaits `dsp/speech/sam_speech_synth.cc` | MIT | round 2 (lack 18) |

Cores: core/block, core/filter, core/glottis, core/modulator, core/stereo
