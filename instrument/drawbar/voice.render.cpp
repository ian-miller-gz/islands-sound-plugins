// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole MONO = 1;
constexpr Whole SIDES = 2;
constexpr Float HALF = 0.5f;

auto side(const DRAWBAR::Pair &pair, Whole channels, Whole channel) -> Float {
  if (channels == MONO) return HALF * (pair.left + pair.right);
  return channel % SIDES == 0 ? pair.left : pair.right;
}

}  // namespace

void SOUND::PLUGINS::DRAWBAR::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &organ = *static_cast<Organ *>(instance);
  const auto land = [&organ](const AUDIO::PLUGIN::Event &event) {
    apply(organ, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, land);
    const Pair pair = sound(organ);
    const Float volume = organ.rows[VOLUME];
    for (Whole channel = 0; channel < organ.channels; ++channel)
      lanes[channel][frame] = volume * ::side(pair, organ.channels, channel);
    const Float size = CORE::BLOCK::loudest(lanes, organ.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, land);
  CORE::BLOCK::publish(organ.meter, CORE::BLOCK::clipped(peak));
}
