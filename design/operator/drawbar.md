# drawbar — round 1

Gains key contacts that close one by one with their own click, tonewheel leakage, tonewheel loading, a preamp whose bias sags at 2x, and a reverb in the instrument — matrix lacks 61, 60, 66, 65, 10. Later rounds: the lower manual and pedals by key split (62), the scanner as a swept line by a shaped table (63), the rotary's mic and cabinet paths with spin-up and spin-down per rotor (64), the gear/equal tuning switch (7, with the shared tuning).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0–8 | 16', 5 1/3', 8', 4', 2 2/3', 2', 1 3/5', 1 1/3', 1' | | 0 | 8 | 8, 8, 8, 0, 0, 0, 0, 0, 0 | 8 | 0 … 8 | |
| 9 | Percussion | | 0 | 1 | 1 | 1 | | |
| 10 | Harmonic | | 0 | 1 | 1 | 1 | Second, Third | |
| 11 | Decay | | 0 | 1 | 0 | 1 | Fast, Slow | |
| 12 | Level | | 0 | 1 | 0 | 1 | Normal, Soft | |
| 13 | Click | | 0 | 1 | 0.3 | 0 | | |
| 14 | Scanner | | 0 | 6 | 6 | 6 | Off, V1, V2, V3, C1, C2, C3 | |
| 15 | Rotary | | 0 | 2 | 1 | 2 | Off, Slow, Fast | |
| 16 | Drive | | 0 | 1 | 0 | 0 | | |
| 17 | Balance | | 0 | 1 | 0.5 | 0 | | |
| 18 | Tune | ct | -100 | 100 | 0 | 0 | | |
| 19 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 20 | Contact | ms | 0 | 8 | 3 | 0 | | new |
| 21 | Leakage | | 0 | 1 | 0.3 | 0 | | new |
| 22 | Loading | | 0 | 1 | 0.5 | 0 | | new |
| 23 | Sag | | 0 | 1 | 0.4 | 0 | | new |
| 24 | Reverb | | 0 | 1 | 0.15 | 0 | | new |

Chain: the tonewheel organ's public panel (nine drawbars 0–8 at 16' … 1', percussion second/third, fast/slow, normal/soft, the scanner V1–V3/C1–C3, the rotary slow/fast, 91 wheels at the gear ratios) and its tank reverb. The new rows' values are ours: a key's nine contacts close at times drawn once per press across Contact ms, each closing its bar's wheel at once with a click of Click/3 (Click keeps its row; the one burst per key becomes one per contact); Leakage 1 bleeds each drawn wheel's neighbours at −40 dB; Loading 1 drops a full chord (all nine bars on ten keys) by about 3 dB; Sag sets how far the preamp's bias follows its level (Drive 0 keeps today's bypass); Reverb is the wet of a fixed room (four lines, 1.5 s, 4 kHz damping).

## Chain

| Stage | Rides |
|---|---|
| key contacts: per key a pending set of nine closing times (seeded draw at press), counted down per frame; a closing adds that bar to `pull` and strikes a click; the lift opens all at once | instrument (`voice.contacts.cpp`); `core/modulator` `draw`, `core/noise` `Burst` (stand) |
| wheels: 91 sines at the gear ratios; targets from closed contacts × bar gains; leakage from a neighbour table built at create (the wheels an octave either side in the compartment and the two adjacent wheels, gains ours), only through drawn wheels so silence holds | instrument `voice.wheels.cpp`; `core/oscillator` `Table` (stands) |
| loading: every target × 1 / (1 + Loading × the summed drawn gain × a named load) | instrument |
| percussion, scanner (stand) | `core/envelope`; `core/line`, `core/modulator` |
| preamp at 2x: an asymmetric biased soft clip, the bias following a leaky level follower × Sag, Drive the gain | `core/shaper` `Oversampler`, `soft`; `core/dynamics` `Detector` (stand) |
| rotary (stands) | `core/line`, `core/filter`, `core/modulator` |
| reverb: the pair's mean into the network, wet × Reverb added per side | `core/reverb` `Network` (stands; built at create) |

No new core: the contacts, leakage, loading and preamp have one rider each and stay in the instrument.

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in. Left and right as today; one lane gets their mean.
- voicing: `poly` (every key sounds). Meter: `CORE::BLOCK::Meter` over the loudest lane.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::DRAWBAR`, 4 the sheet (rows appended to `drawbar.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores (reverb, oversampler and detector ridden, never re-rolled).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines (`voice.internal.hpp` stands at 90: the contacts and preamp state take their own header), functions ≤ 25; no comments; no natural-language strings; no switch where a table serves; no magic literals (−40 dB, 3 dB, 1.5 s, 4 kHz, the load named).

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::DRAWBAR::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a chord sounds through the rotary; each key speaks with a short, uneven click; a ten-key chord is a little quieter per key than one key; a faint halo of neighbour pitches under a held note; Drive 0.6 growls and squashes on a loud chord; a short room tail after the release.

## Learned

| Method | Source | Licence |
|---|---|---|
| key click per busbar: each contact's attack its own, the contacts closing at slightly random times | setBfree `src/tonegen.c` | GPL-2.0+ (method only) |
| leakage built once into a per-key table: compartment neighbours and adjacent spill; tiny terms pruned | setBfree `src/tonegen.c` | GPL-2.0+ (method only) |
| tonewheel loading: level drops as keys pile on | setBfree `src/tonegen.c` | GPL-2.0+ (method only) |
| preamp: an asymmetric biased soft clip whose bias follows a leaky level follower (sag) | setBfree `b_overdrive/overdrive.c` | GPL-2.0+ (method only) |
| reverb inside the organ | setBfree; VB3-II | GPL-2.0+ (method only); commercial (feature page) |
