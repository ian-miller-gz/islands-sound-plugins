# oscillator — round 1

Gains three ins: `pitch` (exponential), `fm` (linear) and `sync` (hard sync). It also gains a Detune row for Super, apart from Width, and a Play row that lets it drone without notes. These answer matrix lacks 15 and 43 (oscillator's lines), 40 (sync, through `CORE::OSCILLATOR::Sync`, which stands unused by the module) and 119 (the separate Detune). Later rounds: through-zero FM, a wave-morph row and a sub out (119, 120), width CV, microtuning (7) and polyphonic cables (S3).

## Rows

No standing row moves; new rows join at the tail.

| # | Row | Unit | Least | Most | Rest | Steps | Words | |
|---|---|---|---|---|---|---|---|---|
| 0 | Wave | | 0 | 4 | 0 | 4 | Saw, Pulse, Triangle, Sine, Super | |
| 1 | Width | | 0.05 | 0.95 | 0.5 | 0 | | |
| 2 | Coarse | st | -24 | 24 | 0 | 48 | | |
| 3 | Fine | ct | -100 | 100 | 0 | 0 | | |
| 4 | Glide | s | 0 | 2 | 0 | 0 | | |
| 5 | Gain | | 0 | 1 | 0.5 | 0 | | |
| 6 | Detune | | 0 | 1 | 0.5 | 0 | | new |
| 7 | Pitch | st | -48 | 48 | 12 | 0 | | new |
| 8 | Index | | 0 | 4 | 1 | 0 | | new |
| 9 | Play | | 0 | 1 | 0 | 1 | Notes, Free | new |

Chain: the eurorack VCO, using the public convention of one octave per unit of pitch CV (1 V/oct; 381 law 5's unit interval makes 1 the octave). Values chosen are ours:
- Pitch is the semitones per unit of `pitch`. Its rest of 12 is that convention.
- Index is the linear FM depth: hertz × (1 + Index × fm), held at ≥ 0. Through-zero FM is a later round.
- Detune is Super's spread. Its rest of 0.5 equals today's, where Width did both jobs. Width is now the pulse width alone.
- Play Free sounds from create at key 60 + Coarse + Fine + Pitch × cv; notes still set the key. Play Notes is today's: silent until the first note.
- A rise on `sync` resets the phase at the sub-sample place, for Saw, Pulse and Triangle. Sine resets plainly; Super ignores `sync`.

## Chain

| Stage | Rides |
|---|---|
| `pitch` (in 0), `fm` (in 1) and `sync` (in 2) read at their law-4 lanes; lane 0 drives the voice (one voice) | NEEDS core/block — `lane` (see design/modular/envelope.md) |
| key: notes through the mono legato allocator with glide, or 60 under Free | `core/voice` (stands) |
| pitch = key + Coarse + Fine/100 + Pitch × cv; hertz × (1 + Index × fm) ≥ 0; re-tuned on any change | `core/phase` `hertz` (stands) |
| sync edge with its sub-sample place | NEEDS core/block — `Edge` (see design/modular/envelope.md) |
| the wave by the standing table of five; Saw, Pulse and Triangle tick through a `Sync` slave | NEEDS core/oscillator — `Sync` driven by an edge: `tick(sync, since, wave)`, where `since` is the part of the frame after the crossing, or none. With no edge it equals `tick(oscillator, wave)`. The standing `tick(sync, master, wave)` computes its `since` from the master and calls it, so every rider keeps its sound |
| Super at Detune; Sine from the table | `core/oscillator` `Super`, `Table` (stand) |
| × Gain written into every lane of in 0; meter over the sample | `core/block` |

## Surface

- ins: AUDIO `pitch`, AUDIO `fm`, AUDIO `sync`, NOTES, CONTROL; outs: AUDIO. It reads its three ins before it writes in 0's lanes. Unwired ins read 0, so notes alone play it as today.
- voicing: `mono`. Meter: `CORE::BLOCK::Meter`.

## Laws

- 379: law 1, law 2, law 3 (`SOUND::PLUGINS::OSCILLATOR`), law 4, law 5, law 6 (three AUDIO ins declared and read), law 7 (the edge-driven sync is core/oscillator's).
- 381: law 1; law 2 (`.type = "modular"`, `.voicing = SOUND::PLUGIN::MONO`); law 3; law 4 (three lane sets in port order); law 5.
- Size: `voice.render.cpp` stands at 69 lines. The in reading and the pitch sum go to a new `voice.ins.cpp`. `oscillator.hpp` moves from `sizeof` counts to `std::size`.
- Code rules: files ≤ 100 lines, functions ≤ 25; no comments; no natural-language strings; Play by a table; no magic literals (60 named).

## Gate

The build waits until the library shift has landed `lane`, `Edge` and the edge-driven `Sync`.
- `cd /home/idless/Repositories/mine/islands-private && cartridges/build.sh /home/idless/Repositories/worktrees/sound-lane-a` is green.
- `nm -DC /home/idless/Repositories/worktrees/sound-lane-a/posix/libsound.so | grep -ci 'SOUND::PLUGINS::OSCILLATOR::'` is non-zero.
- `/home/idless/Repositories/mine/islands-private/.venv/bin/python3 -m pytest tests` is green in the worktree.
- Heard:
  - silent before the note, then a saw on the note;
  - an lfo into `pitch` gives a vibrato of ±1 octave at Pitch 12;
  - a second oscillator into `fm` at Index 2 gives a metallic tone;
  - a second, lower oscillator into `sync` gives the tearing sync sweep as Coarse rises;
  - Detune 1 on Super gives a wide chorus while Width moves only the pulse;
  - Play Free drones with no note.

## Learned

| Method | Source | Licence |
|---|---|---|
| hard sync at the fractional reset with a BLEP and BLAMP | Mutable `plaits/dsp/oscillator/variable_shape_oscillator.h`; stmlib `dsp/polyblep.h` | MIT |
| exponential pitch CV beside linear FM | VCV Fundamental `src/VCO.cpp`; Cherry Audio Voltage Modular oscillator doc | GPL-3.0+ (method only); commercial doc |
| through-zero linear FM with the BLEP direction following (read; a later round) | Bogaudio `src/dsp/oscillator.cpp`; Befaco `src/PonyVCO.cpp` | GPL-3.0+ (method only) |
| a separate detune for a stacked saw | Surge XT `ClassicOscillator.cpp`; Vital `synth_oscillator.cpp` | GPL-3.0 (method only) |

Cores: core/block
