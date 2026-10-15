# rhythm — round 1

Gains swing, a humanizer (timing and velocity), an accent lane in every pattern, and four more voices from the circuits that stand (tom, clap, rim, cowbell) — matrix lacks 90, 24 (accent steps), 91. Later rounds: pattern variations (24's rest), stamina (90's rest), a pan per voice (5), the host's tempo (seam S4: its own Tempo and Run rows stay), programs (3).

## Rows

The 8 standing rows keep their indices (Pattern, Tempo, Run, Fill, the four voice Levels at `LEVELS + voice`); the new rows join at the tail, a table mapping each voice to its Level row.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Pattern | | 0 | 15 | 0 | 15 | Rock … Foxtrot | |
| 1 | Tempo | bpm | 40 | 240 | 120 | 0 | | |
| 2 | Run | | 0 | 1 | 1 | 1 | | |
| 3 | Fill | | 0 | 1 | 0 | 1 | | |
| 4–7 | Kick, Snare, Hat, Cymbal Level | | 0 | 1 | 0.8, 0.7, 0.5, 0.4 | 0 | | |
| 8 | Swing | | 0 | 0.5 | 0 | 0 | | new |
| 9 | Timing | ms | 0 | 20 | 3 | 0 | | new |
| 10 | Dynamics | | 0 | 1 | 0.15 | 0 | | new |
| 11 | Accent | | 0 | 1 | 0.5 | 0 | | new |
| 12 | Tom Level | | 0 | 1 | 0.6 | 0 | | new |
| 13 | Clap Level | | 0 | 1 | 0.6 | 0 | | new |
| 14 | Rim Level | | 0 | 1 | 0.5 | 0 | | new |
| 15 | Cowbell Level | | 0 | 1 | 0.4 | 0 | | new |

Chain: the preset-rhythm box (sixteen patterns as tables, tempo, run, fill, voices drawn from the analogue circuits, over a frame clock). Swing is `CORE::CLOCK`'s own (step pairs long/short; sixteenth patterns only — the triplet patterns already swing; rest 0 keeps the box straight); Timing delays each internal hit by 0..Timing ms, Dynamics lowers each by up to Dynamics, both drawn from a seeded noise (deterministic per run); Accent: steps of the pattern's accent lane hit full, others at 1 − Accent × 0.4. Every pattern gains an accent lane and lanes for the four new voices, written by this order in our own figures for the named dances (the forms are public; no box's bit table is copied). Notes: 37 rim, 39 clap, 41–50 toms by height, 56 cowbell, beside today's.

## Chain

| Stage | Rides |
|---|---|
| the clock: tempo, division, Swing | `core/clock` (stands; `settle` takes the Swing row, not `STRAIGHT`) |
| per edge: lanes and accent read from the pattern table | the instrument's `voice.patterns*.cpp` (the table splits into dotted siblings, straight and triple, under 100 lines each) |
| a hit delayed by Timing and weighed by Dynamics: one pending hit per voice | `core/noise` `White` (stands); the pending slot is the instrument's |
| eight voices: kick, snare, hat, cymbal, tom, clap, rim (the clave's rim stroke), cowbell | `core/percussion` `Kick`, `Snare`, `Hat`, `Cymbal`, `Tom`, `Clap`, `Clave`, `Cowbell` (stand) |
| each voice ticked only while sounding | NEEDS core/percussion — `sounding` (see design/drums/kit.md) |
| sum × Levels, soft clip, every lane | `core/shaper` `soft`, `core/percussion` `play` (stand) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::RHYTHM`, 4 the sheet (`VOICES` grows to eight; the `static_assert` follows the new count), 5 (the pattern names are the dances' own words, as today), 6, 7.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Edits `instrument/rhythm/` only.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings but row and step words; no switch; no magic literals (the 0.4 accent floor, the ms scale named).

## Gate

- After the library shift has landed `sounding`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::RHYTHM::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: Run 0 silent, Run 1 plays at once; Bossa and Samba carry rim and cowbell, the fills reach the toms; Swing 0.3 on Rock shuffles; accented beats stand out; Timing 20 loosens the hats.

## Learned

| Method | Source | Licence |
|---|---|---|
| humanize as random offsets to timing and velocity; swing delays the off-beat steps | Hydrogen `src/core/Basics/Note.cpp` | GPL-2.0+ (method only) |
| a laid-back offset with a relaxing drift | DrumGizmo `src/latencyfilter.cc` | LGPL-3.0+ (method only) |
| accent per step; shuffle; voices of the preset boxes | Roland Cloud TR-808/909 (manuals); Microtonic (product page) | commercial |

Cores: core/percussion
