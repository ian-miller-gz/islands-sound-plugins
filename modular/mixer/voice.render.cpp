// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNITY = 1.0f;

auto side(Whole channel) -> Whole {
  return channel < MIXER::STEREO ? channel : 0;
}

void glide(MIXER::Module &module) {
  for (Whole in = 0; in < MIXER::INS; ++in)
    for (Whole side = 0; side < MIXER::STEREO; ++side)
      CORE::MODULATOR::tick(
        module.smoothers[in][side], module.weights[in][side]);
}

auto swell(const MIXER::Module &module, Float cv) -> Float {
  const Float scale = UNITY + module.rows[MIXER::AMOUNT] * cv;
  return scale > 0 ? scale : 0;
}

void mix(
  const MIXER::Module &module, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) {
  const Whole channels = module.channels;
  for (Whole channel = 0; channel < channels; ++channel) {
    const Whole at = ::side(channel);
    Float sum = 0;
    for (Whole in = 0; in < MIXER::INS; ++in)
      sum += CORE::BLOCK::lane(lanes, in, channel, channels)[frame] *
             module.smoothers[in][at].value;
    const Float cv =
      CORE::BLOCK::lane(lanes, MIXER::CV, channel, channels)[frame];
    CORE::BLOCK::lane(lanes, MIXER::OUT, channel, channels)[frame] =
      sum * ::swell(module, cv);
  }
}

}  // namespace

void SOUND::PLUGINS::MIXER::render(
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
    ::glide(module);
    ::mix(module, lanes, frame);
    const Float size = CORE::BLOCK::loudest(lanes, module.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
