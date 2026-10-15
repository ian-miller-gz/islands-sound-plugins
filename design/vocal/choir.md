# choir — round 1

Gains sections (a Voice row, with Mixed following the key), a free Target vowel, Tension, a vibrato Delay, per-singer Drift, a Singers count, rows for the room's Size and Tail, and voice modes. Its control clock and its pan move onto the library. Answers matrix lacks 1, 29, 30, 31, 33, 34, 9, 17, 104, 10, 12 and 14 (choir's twin). Later rounds: Voices past six once the cost is measured (25, 105), a PADsynth back section (41), syllables over the consonant layer (35), band-limited glottal tables (18), microtuning (7), programs (3), Bend (S1).

## Rows

The 17 standing rows keep their indices, ranges and rests. Morph now moves toward Target, where it used to move toward the next vowel; Target resting on E keeps today's path from A. New rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Gain | | 0 | 1 | 0.5 | 0 | | |
| 1 | Vowel | | 0 | 4 | 0 | 4 | A, E, I, O, U | |
| 2 | Morph | | 0 | 1 | 0.3 | 0 | | toward Target |
| 3 | Motion | Hz | 0.01 | 2 | 0.1 | 0 | | |
| 4 | Sex | | -1 | 1 | 0 | 0 | | |
| 5 | Spread | | 0 | 1 | 0.5 | 0 | | |
| 6 | Detune | ct | 0 | 50 | 12 | 0 | | |
| 7 | Scatter | s | 0 | 0.25 | 0.04 | 0 | | |
| 8 | Breath | | 0 | 1 | 0.2 | 0 | | |
| 9 | Rate | Hz | 0.1 | 12 | 5 | 0 | | |
| 10 | Depth | ct | 0 | 100 | 20 | 0 | | |
| 11 | Ensemble | | 0 | 1 | 0.5 | 0 | | |
| 12 | Room | | 0 | 1 | 0.25 | 0 | | |
| 13–16 | Attack, Decay, Sustain, Release | s, s, —, s | 0.001, 0.001, 0, 0.001 | 4, 4, 1, 4 | 0.3, 0.5, 0.9, 0.8 | 0 | | |
| 17 | Voice | | 0 | 4 | 4 | 4 | Bass, Tenor, Alto, Soprano, Mixed | new |
| 18 | Target | | 0 | 4 | 1 | 4 | A, E, I, O, U | new |
| 19 | Tension | | 0 | 1 | 0.5 | 0 | | new |
| 20 | Delay | s | 0 | 4 | 0.3 | 0 | | new |
| 21 | Drift | ct | 0 | 20 | 4 | 0 | | new |
| 22 | Singers | | 1 | 8 | 8 | 7 | | new |
| 23 | Size | | 0.25 | 2 | 0.8 | 0 | | new |
| 24 | Tail | s | 0.3 | 8 | 1.4 | 0 | | new |
| 25 | Mode | | 0 | 2 | 0 | 2 | Poly, Mono, Legato | new |
| 26 | Glide | s | 0 | 2 | 0.08 | 0 | | new |

