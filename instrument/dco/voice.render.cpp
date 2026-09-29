// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Whole MONO = 1;
constexpr Whole SIDES = 2;

auto side(const DCO::Pair &pair, Whole channels, Whole channel) -> Float {
  if (channels == MONO) return pair.whole;
  return channel % SIDES == 0 ? pair.left : pair.right;
}

}  // namespace

void SOUND::DCO::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &synth = *static_cast<Synth *>(instance);
  const auto land = [&synth](const AUDIO::PLUGIN::Event &event) {
    apply(synth, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, land);
    const Pair pair = sound(synth);
    const Float volume = synth.rows[VOLUME];
    for (Whole channel = 0; channel < synth.channels; ++channel)
      lanes[channel][frame] = volume * ::side(pair, synth.channels, channel);
    const Float size = CORE::BLOCK::loudest(lanes, synth.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, land);
  CORE::BLOCK::publish(synth.meter, CORE::BLOCK::clipped(peak));
}
