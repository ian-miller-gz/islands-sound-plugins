// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto white(NOISE::Source &source) -> Float {
  return CORE::NOISE::tick(source.white);
}

auto pink(NOISE::Source &source) -> Float {
  return CORE::NOISE::tick(source.pink);
}

auto brown(NOISE::Source &source) -> Float {
  return CORE::NOISE::tick(source.brown);
}

auto blue(NOISE::Source &source) -> Float {
  return CORE::NOISE::tick(source.blue);
}

using Colour = auto (*)(NOISE::Source &) -> Float;

constexpr Colour COLOURS[] = {white, pink, brown, blue};

auto colour(const NOISE::Module &module) -> Colour {
  const Whole chosen = Whole(module.rows[NOISE::COLOUR]);
  return COLOURS[chosen < std::size(COLOURS) ? chosen : NOISE::WHITE];
}

}  // namespace

void SOUND::PLUGINS::NOISE::render(
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
    follow(module, lanes, frame);
    const Colour tick = ::colour(module);
    const Float level = CORE::MODULATOR::tick(module.level, module.target);
    for (Whole channel = 0; channel < module.channels; ++channel)
      CORE::BLOCK::lane(lanes, CV, channel, module.channels)[frame] =
        tick(module.sources[channel]) * level;
    const Float size = CORE::BLOCK::loudest(lanes, module.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