The chain is the four-part choir (bass, tenor, alto, soprano), each singer a source–filter voice (vowel's chain). Mixed seats each note's part by its key: register place = (key − 48) ÷ 8, held to 0–3, so C3 and below sings bass and C5 and up sings soprano. That rule is ours, after the sectioned sampled choirs; it rests on Mixed so the demo hears it.

Values chosen to keep today's sound:
- Tension 0.5 is today's fixed open quotient of 0.6.
- Size 0.8 and Tail 1.4 s are today's room constants; Damp stays a constant.
- Singers 8 is today's count.

How the rows act:
- Delay is the vibrato's fade-in; today's fade is 0.
- Effort is velocity × 0.5 (named); it has no row.

## Chain

| Stage | Rides |
|---|---|
| allocation: six parts, oldest part stolen. Mode maps through vowel's table; Glide applies only in Mono and Legato | `core/voice` (stands) |
| control once per stride (64 frames, today's `CONTROL`; the instrument's `clock` leaves). Per part: the register (Voice, or Mixed by key); the tract aimed by Voice, Vowel, Target and Morph × (1 + Motion sway)/2; then bent by the glided pitch and the effort | NEEDS core/block — `Stride` (see design/subtractive/acid.md); NEEDS core/filter — `Tract` (see design/vocal/vowel.md) |
| per singer, per stride: cents = seat detune × Detune + vibrato (faded in by Delay) + Drift × (wobble + jitter ÷ 5), passed to `GLOTTIS::tune`. The formant glides to its part's tract with its own shift (Sex × the Spread fan, as today) and jumps on a strike; a tied note glides | NEEDS core/modulator — `Drift` (see design/subtractive/dco.md), two per singer, seeded per seat; NEEDS core/filter — `Formant` `glide`, `jump` (see design/vocal/vowel.md) |
| source: the glottal pulse by Tension, with Breath | NEEDS core/glottis — tension (see design/vocal/vowel.md) |
| only the first Singers of each part sing; the part sum is scaled by 1 ÷ √Singers (today's `BLEND` at 8) | this instrument |
| seat pans move onto the equal-power law (today's linear `panned` leaves) | NEEDS core/stereo — the pan law (see design/subtractive/pads.md) |
| the ensemble: three swept reads per side (stands) | `core/line`, `core/modulator` (stand) |
| the room: four-line network, settled by Size, Tail and the named Damp; Room mix | `core/reverb` (stands) |
| Gain; the lanes (today's `pour` leaves); flush, cursor, and a meter over the loudest lane | NEEDS core/stereo — `pour` (see design/vocal/vowel.md); `core/block` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. The instrument is a source: it declares no AUDIO in and writes every sample. It stays stereo, with lanes as today.
- voicing: `poly`.
- Meter: `CORE::BLOCK::Meter` over `CORE::BLOCK::loudest` (as today).

## Laws

- 379: law 1 (tree), law 2 (includes: core aggregates only), law 3 (context `SOUND::PLUGINS::CHOIR`), law 4 (the sheet; new rows after Release), law 5 (names: Size and Tail, because Decay is the envelope's word here), law 6 (honesty), law 7 (cores: `pour`, the pan law and the stride leave this instrument for the library).
- 381: law 1; law 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`); law 3 (files under instruments/poly); law 4 (no AUDIO in).
- Size: files ≤ 100 lines, functions ≤ 25.
  - `choir.hpp` stands at 69 lines; its words and range constants move to `choir.words.hpp`.
  - `voice.internal.hpp` stands at 96 lines; `Seat`, `Singer`, `Part`, `Side`, `Ensemble` and `Room` move to `voice.seats.hpp`.
  - The register and Mixed rule go to a new `voice.register.cpp`.
- Code rules:
  - No comments and no natural-language strings.
  - Voice and Mode map through tables, never a switch.
  - No magic literals: 48, 8, 0.5, 0.4, 0.01 and 5 are named.

## Gate

The build waits until the library shift has landed `Tract`/`Formant`, the glottis tension, `Drift`, `Stride` and core/stereo.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::CHOIR::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- What the reviewer must hear:
  - silence before the note, then a sung "ah" swelling in;
  - a C2 sung dark by basses and a C6 sung small by sopranos (Mixed);
  - Singers 1 giving a thin solo, Singers 8 a section;
  - Tail 6 s ringing long after release;
  - Tension 1 brighter and pressed;
  - Mode Legato gliding the whole section.

## Learned

| Method | Source | Licence | Here |
|---|---|---|---|
| a voice type × vowel grid; register acting as the shift | Plaits `dsp/speech/naive_speech_synth.cc`; Braids `digital_oscillator.cc` | MIT | this round (Voice, through the core) |
| sections by register (bass, tenor, alto, soprano; male and female) | EastWest Hollywood Choirs (feature pages) | commercial | this round (the Voice words, Mixed by key: ours) |
| a tension pulse; aspiration by tension and phase | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) | this round (core/glottis) |
| pitch wobble: slow smooth noises at unrelated rates | pink-trombone-mod `src/Glottis.ts` | MIT (the port's) | this round (Drift per singer) |
| unison: seeded phases, own vibrato, stereo spread | ZynAddSubFX `ADnote`; Vital `synth_oscillator.h` | GPL-2.0+; GPL-3.0 | stands (seats); vibrato Delay this round |
| every formant parameter one-pole smoothed | ZynAddSubFX `src/DSP/FormantFilter.cpp` | GPL-2.0+ | this round (core `glide`) |
| autobend and vocal effort | Faust `physmodels.lib` (after chant-lib) | LGPL-2.1+ with exception | this round (core `bend`, effort from velocity) |
| a PADsynth ensemble table under the back singers | PADsynth algorithm page; ZynAddSubFX `PADnote` | public domain; GPL-2.0+ | later (41, after the cost is measured) |
| pulse-reset sine formants as a cheap back-singer voice | Plaits `dsp/speech/sam_speech_synth.cc` | MIT | later (105) |
| syllables from onset consonants | Hollywood Choirs WordBuilder; klatt-syn `src/Klatt.ts` | commercial; MIT | round 2 (35) |

Cores: core/block, core/filter, core/glottis, core/modulator, core/stereo
