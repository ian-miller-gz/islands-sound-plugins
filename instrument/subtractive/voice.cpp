// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<SUBTRACTIVE::Synth, SUBTRACTIVE::SHEET>;

constexpr Float TAU = 6.2831853f;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new SUBTRACTIVE::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(SUBTRACTIVE::SHEET, synth->rows);
  CORE::PHASE::tune(synth->tuning, rate);
  synth->turn = TAU / Float(rate);
  SUBTRACTIVE::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  auto &synth = *static_cast<SUBTRACTIVE::Synth *>(instance);
  return synth.level[synth.face.load(std::memory_order_acquire)];
}

void destroy(void *instance) {
  delete static_cast<SUBTRACTIVE::Synth *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = SUBTRACTIVE::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "subtractive",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace

auto SOUND::PLUGINS::SUBTRACTIVE::delta(Float span, Whole rate) -> Float {
  const Float frames = span * Float(rate);
  return frames <= 1.0f ? 1.0f : 1.0f / frames;
}

void SOUND::PLUGINS::SUBTRACTIVE::shape(Envelope &envelope, Whole rate) {
  envelope.rise = delta(envelope.attack, rate);
  envelope.fall = delta(envelope.decay, rate) * (1.0f - envelope.sustain);
  envelope.drop = delta(envelope.release, rate);
}
