# supersaw — round 1

Gains an LFO on pitch and cutoff, filter modes beyond the 24 dB low-pass, velocity to level and cutoff, a stereo chorus in the instrument; its control-rate counter moves onto the library — matrix lacks 11 (the LFO; the slots a later round), 16, 4, 10 (the chorus; the delay a later round), 14. Later rounds: a second oscillator beside the seven saws (lack 49).

## Rows

New rows join at the tail; no standing row moves.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Tune | ct | -200 | 200 | 0 | 0 | | |
| 1 | Bend | st | -12 | 12 | 0 | 0 | | |
| 2 | Detune | | 0 | 1 | 0.5 | 0 | | |
| 3 | Mix | | 0 | 1 | 0.6 | 0 | | |
| 4 | Cutoff | Hz | 20 | 20000 | 6000 | 0 | | |
| 5 | Resonance | | 0 | 1 | 0.1 | 0 | | |
| 6 | Envelope | oct | 0 | 5 | 1 | 0 | | |
| 7 | Keyboard | | 0 | 1 | 0.5 | 0 | | |
| 8 | Attack1 | s | 0.001 | 10 | 0.005 | 0 | | |
| 9 | Decay1 | s | 0.001 | 10 | 0.6 | 0 | | |
| 10 | Sustain1 | | 0 | 1 | 0.4 | 0 | | |
| 11 | Release1 | s | 0.001 | 10 | 0.4 | 0 | | |
| 12 | Attack2 | s | 0.001 | 10 | 0.005 | 0 | | |
| 13 | Decay2 | s | 0.001 | 10 | 0.3 | 0 | | |
| 14 | Sustain2 | | 0 | 1 | 0.8 | 0 | | |
| 15 | Release2 | s | 0.001 | 10 | 0.4 | 0 | | |
| 16 | Unison | | 1 | 4 | 1 | 3 | 1, 2, 3, 4 | |
| 17 | Spread | ct | 0 | 50 | 10 | 0 | | |
| 18 | Width | | 0 | 1 | 0.5 | 0 | | |
| 19 | Glide | s | 0 | 3 | 0 | 0 | | |
| 20 | Volume | | 0 | 1 | 0.5 | 0 | | |
| 21 | Rate | Hz | 0.05 | 20 | 5 | 0 | | new |
| 22 | Wave | | 0 | 3 | 0 | 3 | Sine, Triangle, Saw, Square | new |
| 23 | Vibrato | st | 0 | 12 | 0 | 0 | | new |
| 24 | Wobble | oct | 0 | 3 | 0 | 0 | | new |
| 25 | Mode | | 0 | 3 | 0 | 3 | 24dB, 12dB, Band, High | new |
| 26 | Velocity | | 0 | 1 | 0.5 | 0 | | new |
| 27 | Touch | oct | 0 | 3 | 1 | 0 | | new |
| 28 | Chorus | | 0 | 1 | 0.3 | 0 | | new |

Chain: the dance lead's public path (seven detuned saws with the detune curve and side mix, a high-pass rising with detune, the 24 dB low-pass, two ADSRs, unison) plus the JP-style chain's LFO and filter modes. Vibrato and Wobble rest at 0 (the LFO idle, as every competitor rests); Mode 24dB is today's filter. Chorus is the wet of a fixed stereo chorus (0.5 Hz, 2.5 ms — ours).

## Chain

| Stage | Rides |
|---|---|
| allocation, eight voices; unison stacks with Spread and Width; Glide | `core/voice` |
| seven saws, scattered phases; the high-pass at hertz × Detune | `core/oscillator` `Super`, `core/filter` `Biquad` |
| one LFO of Wave at Rate: Vibrato semitones on pitch, Wobble octaves on cutoff | `core/modulator` `Lfo` |
| filter by Mode through a table: 24dB → `Ladder`; 12dB, Band, High → `Variable` `LOW`, `BAND`, `HIGH`; cutoff × Keyboard × Envelope × Wobble·LFO × Touch·velocity, settled once per stride (today's 16-frame `clock` replaced) | `core/filter`; NEEDS core/block — `Stride` (see acid) |
| filter and loudness ADSRs; loudness depth = Velocity | `core/envelope` |
| the pair (pan by unison place) plus the chorus of the mono sum | NEEDS core/line — `Chorus` (see dco) |
| Volume | — |

## Surface

- ins: NOTES, CONTROL; outs: AUDIO. A source: no AUDIO in; left, right, mono sum as today.
- voicing: `poly`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: 1 tree, 2 includes, 3 `SOUND::PLUGINS::SUPERSAW`, 4 the sheet (rows appended to `supersaw.indices.hpp` and `ROWS`), 5 names, 6 honesty, 7 cores (no own counter, no own chorus).
- 381: 1, 2 (`.type = "instrument"`, `.voicing = SOUND::PLUGIN::POLY`), 3 instruments/poly, 4 no AUDIO in.
- Files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Mode and Wave pick from tables; no magic literals.

## Gate

- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh <SNAPSHOT>` green (lane a: `/home/idless/Repositories/worktrees/sound-lane-a`).
- `nm -DC <SNAPSHOT>/posix/libsound.so | grep -ci 'SOUND::PLUGINS::SUPERSAW::'` non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` green in the worktree.
- Heard: silent before a note, a chord sounds; Wobble 1 sweeps the cutoff at Rate; Mode High and Band audibly differ from 24dB; Chorus widens the stack; hard notes louder and brighter.

## Learned

| Method | Source | Licence |
|---|---|---|
| LFOs and filter modes in the JP-style chain | the V Collection's Jup-8000 V; Hive 2 (manuals) | commercial manuals |
| TPT state-variable taps | stmlib `dsp/filter.h` | MIT |
| bucket-brigade chorus as swept delay reads | Holters & Parker DAFx-18 | copyright (method only) |
| coefficients at control rate | stmlib `dsp/parameter_interpolator.h` | MIT |

Cores: core/block, core/line
