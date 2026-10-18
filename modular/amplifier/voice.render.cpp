// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void gate(
  AMPLIFIER::Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame,
  Flag due) {
  const Float *rows = module.rows;
  const Whole channels = module.channels;
  for (Whole channel = 0; channel < channels; ++channel) {
    auto *in = CORE::BLOCK::lane(lanes, AMPLIFIER::IN, channel, channels);
    const Float cv =
      CORE::BLOCK::lane(lanes, AMPLIFIER::CV, channel, channels)[frame];
    const Float control = rows[AMPLIFIER::OFFSET] + rows[AMPLIFIER::DEPTH] * cv;
    in[frame] = AMPLIFIER::amplify(module, {channel, control, in[frame], due});
  }
}

}  // namespace

void SOUND::PLUGINS::AMPLIFIER::render(
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
    const Flag due = CORE::BLOCK::due(module.stride, STRIDE);
    ::gate(module, lanes, frame, due);
    const Float size = CORE::BLOCK::loudest(lanes, module.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
