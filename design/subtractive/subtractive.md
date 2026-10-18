# subtractive — round 1

Gains its re-founding on the cores (before any gain), band-limited oscillators, a state-variable filter with four modes and keyboard tracking, and a mono mode with glide — matrix lacks 2, 36, 16, 12. Later rounds: an LFO and a second envelope routing (lack 11).

## Rows

The twelve standing rows keep their names, units, ranges, rests and indices (the walks read `row 0 … Gain`); new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Gain | | 0 | 1 | 0.30 | 0 | | |
| 1 | Shape | | 0 | 2 | 0 | 2 | Saw, Pulse, Triangle | |
| 2 | Spread | ct | 0 | 25 | 7 | 0 | | |
| 3 | Cutoff | Hz | 20 | 12000 | 1200 | 0 | | |
| 4 | Resonance | | 0 | 0.95 | 0.30 | 0 | | |
| 5 | Depth | Hz | 0 | 8000 | 2400 | 0 | | |
| 6 | Sweep | s | 0.001 | 4.001 | 0.011 | 0 | | |
| 7 | Close | s | 0.001 | 4.001 | 0.401 | 0 | | |
| 8 | Attack | s | 0.001 | 4.001 | 0.011 | 0 | | |
| 9 | Decay | s | 0.001 | 4.001 | 0.1946 | 0 | | |
| 10 | Sustain | | 0 | 1 | 0.70 | 0 | | |
| 11 | Release | s | 0.001 | 4.001 | 0.251 | 0 | | |
| 12 | Mode | | 0 | 3 | 0 | 3 | Low, Band, High, Notch | new |
| 13 | Keyboard | | 0 | 1 | 0.5 | 0 | | new |
| 14 | Assign | | 0 | 1 | 0 | 1 | Poly, Mono | new |
| 15 | Glide | s | 0 | 3 | 0 | 0 | | new |

Chain: the generic two-oscillator subtractive voice (two detuned oscillators of one shape, a resonant filter swept by a contour, a loudness ADSR, velocity to level), the founding exemplar's own. The standing values are its own; new rows' ranges are ours.

## Chain

| Stage | Rides |
|---|---|
| the sheet: `ROWS`, `SHEET`, `Surface<Synth, SHEET>`; `params.cpp`, `voice.params.cpp`, `voice.steer.cpp` and their six `switch`es leave | `core/table` |
| allocation: eight voices, oldest stolen; Assign Mono = one voice, last priority, legato; Glide | `core/voice` |
| two oscillators of Shape, the second Spread cents above, band-limited, phases reset on a strike; their mean | `core/oscillator` (`SAW`, `PULSE`, `TRIANGLE` match Shape 0–2) |
| filter: Mode through a table onto `LOW`, `BAND`, `HIGH`, `NOTCH`; cutoff (Cutoff + Depth × contour) × 2^(Keyboard × (key − 60) / 12); Resonance as the emphasis | `core/filter` `Variable` |
| contour: attack Sweep, decay and release Close, sustain 0; loudness ADSR; both linear, reset on strike; loudness depth 1 (velocity scales level, as today) | `core/envelope` (`voice.envelope.cpp` and its `switch` leave) |
| denormal flush, event cursor, meter | `core/block` (the hand-rolled flush, publish and `face` leave) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in, every sample written, one sample to every lane.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter`.
- Units after: `subtractive.hpp` (indices, `ROWS`, `SHEET`), `voice.internal.hpp`, `voice.cpp`, `voice.render.cpp`, `voice.notes.cpp`, `voice.settle.cpp`, `voice.tone.cpp` — the shape of instrument/ladder.

## Laws

- 379: 1 tree (the voice units of the exemplar ladder), 2 includes (`../../core/<family>/<family>.hpp` only; no `<xmmintrin.h>`, no `<threads.hpp>` in the instrument), 3 `SOUND::PLUGINS::SUBTRACTIVE`, 4 the sheet is the core's (no hand-written `SURFACE`), 5 names (no `440.0f`, `69.0f`, `2.0f` literals: `CORE::PHASE`), 6 honesty, 7 cores (nothing the cores hold re-rolled).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; no switch at all — tables; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::SUBTRACTIVE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- `grep -c switch instrument/subtractive/*` is 0 in every unit.
- Heard: silent before a note, a chord sounds as before at rest (same rows, cleaner top: no aliasing whistle on a high saw); Mode High thins it, Band hollows it; Assign Mono with Glide slides between keys.

## Learned

| Method | Source | Licence |
|---|---|---|
| polyBLEP placed at the fractional crossing | stmlib `dsp/polyblep.h`; Plaits `variable_shape_oscillator.h` | MIT |
| TPT state-variable, four taps from one loop | stmlib `dsp/filter.h`; Zavalishin, *The Art of VA Filter Design* | MIT; copyright |
| mono mode and portamento | Surge `SurgeVoice.cpp`; Yarns `part.h` | GPL-3.0 (method only); MIT |
| allocator: same note, else idle, else oldest | stmlib `algorithms/voice_allocator.h` | MIT |
