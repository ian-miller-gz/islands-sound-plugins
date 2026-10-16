# review — round 1, lane b

## Verdicts
- dco: adequate — rows 26–31 as `## Rows`; Assign by a table with a release on change, Glide, Velocity, Touch, Drift and Trim per slot, the Transistor at 2x per stride, the chorus lifted to core/line; POLY, meter, no AUDIO in.
- mono: adequate — rows 27–29 as `## Rows`; Velocity depth, Touch, Drift on pitch and cutoff, the Transistor at 2x per stride; MONO, meter.
- ensemble: adequate — rows 13–15 as `## Rows`; the envelope mode a table of two key reads, velocity weighed, the three-band resonator before the ensemble; POLY, meter.
- pads: adequate — six Pan rows at the tail as stated, equal-power pan per pad over core/stereo, soft clip per side; POLY, meter over the loudest lane.
- polymod: adequate — rows 45–49 as `## Rows`; Trim and Drift per slot, Velocity, Touch, the core chorus at 0.5 Hz and 2.5 ms, the stereo side rule; POLY, meter.
- supersaw: adequate — rows 21–28 as `## Rows`; the LFO on pitch and cutoff, Mode by a table of Ladder and Variable, Velocity, Touch, the core chorus, Stride in place of the counter; POLY, meter.
- drawbar: adequate — rows 20–24 as `## Rows`; contacts drawn per press, the leak table at −40 dB, the 3 dB load, the sagging preamp at 2x, the four-line room; the heaviest render of the lane (see Findings); POLY, meter.
- tine: adequate — rows 16–22 as `## Rows`; Taper, Curve, Touch low-pass, hammer and damper bursts, Hold through the core's flag, Width by key over core/stereo; POLY, meter.
- wavetable: adequate — rows 29–36 as `## Rows`; the levelled tables and blend in core/oscillator with a harness and test_levels.py, unison through the allocator at 1/√N, the four warps and the filter modes by tables, Touch and Push; POLY, meter.
- clave: adequate — rows 3–8 as `## Rows`; the two bar modes at the beam ratios, the rim shift, the crack's cutoff, Curve and Touch, Key play; the standing settle keeps kit's sound; MONO, meter.
- cowbell: adequate — rows 3–8 as `## Rows`; the pair's interval, the band's Tone, Clank weighed by Touch, Curve, Key play; MONO, meter.
- hat: adequate — rows 5–9 as `## Rows`; CLOSED, PEDAL, OPEN by a key table, the ring pairs, the noise share, the 5 ms choke fade, Touch on the band; MONO, meter.
- kit: adequate — rows 19–43 as `## Rows`; Decay, Tone and Pan blocks, Curve through the accent, sounding tests per drum, a stereo mix over core/stereo; POLY, meter over the loudest side.
- percussive: adequate — re-founded on core/table and core/block; 33 rows as `## Rows`; five pads from the recipes' rests, Curve, the choke fade; POLY, meter.
- snare: adequate — rows 5–10 as `## Rows`; Bend and Drop sweeps, the Hold, the five-mode Body, the three strokes by a key table with the named third, ratio and +3 dB; MONO, meter.
- sampler: adequate — re-founded on the cores; 35 rows as `## Rows` with Stock widened to the clips; the Tone clip, the four walkers, the fade, the three readers with the 2x path past a step of one, the filter and contour, the LFO, Mode and Voices; POLY, meter.
- vowel: adequate — 22 rows as `## Rows` with Tension in place of Open; Voice, Target and Motion through the core tract, Effort, Drift as two walks per throat, Mode by a table, Width over core/stereo, control per stride; POLY, meter.
- amplifier: adequate — Response and Mode as `## Rows`; the three modes by a table, the taper, the low-pass gate per stride; in 0 and `cv` read before the write; modular, no voicing, meter.
- lfo: adequate — Skew, Smooth, Range and Amount as `## Rows`; `sync` and `cv` read, Range by a table of two factors, the skew warp in core/modulator; modular, meter.
- noise: adequate — Violet and Velvet, Mode, Rate and Amount as `## Rows`; `cv` read, the colours and modes by tables, Velvet and Walk in core/noise, std::size counts; modular, meter.
- resonator: adequate — Model and Drive as `## Rows`; the four models by a table at 2x in the Oversampler, the cutoff per 4-frame stride from the LFO and `cv`; modular, meter.
- slew: adequate — Shape and Amount as `## Rows`; the shaped step in core/modulator, both times per sample from `cv`; modular, meter.

