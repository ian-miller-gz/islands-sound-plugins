// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "sampler.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::SAMPLER {

constexpr Whole STRIDE = 16;
constexpr Whole ROOTS = 64;
static_assert(POLYPHONY == CORE::VOICE::VOICES);

struct Clip {
  String name;
  Vector<Vector<Float>> lanes;
  Float ratio = 1;
};

struct Region {
  Integer origin = 0;
  Integer sign = 1;
  Float length = 0;
  Integer start = 0;
  Integer end = 0;
  Float fade = 0;
  Whole walk = 0;
  Flag wraps = false;
};

struct Blend {
  Float near = 1;
  Float far = 0;
};

struct Voice {
  Whole clip = 0;
  Float along = 0;
  Flag turned = false;
  Float step = 0;
  Float level = 0;
  Flag held = false;
  Whole reader = 0;
  Region region;
  CORE::ENVELOPE::Gate loudness;
  CORE::ENVELOPE::Gate contour;
  Vector<CORE::SHAPER::Oversampler> downs;
  Vector<CORE::FILTER::Variable> filters;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Whole pace = 1;
  Float rows[PARAMETERS] = {};
  Vector<Clip> clips;
  CORE::VOICE::Allocator allocator = {.steal = CORE::VOICE::QUIETEST};
  Voice voices[POLYPHONY];
  CORE::ENVELOPE::Envelope loudness;
  CORE::ENVELOPE::Envelope contour;
  CORE::MODULATOR::Lfo lfo;
  Float swing = 0;
  Float roots[ROOTS + 1] = {};
  CORE::BLOCK::Stride stride;
  CORE::BLOCK::Meter meter;
};

using Surface = CORE::TABLE::Surface<Synth, SHEET>;

void take(Synth &synth);
void tone(Synth &synth);
auto frames(const Clip &clip) -> Whole;

auto picked(const Synth &synth, Float value) -> Float;
auto reading(void *instance, Whole index) -> String;
auto control(void *instance, Whole index, AUDIO::PLUGIN::Control &out) -> Flag;

void settle(Synth &synth);
void steer(Synth &synth, Whole at);
auto base(const Synth &synth, const Voice &voice, Float key) -> Float;
void land(Synth &synth, Whole row, Float value);
void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto region(const Synth &synth, const Clip &clip, Flag sustained) -> Region;
auto walk(Voice &voice) -> Flag;
void prepare(Synth &synth);
auto blend(const Synth &synth, const Region &region, Float along) -> Blend;
auto reader(const Synth &synth, Float step) -> Whole;
auto read(const Synth &synth, Voice &voice, Whole lane) -> Float;
void shade(Synth &synth, Whole at, Float key, Float contour);
auto filtered(const Synth &synth, Voice &voice, Whole lane, Float in) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::SAMPLER
