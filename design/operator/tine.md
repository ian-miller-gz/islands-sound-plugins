# tine — round 1

Gains key-dependent timbre, a velocity curve with a darkening low-pass, the hammer's thump and the damper's stop, a hold row, and a stereo image by key — matrix lacks 57, 4, 58, 38, 5. Later rounds: the reed mode (59), drive and chorus in the instrument (10, after core/line `Chorus` stands), a modal tone bar (27), anti-aliasing of the pickup (6).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tone | | 0 | 4 | 1.2 | 0 | | |
| 1 | Velocity | | 0 | 1 | 0.8 | 0 | | |
| 2 | Strike | s | 0.01 | 2 | 0.3 | 0 | | |
| 3 | Coupling | | 0 | 1 | 0.2 | 0 | | |
| 4 | Bell | | 0 | 1 | 0.25 | 0 | | |
| 5 | Ring | s | 0.01 | 2 | 0.4 | 0 | | |
| 6 | Pickup | | 0 | 1 | 0.5 | 0 | | |
| 7 | Voicing | | -1 | 1 | 0.3 | 0 | | |
| 8 | Decay | s | 0.5 | 20 | 6 | 0 | | |
| 9 | Release | s | 0.02 | 2 | 0.12 | 0 | | |
| 10 | Rate | Hz | 0.1 | 12 | 4.5 | 0 | | |
| 11 | Depth | | 0 | 1 | 0.3 | 0 | | |
| 12 | Stereo | | 0 | 1 | 1 | 0 | | |
| 13 | Tune | ct | -100 | 100 | 0 | 0 | | |
| 14 | Bend | st | -12 | 12 | 0 | 0 | | |
| 15 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 16 | Taper | | 0 | 1 | 0.5 | 0 | | new |
| 17 | Curve | | -1 | 1 | 0 | 0 | | new |
| 18 | Touch | oct | 0 | 6 | 3 | 0 | | new |
| 19 | Hammer | | 0 | 1 | 0.3 | 0 | | new |
| 20 | Damper | | 0 | 1 | 0.3 | 0 | | new |
| 21 | Hold | | 0 | 1 | 0 | 1 | | new |
| 22 | Width | | 0 | 1 | 0.5 | 0 | | new |

Chain: the tine electric piano (tine and tone bar as a two-operator pair, the bell partial, the pickup's asymmetry, the suitcase's stereo vibrato as tremolo and autopan, the sustain pedal). The new rows' values are ours: Taper scales the index and the bell by 2^(−Taper·(key − 60)/12), at most ×2 below; Curve shapes velocity as v^(4^−Curve) (0 linear, +1 a light touch, −1 a heavy one) before every velocity use; Touch closes a per-voice low-pass from six octaves above the key's pitch by Touch × (1 − v) octaves; Hammer is a 10 ms thump at strike coloured at eight times the key's pitch, Damper a 30 ms tick at the lift coloured at 1.5 kHz, each × v; Width places notes by key about middle C (60), the full field at ±30 keys (pluck's rule).

## Chain

| Stage | Rides |
|---|---|
| allocation, sixteen voices, oldest stolen; Hold via the core's flag (polymod's pattern) | `core/voice` (stands) |
| velocity through Curve, then into level depth, strike, ring, Touch, Hammer, Damper | `core/envelope` `weigh` (stands) |
| tine modulated by the bar, index Tone × (strike + floor) × taper(key); bell × taper(key) | `core/oscillator` `Operator`, `Table` (stand) |
| pickup: biased soft clip (Pickup, Voicing) | `core/shaper` `soft` (stands) |
| per voice low-pass by Touch | `core/filter` `Pole` LOW (stands) |
| per voice hammer burst at strike, damper burst at the lift | `core/noise` `Burst` (stands) |
| loudness ADSR (Decay tracking key, Release), DC blocker | `core/envelope`, `core/shaper` `Blocker` (stand) |
| per voice place by key × Width, summed per side | NEEDS core/stereo (see design/subtractive/pads.md: the equal-power pan law and the key place) |
| tremolo and autopan per side (Rate, Depth, Stereo), Volume | `core/modulator` `Lfo` (stands) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in. Left and right as today, now from per-side sums; one lane gets their mean.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter` over the loudest lane.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::TINE`, 4 the sheet (rows appended to `tine.hpp`'s indices and `ROWS`; move the indices to `tine.indices.hpp` if the root passes 100 lines), 5 names, 6 honesty, 7 cores (no pan law written in the instrument).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines (`voice.settle.cpp` stands at 74: the hammer, damper and touch settle in their own unit), functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (60, 30 keys, six octaves, 10 ms, 30 ms, 1.5 kHz named).

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::TINE::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a chord sounds; low notes bark and high notes ring clean (Taper); a soft note is darker and quieter than a hard one (Touch); a dull thump on every strike and a faint tick on every lift; a rising run walks left to right (Width); Hold 1 sustains past the lift.

## Learned

| Method | Source | Licence |
|---|---|---|
| tine by two FM pairs, the modulator on a high partial with a far shorter decay; velocity sets the depth (and a key-dependent index) | STK `src/Rhodey.cpp` | STK (MIT-like) |
| velocity through an adjustable curve to level and a per-voice darkening low-pass; notes panned by key about the middle | mda ePiano `src/mdaEPiano.cpp` | GPL-3.0+ LV2 port (method only); MIT or GPL-2.0 original |
| hammer and release noise, the damper's stop | Pianoteq electric pianos; Stage-73 V | commercial (feature pages) |
| sustain pedal as a hold on lifts | mda ePiano `src/mdaEPiano.cpp`; Pianoteq (manual) | GPL-3.0+ port (method only); commercial |

Cores: core/stereo
