// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void step(
  RESONATOR::Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float *rows = module.rows;
  const Float swing =
    CORE::MODULATOR::tick(module.lfo) * rows[RESONATOR::SWEEP];
  if (!CORE::BLOCK::due(module.stride, RESONATOR::STRIDE)) return;
  const Whole channels = module.channels;
  for (Whole channel = 0; channel < channels; ++channel) {
    const Float cv =
      CORE::BLOCK::lane(lanes, RESONATOR::CV, channel, channels)[frame];
    RESONATOR::sweep(module, channel, swing, cv);
  }
}

auto filtered(const RESONATOR::Model &model, RESONATOR::Stage &stage, Float in)
  -> Float {
  const CORE::SHAPER::Pair pair = CORE::SHAPER::up(stage.oversampler, in);
  const CORE::SHAPER::Pair out = {
    model.tick(stage, pair.early), model.tick(stage, pair.late)};
  return CORE::SHAPER::down(stage.oversampler, out);
}

}  // namespace

void SOUND::PLUGINS::RESONATOR::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &module = *static_cast<Module *>(instance);
  const auto applied = [&module](const AUDIO::PLUGIN::Event &event) {
    apply(module, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, applied);
    ::step(module, lanes, frame);
    const RESONATOR::Model &model = RESONATOR::model(module);
    for (Whole channel = 0; channel < module.channels; ++channel) {
      Float &sample = CORE::BLOCK::lane(
        lanes, RESONATOR::IN, channel, module.channels)[frame];
      sample = ::filtered(model, module.stages[channel], sample);
    }
    const Float size = CORE::BLOCK::loudest(lanes, module.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
