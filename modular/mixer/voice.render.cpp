// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto side(Whole channel) -> Whole {
  return channel < MIXER::STEREO ? channel : 0;
}

void mix(
  const MIXER::Module &module, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) {
  const Whole channels = module.channels;
  for (Whole channel = 0; channel < channels; ++channel) {
    const Whole at = ::side(channel);
    Float sum = 0;
    for (Whole in = 0; in < MIXER::INS; ++in)
      sum += lanes[in * channels + channel][frame] * module.weights[in][at];
    lanes[channel][frame] = sum;
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
    ::mix(module, lanes, frame);
    const Float size = CORE::BLOCK::loudest(lanes, module.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
