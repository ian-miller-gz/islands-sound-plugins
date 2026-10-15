# whisper — round 1

Gains derived vowel targets in place of the table taken from Csound, plus a register Voice and Sex, a free Target vowel with Motion, a Breadth of the formants with a derived level in place of the ×8 makeup, Voicing (a faint glottal pulse under the noise), voice modes and a stereo place. Answers matrix lacks 1, 29, 30, 31, 32, 34 (whisper's Voicing), 12 and 5. Later rounds: fricatives and plosives over the consonant layer (35), microtuning (7), programs (3), Bend (S1), lyrics (S6).

## Rows

The 9 standing rows keep their indices, ranges and rests. Morph now moves toward Target, where it used to move toward the next vowel; Target resting on E keeps today's path from A. New rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Gain | | 0 | 1 | 0.5 | 0 | | |
| 1 | Colour | | 0 | 3 | 0 | 3 | White, Pink, Brown, Blue | |
| 2 | Vowel | | 0 | 4 | 0 | 4 | A, E, I, O, U | |
| 3 | Morph | | 0 | 1 | 0 | 0 | | toward Target |
| 4 | Tracking | | 0 | 1 | 0.5 | 0 | | |
| 5–8 | Attack, Decay, Sustain, Release | s, s, —, s | 0.001, 0.001, 0, 0.001 | 4, 4, 1, 4 | 0.02, 0.2, 0.8, 0.3 | 0 | | |
| 9 | Voice | | 0 | 3 | 1 | 3 | Bass, Tenor, Alto, Soprano | new |
| 10 | Sex | | -1 | 1 | 0 | 0 | | new |
| 11 | Target | | 0 | 4 | 1 | 4 | A, E, I, O, U | new |
| 12 | Motion | Hz | 0.01 | 2 | 0.1 | 0 | | new |
| 13 | Breadth | | 0.5 | 4 | 2 | 0 | | new |
| 14 | Voicing | | 0 | 1 | 0.1 | 0 | | new |
| 15 | Mode | | 0 | 2 | 0 | 2 | Poly, Mono, Legato | new |
| 16 | Glide | s | 0 | 2 | 0.08 | 0 | | new |
| 17 | Width | | 0 | 1 | 0.5 | 0 | | new |

The chain is the noise-excited branch of the cascade/parallel synthesizer (Klatt 1980): aspiration into the formant resonators. Targets come from vowel's derived tract.

Values and words chosen:
- Breadth rests at 2, because whispered formants are wider than voiced ones.
- Voicing rests at 0.1, a faint pulse under the hiss, so the demo hears the row.
- Width is the stereo place, as across this round. Breadth is the formant width, the word additive took for a bump's width.
- Sex is ±400 ct, as vowel's.

How the rows act:
- Morph sets the place between Vowel and Target as Morph × (1 + sway)/2, where sway is a sine at the Motion rate (choir's rule).
- Mode words and their table are vowel's; Glide applies only in Mono and Legato.

## Chain

| Stage | Rides |
|---|---|
| allocation: eight voices, oldest stolen; Mode maps through a table; Glide | `core/voice` (stands) |
| control once per stride (16 frames). Per voice: the tract is aimed by Voice, Vowel, Target and the Morph part, with no bend (hertz 0) and effort 0. Breadth goes on the formant. The shift = Sex × Tracking of the glided key about 60 (today's rule, now per stride, not only at strike). The formant glides, and jumps on a strike | NEEDS core/block — `Stride` (see design/subtractive/acid.md); NEEDS core/filter — `Tract`, `Formant` `breadth`, `glide`, `jump` (see design/vocal/vowel.md) |
| excitation per voice: the noise by Colour (the standing table) × `makeup` × (1 − Voicing), plus a glottal pulse at the glided key × Voicing, both into one bank. The pulse is lax (tension 0.3, named) with no breath; one `GLOTTIS::Source` per voice, and the pulse table is built at create | `core/noise` (stands); NEEDS core/glottis — tension (see design/vocal/vowel.md); NEEDS core/filter — `makeup` (see design/vocal/vowel.md) |
| the five-band formant bank | `core/filter` |
| loudness ADSR (stands) | `core/envelope` |
| each voice placed at its seeded place × Width and summed per side | NEEDS core/stereo — the pan law (see design/subtractive/pads.md) |
| Gain (`LOUDNESS` 8 leaves); the lanes; flush, cursor, and a meter over the loudest lane | NEEDS core/stereo — `pour` (see design/vocal/vowel.md); `core/block` |

`makeup` normalises white noise. Pink, Brown and Blue keep their own levels relative to white, as today's single makeup did.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. The instrument is a source: it declares no AUDIO in and writes every sample. It becomes stereo: lane 0 left, lane 1 right, with further lanes and a single lane taking the mean.
- voicing: `poly`.
- Meter: `CORE::BLOCK::Meter` over `CORE::BLOCK::loudest`.

## Laws

- 379: law 1 (tree), law 2 (includes: core aggregates only; `glottis` joins `filter`, `noise` and `table`), law 3 (context `SOUND::PLUGINS::WHISPER`), law 4 (the sheet; new rows after Release), law 5 (names), law 6 (honesty), law 7 (cores: the level comes from `makeup`, never a guessed constant).
- 381: law 1; law 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`); law 3 (files under instruments/poly); law 4 (no AUDIO in).
- Size: files ≤ 100 lines, functions ≤ 25.
  - `whisper.hpp` stands at 51 lines; its words (`NOISES`, `VOWELS`, `VOICES`, `MODES`) and range constants move to `whisper.words.hpp`.
  - `voice.notes.cpp` stands at 68 lines; the per-stride steering goes to a new `voice.steer.cpp`.
- Code rules:
  - No comments and no natural-language strings.
  - Colour and Mode map through tables, never a switch.
  - No magic literals: 16, 60 and 0.3 are named.

## Gate

The build waits until the library shift has landed `Tract`/`Formant` with `breadth` and `makeup`, the glottis tension, `Stride` and core/stereo.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::WHISPER::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- What the reviewer must hear:
  - silence before the note, then a whispered "ah" on the note, spread across the two sides;
  - Voicing 0.6 giving a breathy pitched note;
  - Breadth sweeping from 0.5 to 4, moving from whistled to diffuse at an even level;
  - Voice Soprano sounding smaller;
  - Morph 1 landing on "eh" without zipper.

## Learned

| Method | Source | Licence | Here |
|---|---|---|---|
| noise-excited formants: aspiration into the resonators | klatt-syn `src/Klatt.ts` | MIT | stands; Breadth this round |
| an excitation mix from whisper to breathy voice | Faust `physmodels.lib`; Synthesizer V Studio 2 (Voicing, per its reviews) | LGPL-2.1+ with exception; commercial | this round (Voicing) |
| a voice type × vowel grid | Plaits `dsp/speech/naive_speech_synth.cc`; Braids `digital_oscillator.cc` | MIT | this round (Voice, through the core) |
| every formant parameter one-pole smoothed | ZynAddSubFX `src/DSP/FormantFilter.cpp` | GPL-2.0+ | this round (core `glide`) |
| a 2-D vowel square in log frequency | Vital `src/synthesis/filters/formant_filter.cpp` | GPL-3.0 | read; our place is (open, front) |
| turbulence where the tract narrows; plosive impulses | pink-trombone-mod `src/Tract.ts` | MIT (the port's) | round 2 (35) |
| frication into F2 and up, alternating signs | klatt-syn `src/Klatt.ts`; eSpeak NG `klatt.c` | MIT; GPL-3.0+ | round 2 (35) |

Cores: core/block, core/filter, core/glottis, core/stereo
