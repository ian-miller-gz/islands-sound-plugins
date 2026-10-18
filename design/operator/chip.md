# chip — round 1

Gains band-limited steps on every channel, velocity to level, delayed vibrato and glide, and the sweep unit on the pulses — matrix lacks 36, 4, 17 (vibrato with delay; Tune and Bend stand) with the survey's portamento, 67. Later rounds: poly spread over the channels (12), step sequences with loop and release (24, new core/sequence with acid, mono, rhythm), the noise drum mode (68), the console clock switch (69), a pan per channel (5, core/stereo); the sample channel waits on the seam (S2).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Duty1 | | 0 | 3 | 2 | 3 | 12.5%, 25%, 50%, 75% | |
| 1 | Level1 | | 0 | 15 | 15 | 15 | 0 … 15 | |
| 2 | Decay1 | | 0 | 15 | 4 | 15 | 0 … 15 | |
| 3 | Sustain1 | | 0 | 15 | 10 | 15 | 0 … 15 | |
| 4 | Duty2 | | 0 | 3 | 1 | 3 | 12.5%, 25%, 50%, 75% | |
| 5 | Level2 | | 0 | 15 | 0 | 15 | 0 … 15 | |
| 6 | Decay2 | | 0 | 15 | 4 | 15 | 0 … 15 | |
| 7 | Sustain2 | | 0 | 15 | 10 | 15 | 0 … 15 | |
| 8 | Offset2 | st | -24 | 24 | 0 | 48 | | |
| 9 | Triangle | | 0 | 1 | 0 | 1 | | |
| 10 | Offset3 | st | -24 | 24 | -12 | 48 | | |
| 11 | Mode | | 0 | 1 | 0 | 1 | Long, Short | |
| 12 | Period | | 0 | 15 | 8 | 15 | 0 … 15 | |
| 13 | Level4 | | 0 | 15 | 0 | 15 | 0 … 15 | |
| 14 | Decay4 | | 0 | 15 | 1 | 15 | 0 … 15 | |
| 15 | Sustain4 | | 0 | 15 | 0 | 15 | 0 … 15 | |
| 16 | First | st | 0 | 24 | 0 | 24 | 0 … 24 | |
| 17 | Second | st | 0 | 24 | 0 | 24 | 0 … 24 | |
| 18 | Speed | Hz | 1 | 240 | 60 | 0 | | |
| 19 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 20 | Bend | st | -12 | 12 | 0 | 0 | | |
| 21 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 22 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 23 | Vibrato | st | 0 | 2 | 0.25 | 0 | | new |
| 24 | Rate | Hz | 0.5 | 12 | 5.5 | 0 | | new |
| 25 | Delay | s | 0 | 2 | 0.4 | 0 | | new |
| 26 | Glide | s | 0 | 1 | 0 | 0 | | new |
| 27 | Sweep | | -7 | 7 | 0 | 14 | | new |
| 28 | Pace | | 0 | 7 | 3 | 7 | | new |

Chain: the 8-bit console's audio unit (public: two pulses at 12.5/25/50/75 %, a 32-step triangle, a 15-bit noise register long/short over sixteen periods, a 240 Hz quarter frame stepping the envelopes and a 120 Hz half frame stepping the sweep — shift 0–7, divider 0–7, negate, the first pulse negating by ones' complement and the second by two's, a target past 2047 or a period under 8 muting; the nonlinear mixer and the 90 Hz, 440 Hz high-passes and 14 kHz low-pass). Ours: Sweep 0 is off, its size the shift and its sign the direction (positive raises the pitch), Pace the divider, acting on both pulses; Velocity scales each envelope's start level by `weigh(v, Velocity)`, rounded to the 0–15 steps; Vibrato is a sine of Rate Hz faded in after Delay; Glide smooths the key; pitch is read once per quarter frame (the console's own stepped portamento).

## Chain

| Stage | Rides |
|---|---|
| one voice, last priority, retriggered; Glide as the pitch smoother | `core/voice` (glide stands) |
| per quarter frame: glided key + Tune + Bend + arpeggio offset + vibrato → the channels' periods | `core/modulator` `Lfo` (fade stands), `core/phase` |
| per half frame: the sweep on each pulse's period, muting by the console's rule | instrument (`voice.sweep.cpp`) |
| envelopes in steps, start level × velocity weight | `core/envelope` `weigh`; instrument `voice.steps.cpp` |
| channels emit edges: each duty or triangle boundary and each register shift crossed inside a sample, with its fraction 0..1 | instrument (`voice.tone.cpp`, `voice.mixer.cpp`) |
| the nonlinear mix per edge: the step's height is mixed(after) − mixed(before), placed band-limited | NEEDS core/oscillator — `Edges`: a band-limited step accumulator for a stepped signal; `jump(edges, height, fraction)` places a step of `height` at `fraction` 0..1 of the coming sample as the two-sample polyBLEP residual `blep` already computes, generalised to any height and place; `tick(edges, level) -> Float` returns the level plus the residual due now and shifts the next in; allocation-free, one per voice. Riders: chip now; the square banks of hat, cymbal and cowbell a later round |
| three output filters, Volume, one sample to every lane | `core/filter` `Pole` (stands) |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; every lane written (mono as today).
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::CHIP`, 4 the sheet (rows appended to `chip.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores (`Edges` in core/oscillator, never written in the instrument).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::MONO`), 3 instruments/mono, 4 no AUDIO in.
- Files ≤ 100 lines (`voice.internal.hpp` stands at 96: split the channel structs into their own header first), functions ≤ 25; no comments; no natural-language strings; no switch where a table serves (the two negate rules a table of two); no magic literals (2047, 8, 120 Hz named).

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::CHIP::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a note sounds as the square lead; a run up to C7 stays clean, without a whine folding down under it; a held note gains vibrato after 0.4 s; a soft note is quieter by steps; Sweep −3 drops each note like a falling zap; Glide 0.2 slides a run.

## Learned

| Method | Source | Licence |
|---|---|---|
| band-limited steps: a channel reports its level changes with their times; each change added as a band-limited step placed by its fraction | blip_buf `blip_buf.c` (Furnace tree); Game_Music_Emu `gme/Blip_Buffer.h` | LGPL-2.1+ (method only) |
| channels run event to event and emit edges only | Game_Music_Emu `gme/Nes_Oscs.cpp` | LGPL-2.1+ (method only) |
| sweep: target period ± a shifted copy; the first pulse negates by ones' complement; overflow or a tiny period mutes | NESdev wiki APU Sweep; Game_Music_Emu `gme/Nes_Oscs.cpp` | no licence stated (facts); LGPL-2.1+ (method only) |
| pitch as one fractional note: key, bend, sweep, portamento and delayed vibrato summed, converted once | Magical 8bit Plug 2 `Source/TonalVoice.cpp` | GPL-3.0 (method only) |
| velocity to level | Magical 8bit Plug 2; chipsounds | GPL-3.0 (method only); commercial (feature page) |
