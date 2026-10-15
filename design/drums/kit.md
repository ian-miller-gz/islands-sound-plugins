# kit — round 1

Gains per-drum Decay and Tone rows beside Tune and Level, a pan per drum and a stereo out, velocity through a curve beyond today's two steps, and a silence test so a quiet drum costs nothing — matrix lacks 28, 5, 4, 88. Later rounds: maracas and the map's second-row drums (87, a shaker circuit), choke groups with a fade (19, once a group can hold several drums), the single drums' new rows (their circuits land first this round), programs (3).

## Rows

The 19 standing rows keep their indices (`place(drum, knob)`, Accent 18); the new blocks join at the tail.

| # | Row | Unit | Least | Most | Rest | |
|---|---|---|---|---|---|---|
| 0–17 | Kick … Conga Tune, Level | st, — | -12, 0 | 12, 1 | 0; 0.8 (Cymbal 0.6, Cowbell 0.7, Clave 0.7) | |
| 18 | Accent | | 0 | 1 | 0.5 | |
| 19 | Kick Decay | s | 0.05 | 3 | 0.5 | new |
| 20 | Snare Decay | s | 0.05 | 1.5 | 0.25 | new |
| 21 | Hat Decay | s | 0.05 | 2 | 0.45 | new |
| 22 | Clap Decay | s | 0.05 | 2 | 0.3 | new |
| 23 | Tom Decay | s | 0.05 | 2 | 0.4 | new |
| 24 | Cymbal Decay | s | 0.2 | 5 | 1.5 | new |
| 25 | Cowbell Decay | s | 0.05 | 2 | 0.35 | new |
| 26 | Clave Decay | s | 0.01 | 0.5 | 0.06 | new |
| 27 | Conga Decay | s | 0.05 | 1.5 | 0.3 | new |
| 28 | Kick Tone | Hz | 200 | 12000 | 4000 | new |
| 29 | Snare Tone | Hz | 500 | 10000 | 3000 | new |
| 30 | Hat Tone | Hz | 2000 | 14000 | 6000 | new |
| 31 | Clap Tone | Hz | 400 | 5000 | 1100 | new |
| 32 | Tom Tone | Hz | 500 | 12000 | 4000 | new |
| 33 | Cymbal Tone | | 0 | 1 | 0.5 | new |
| 34–42 | Kick … Conga Pan | | -1 | 1 | 0, 0.1, -0.35, -0.1, 0.3, 0.45, -0.5, 0.5, -0.25 | new |
| 43 | Curve | | -1 | 1 | 0 | new |

No choice rows. Chain: the analogue kit (the nine circuits seated as one kit over the general MIDI drum map 35–75). Decays and Tones rest at today's voicings and span the single drums' own ranges; Hat Decay is the open hat's (the closed stays 0.06 s); the Tone block covers the six circuits that take a tone today (cowbell, clave and conga gain theirs in their own orders this round, the kit seats them in round 2). Pans rest at a drummer's-seat spread of ours. Velocity: struck through Curve, then × (1 − Accent × 0.6) below 0.75 as today — so velocity now reaches the drum between the two steps.

## Chain

| Stage | Rides |
|---|---|
| note → drum and stroke by the pad table; velocity through Curve and the accent | NEEDS core/percussion — `struck(velocity, curve)` (see design/drums/clap.md) |
| each drum settled from its Tune, Decay and Tone rows (the voicing table keeps only what has no row) | `core/percussion` circuits (stand; standing `settle` signatures) |
| each drum ticked only while sounding | NEEDS core/percussion — `sounding`: every circuit answers `sounding(circuit) -> Flag` and its `tick` returns 0 at once when silent (a resonator below a named floor counts silent). Riders: kit, machine, rhythm (every single drum gains it free) |
| each drum placed by its Pan, summed per side, soft-clipped per side | NEEDS core/stereo (see design/subtractive/pads.md: the equal-power pan law) |
| lanes: left, right, or one lane their mean; the peak of the loudest side | NEEDS core/percussion — `play` for a side pair: the standing frame loop (cursor, `due`/`rest`, clip, peak) with a sound returning a `core/stereo` pair; lane 0 left, lane 1 right, further lanes the mean, a single lane the mean. Riders: kit, machine (pads may ride it) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; now stereo.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter` over the loudest side.

## Laws

- 379: 1, 2, 3 `SOUND::PLUGINS::KIT`, 4 the sheet (`DECAYS`, `TONES`, `PANS`, `CURVE` block starts named; the `static_assert` grows to the new count; rows to `kit.rows.hpp` if `kit.hpp` passes 100), 5, 6, 7 (no pan law in the instrument).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in, 6 (the Pan rows are bipolar; Kick Pan rests at 0 and shows its centre mark).
- Edits `instrument/kit/` only. `voice.notes.cpp` (87) and `voice.settle.cpp` (88) split: the pad table and the voicing table each to a dotted sibling.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings but the row names; no switch; no magic literals.

## Gate

- After the library shift has landed `struck(velocity, curve)`, `sounding`, `play` for a side pair and core/stereo.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::KIT::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a hit, every mapped key sounds; hats left, cymbals right, kick centred; Kick Decay 3 a long boom; a velocity ramp on 38 rises smoothly, not in two steps.

## Learned

| Method | Source | Licence |
|---|---|---|
| per-voice decay, tone and pan on a kit's panel | Roland Cloud TR-808/909 (manuals); Microtonic (product page) | commercial |
| equal-power pan from quarter-offset sines | Bogaudio `signal.cpp` (matrix `## Methods`) | GPL-3.0+ (method only) |
| a silent voice skipped; the hit rendered once and read | Geonkick `src/dsp/src/synthesizer.c` | GPL-3.0+ (method only) |
| velocity through a monotone curve | DrumGizmo `src/powermap.cc` | LGPL-3.0+ (method only) |

Cores: core/percussion, core/stereo
