# pads — round 1

Gains a pan per pad and a stereo out under the equal-power law — matrix lack 5 (pads' line). Later rounds: the choke group and a pitch-envelope depth past Bend (lacks 19, 13 — drum rows whose core the drums family's requirements state).

## Rows

The 42 standing rows keep their indices (`place(pad, knob)`); the six Pan rows join at the tail, at `PADS * KNOBS + pad`.

| # | Row | Unit | Least | Most | Rest | |
|---|---|---|---|---|---|---|
| 0–6 | Kick Tune, Bend, Decay, Tone, Noise, Click, Level | Hz, —, s, Hz, —, —, — | 30, 0, 0.05, 200, 0, 0, 0 | 1000, 1, 2, 12000, 1, 1, 1 | 60, 0.6, 0.45, 600, 0.1, 0.5, 0.8 | |
| 7–13 | Snare (the same seven) | as Kick | as Kick | as Kick | 190, 0.3, 0.3, 6000, 0.6, 0.4, 0.8 | |
| 14–20 | High | as Kick | as Kick | as Kick | 240, 0.5, 0.4, 2500, 0.2, 0.3, 0.8 | |
| 21–27 | Mid | as Kick | as Kick | as Kick | 180, 0.5, 0.5, 2000, 0.2, 0.3, 0.8 | |
| 28–34 | Low | as Kick | as Kick | as Kick | 130, 0.5, 0.6, 1600, 0.2, 0.3, 0.8 | |
| 35–41 | Floor | as Kick | as Kick | as Kick | 95, 0.5, 0.8, 1200, 0.2, 0.3, 0.8 | |
| 42 | Kick Pan | | -1 | 1 | 0 | new |
| 43 | Snare Pan | | -1 | 1 | 0.15 | new |
| 44 | High Pan | | -1 | 1 | -0.5 | new |
| 45 | Mid Pan | | -1 | 1 | -0.2 | new |
| 46 | Low Pan | | -1 | 1 | 0.2 | new |
| 47 | Floor Pan | | -1 | 1 | 0.5 | new |

No choice rows. Chain: the hexagonal-pad kit (six swept-tone and noise pads with click and bend, one per note). Pans rest at a drummer's-seat spread of our own; the Pan names follow the sheet's standing `<Pad> <Knob>` spelling.

## Chain

| Stage | Rides |
|---|---|
| note → pad by the key table (35–50), velocity to level | `core/percussion` (`keyed`, `Pad`, `struck` stand) |
| each pad's voice | `core/percussion` `Pad` |
| each pad placed by its Pan, summed per side, soft-clipped per side | NEEDS core/stereo — the equal-power pan law: a place −1..1 to left and right gains, a centred source keeping today's level on each lane; a key place from pitch about a centre key scaled by a width (pluck rides it too) |
| the frame loop: this instrument's own over `CORE::BLOCK` (cursor, flush, loudest lane); `PERCUSSION::play` writes one sample to every lane and no longer serves | `core/block`, `core/shaper` `soft` |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; lane 0 left, lane 1 right, a single lane the mono sum (the dco's `side` rule).
- voicing: `poly`. Meter: `CORE::BLOCK::Meter` over the loudest lane.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::PADS`, 4 the sheet (a `PAN` block after the knob block; the `static_assert` grows to `PADS * KNOBS + PADS`), 5 names, 6 honesty, 7 cores.
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in, 6 (Kick Pan is bipolar and rests at 0: its centre mark shows).
- Files ≤ 100 lines (`pads.hpp` splits its rows into a dotted sibling if it would pass 100), functions ≤ 25; no comments; no natural-language strings but the row names; no switch; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::PADS::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a hit, every pad sounds on its key; a tom roll (keys 50, 47, 43, 41) walks left to right across the field; the kick stays centred.

## Learned

| Method | Source | Licence |
|---|---|---|
| equal-power pan from quarter-offset sines | Bogaudio `signal.cpp` (matrix `## Methods`) | GPL-3.0+ (method only) |
| a pan per drum in a kit | Roland Cloud TR-808/909, Microtonic (product pages, drums survey) | commercial pages |

Cores: core/percussion, core/stereo
