# review — round 1, lane a

## Verdicts

- acid: adequate — rows, chain, surface and cores as design/subtractive/acid.md states; the bassline gain table is an own derivation; heard
- duo: adequate with fixes — rows, chain and the duo voicing honoured; the drift salt renamed off its row index here; the 24 dB slope's drive level stands uncompensated beside the 12 dB slope
- ladder: adequate — rows, chain, drift and transistor filter as stated; stride reset on strike and on a row move; heard
- multimode: adequate — all 39 rows, the saturating variable filter at 2x, trim and drift as stated; heard
- pluck: adequate with fixes — rows and chain honoured; the loop's shortest delay read the Decay row's least through a name shadow, qualified here
- subtractive: adequate — re-founded on the cores with every standing row kept; nothing of the four deleted units lost; heard
- chip: adequate — all 29 rows, the edge-driven channels, the sweep table and the glide as stated; heard clean
- operator: adequate — all 132 rows, the mode table, scaling and pitch walk ride the cores; the routing chart is in own form; heard
- additive: adequate — every formula of the chain paragraph implemented as written over the cores; heard
- clap: adequate — rows, bursts, scatter and the tail band as stated; heard
- conga: adequate — rows, strokes by note, the two modes and the slap high-pass as stated; heard
- cymbal: adequate with fixes — rows, three plates, three bands and the choke as stated; the core unit's noise and gain constants renamed off the family and filter words here
- kick: adequate — all 16 rows, the click kinds as a table, sigh and shelves as stated; heard
- machine: adequate — all 34 rows, the general map, the cymbal loop and the pans as stated; heard
- rhythm: adequate — sixteen patterns with accent and new voice lanes, swing, timing and dynamics as stated; plays at once with Run resting on
- tom: adequate — rows, three heights, the modes and the key path as stated; heard
- choir: back to build — the host stalls with choir seated: the export never returns and quit is never read while whisper and clap finish in a second; the one render that landed peaked near −29 dBFS
- whisper: adequate — rows, the shared bank, makeup and the stereo spread as stated; heard at full scale at rest
- envelope: adequate — all 15 rows, the gate and cv lanes, delay, hold and loop as stated; the cv heard rising after the note
- mixer: adequate with fixes — rows, lanes and pan as stated; the taper is re-rolled in the module where the requirement assigns it to core/dynamics
- oscillator: adequate — all 10 rows, the pitch, fm and sync lanes and the edge sync as stated; heard
- shaper: adequate — rows, the six curves, the integral paths and the quality table as stated; heard

## Guard

- `python3 -I language.py --sheets <WORKTREE>`: clean.
- `python3 -I language.py --sheets --base stable <WORKTREE>`: clean; every commit message `stable..HEAD` is the branch name.
- Hits fixed: none. Hits inherited: none.

## Heard

Recipe, per instrument, through `/home/idless/Repositories/worktrees/islands-private-uat/build/Island` with `SDL_AUDIO_DRIVER=absent`, flags `--frames 0 --width 1280 --height 720 --overlay 0 --visible 0 --window console=0 --midi none --cartridge /home/idless/Repositories/worktrees/sound-lane-a --listen <SOCKET>`; words by `tests/paced.py`: `stock riff`, `author 0 <PITCH> 480 1920 0.8` (the note at tick 480 = frame 12000, one beat long), `seat <PLUGIN> 0`, `wire root 1 <PLUGIN> <NOTES IN>`, `wire <PLUGIN> 0 master 0`, `clip 0 1 0 0 72000`, `export 0 <PATH>`, `quit`. The wav read by a peaks script: 16-bit peak over frames 0..12000 (before) and 12000..72000 (after). Renders under `logs/crew/notes/f1review1/`. Pitch 60 unless stated.

- acid: 72000 frames, peak 13861, silent before the note
- duo: 72000 frames, peak 9329, silent before
- ladder: 72000 frames, peak 12291, silent before
- multimode: 72000 frames, peak 19295, silent before
- pluck: 72000 frames, peak 11128, silent before
- subtractive: 72000 frames, peak 9625, silent before
- chip: 72000 frames, peak 4561, silent before
- operator: 72000 frames, peak 3627, silent before
- additive: 72000 frames, peak 4286, silent before
- clap: 72000 frames, peak 16026, silent before
- conga (pitch 63): 72000 frames, peak 16570, silent before
- cymbal (pitch 49): 72000 frames, peak 26213, silent before
- kick (pitch 36): 72000 frames, peak 25854, silent before
- machine (pitch 36): 72000 frames, peak 22827, silent before
- rhythm: 72000 frames, peak 24543, not silent before — Run rests on, the box plays from frame 0 as the requirement states
- tom (pitch 45): 72000 frames, peak 17120, silent before
- choir: first run 72000 frames, peak 1183, silent before, then the host never read `quit` (killed after 14 minutes); second run no export within 30 s, an 8 KB partial wav, host killed at 45 s
- whisper: 72000 frames, peak 32727, silent before
- envelope (notes in 2, cv out to master): 72000 frames, peak 29490, silent before
- oscillator (notes in 3): 72000 frames, peak 16205, silent before, still sounding at the tail
- mixer (oscillator notes in 3 → mixer in 0): 72000 frames, peak 16205, silent before
- shaper (oscillator → shaper in 0): 72000 frames, peak 32767, silent before, full scale under the resting 12 dB drive
- The digest route (`note root 1 0 60 0.8 2400`, `bounce 2400`, no clip placed) answered nothing within 70 s for whisper and for choir alike; not used.