## Guard
- `language.py --sheets` over the worktree: clean. `--base factory/1-a`: clean; every commit message is the branch name.
- Hits fixed: none. Hits inherited: none.

## Heard
- Recipe: the UAT host `/home/idless/Repositories/worktrees/islands-private-uat/build/Island` with `SDL_AUDIO_DRIVER=absent --frames 0 --width 640 --height 480 --overlay 0 --visible 0 --window console=0 --midi none --cartridge /home/idless/Repositories/worktrees/sound-lane-b --listen <socket>`; words by tests/paced.py: `stock riff`, `author 0 <pitch> 960 960 0.8`, `seat <plugin> 0`, `wire root 1 <plugin> 0`, `wire <plugin> 0 master 0`, `clip 0 1 0 0 48000`, `export 0 <wav>`, `note root 1 12000 <pitch> 0.8 24000`, `bounce 48000`, `quit`; an effect takes an `oscillator` on the notes lane into in 0; a source is wired to master alone. The wav read for frames, peak and the first non-zero frame; the note lands at frame 24000. Renders under logs/crew/notes/f1review2/.
- dco (pitch 48): 48000 frames, peak 10897, silent before frame 24012; digest 3ecd2578.
- mono (48): 48000 frames, peak 9249, silent before 24000; digest e04d8f3d.
- ensemble (60): 48000 frames, peak 7524, silent before 24217; digest 4f63ef5f.
- pads (36): 48000 frames, peak 19155, silent before 24000; digest f82229a5.
- polymod (60): 48000 frames, peak 11641, silent before 24003; digest 118909f4.
- supersaw (60): 48000 frames, peak 9041, silent before 24001; digest 3ac4554d.
- drawbar (60): 48000 frames, peak 3752, silent before 24054; digest 080f5428 (the bounce answers only with PACED_FRAME=2).
- tine (60): 48000 frames, peak 3604, silent before 24000; digest 49d27b2d.
- wavetable (60): 48000 frames, peak 2980, silent before 24003; digest 8f18496e.
- clave (60): 48000 frames, peak 20878, silent before 24000; digest eae52bad.
- cowbell (60): 48000 frames, peak 24854, silent before 24001; digest ad015805.
- hat (42): 48000 frames, peak 17250, silent before 24000; digest 10e5f325.
- kit (36): 48000 frames, peak 21901, silent before 24000; digest 8a21f7ed.
- percussive (36): 48000 frames, peak 17788, silent before 24001; at 60 silent (no key there).
- snare (60): 48000 frames, peak 21030, silent before 24000; digest 3e3df875.
- sampler (60, the Tone clip): 48000 frames, peak 5505, silent before 24003; digest 237a1149.
- vowel (60): 48000 frames, peak 1850, silent before 24012; digest d6635105.
- amplifier (oscillator in 0): silent with `cv` unwired, as the requirement states; envelope → `cv`: peak 10295, silent before 24001; lfo → `cv` with the note at 0: peak 16278.
- lfo (source): 48000 frames, peak 32767 from frame 1, no note; digest 76b7435d.
- noise (source): 48000 frames, peak 8191 from frame 0, no note; digest 2fb43cfb.
- resonator (oscillator in 0, 48): 48000 frames, peak 18304, silent before 24010; digest f023644d.
- slew (oscillator in 0, 48): 48000 frames, peak 16293, silent before 24001; digest 09363f0d.

## Findings
- drawbar renders a second of audio in more than the walk client's 0.1 s reply window (a 48000-frame bounce answers only with PACED_FRAME=2; ten seconds take about 2 s more than tine's); 91 wheels, the room and the rotary tick every sample whatever the level. A cost for the demo's recipe and the next round, not a defect.
- A module's rows are turned through its CONTROL in wired from the root's control lane (`wire root 2 <plugin> <in>`, `turn root 2 0 <row> <value>`); the notes lane refuses the wire. The demo recipe should state it.
- hat's Touch most (3) is a bare literal where snare names `OCTAVES`; the lane's sheets keep bare range numbers as the standing sheets do.
- The ensemble design page carries no `Cores:` line.
- The snapshot's `oscillator` is the pre-round module; lane a's rebuild is not on this lane, so the effect renders used the old one.
