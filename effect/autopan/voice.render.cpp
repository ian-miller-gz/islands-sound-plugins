// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

struct Gains {
  Float left;
  Float right;
};

auto pan(AUTOPAN::Effect &effect) -> Gains {
  const Float wave = CORE::MODULATOR::tick(effect.lfo);
  const Float place =
    CORE::MODULATOR::tick(effect.smoother, wave * effect.rows[AUTOPAN::DEPTH]);
  const Float angle = AUTOPAN::ARC * (AUTOPAN::UNITY + place);
  return {AUTOPAN::ROOT * std::cos(angle), AUTOPAN::ROOT * std::sin(angle)};
}

void play(
  AUTOPAN::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Gains gains = ::pan(effect);
  const Whole pairs = effect.channels / AUTOPAN::SIDES;
  for (Whole pair = 0; pair < pairs; ++pair) {
    lanes[pair * AUTOPAN::SIDES + AUTOPAN::LEFT][frame] *= gains.left;
    lanes[pair * AUTOPAN::SIDES + AUTOPAN::RIGHT][frame] *= gains.right;
  }
  const Float lone = (gains.left + gains.right) * AUTOPAN::HALF;
  for (Whole at = pairs * AUTOPAN::SIDES; at < effect.channels; ++at)
    lanes[at][frame] *= lone;
}

}  // namespace

void SOUND::PLUGINS::AUTOPAN::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &effect = *static_cast<Effect *>(instance);
  const auto steer = [&effect](const AUDIO::PLUGIN::Event &event) {
    apply(effect, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, steer);
    ::play(effect, lanes, frame);
    const Float loud = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (loud > peak) peak = loud;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