## Findings

- choir: the stall is not located; `voice.render.cpp`, `voice.steer.cpp` and `voice.sing.cpp` show no unbounded loop; a per-block cost or a stall in the core line, reverb or tract path is the next build's first question.
- whisper: a velocity 0.8 note at rest reaches 32727 of 32767; the makeup gain leaves no headroom.
- choir: the one landed render peaked at 1183 (about −29 dBFS) against 4000–26000 for the rest of the lane.
- duo `voice.filter.cpp:30,38`: the 12 dB tick is level-compensated by `/ gain`, the 24 dB tick is not; with Drive on the slopes differ by up to 9.5 dB.
- duo, acid, multimode `voice.notes.cpp`: a strike does not reset the stride; ladder does.
- mixer `voice.settle.cpp:15-18`: the taper named for `core/dynamics` by design/modular/mixer.md and amplifier.md is written in the module; no `taper` stands in core/dynamics.
- mixer `voice.render.cpp:9-11`: lanes beyond two take the left weight where design/core/stereo.md names `pour`'s mean.
- shaper `voice.paths.cpp:34`: the Chebyshev path hard-clips before the polynomial; not in the requirement.
- subtractive `voice.settle.cpp:10,14`: `WAVES[]` and `KINDS[]` share their spelling with the counts `SUBTRACTIVE::WAVES`/`KINDS`; correct only through `::`.
- clap: `CORE::PERCUSSION::BURSTS` (`percussion.clap.hpp:7`) and the row index `CLAP::BURSTS` are both 4 by coincidence.
- conga `voice.notes.cpp:14`: `enum { LOW, MID, HIGH, MUTE }` re-spells `HEIGHT::LOW/MID/HIGH` with another meaning; `:26` uses `std::exp2` where `CORE::PHASE::ratio` serves.
- chip `voice.mixer.cpp:12-13,22-23`: the mixer fit constants have no derivation in the design; `voice.channels.hpp:41` `Wave::muted` is written and never read.
- operator `voice.settle.cpp:10`: `PI` restated beside the core's; a Fixed operator still takes Tune and Detune (the design lists what Fixed ignores without them).
- additive `voice.notes.cpp:27`: a TIED note re-strikes both envelopes and zeroes all partial phases.
- rhythm `voice.steps.cpp:6`: a second using-directive (`CORE::PERCUSSION`) in the unit; `voice.patterns.straight.cpp:14`: Rock's hat lane moved from eighths to sixteenths, not asked by the requirement.
- envelope `voice.settle.cpp:15-18`: a legato tie to an idle envelope skips Delay and keeps the previous velocity.
- core/filter `filter.transistor.cpp:31-33`: feedback is `RING × series × 1.05 × emphasis` where design/core/filter.md states `RING × emphasis`; `filter.formant.cpp:29`: a `breadth` set before the first `jump` divides 0/0.
- core/percussion: `TOP`, `DIPOLE`, `QUADRUPOLE`, `OVERTONES` restated in `percussion.tom.cpp`, `percussion.conga.cpp`, `percussion.tom.strike.cpp`; `BASE = 4` twice; `sing(Tom&)` twins `sing(Conga&)`; `percussion.play.hpp` restates `STEREO::magnitude`; `percussion.hybrid.loop.cpp:83` wraps by one subtraction.
- core: four file-local three-argument clamps (`block.control.cpp`, `modulator.drift.cpp`, `line.chorus.cpp`, `percussion.tom.cpp`) and `PI`/`TURN` restated in four units beside `std::numbers`; a public `BLOCK::held` is a design question for the next library round.
- core/filter `filter.tract.cpp` stands at 100 lines; `voice.hpp` at 95; `percussion.kick.click.cpp` 97.
- Recipe: the bounce route with a developed note and no clip answers nothing; the wav route needs the module's NOTES in index (envelope 2, oscillator 3).
