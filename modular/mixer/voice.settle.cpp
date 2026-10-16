// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Shape = auto (*)(Float level) -> Float;

constexpr Float UNITY = 1.0f;
constexpr Float SPAN = 60.0f;

auto linear(Float level) -> Float { return level; }

auto logarithmic(Float level) -> Float {
  const Float size = CORE::BLOCK::magnitude(level);
  return level * CORE::DYNAMICS::gain(SPAN * (size - UNITY));
}

constexpr Shape SHAPES[] = {linear, logarithmic};
static_assert(std::size(SHAPES) == MIXER::TAPERS);

auto level(const MIXER::Module &module, Whole in) -> Float {
  const Float *rows = module.rows;
  const Whole taper = Whole(rows[MIXER::TAPER]);
  const Shape shape = SHAPES[taper < MIXER::TAPERS ? taper : MIXER::LINEAR];
  return shape(rows[MIXER::LEVEL + in]) * (UNITY - rows[MIXER::MUTE + in]);
}

}  // namespace

void SOUND::PLUGINS::MIXER::settle(Module &module) {
  const Float *rows = module.rows;
  const Flag panned = module.channels == STEREO;
  for (Whole in = 0; in < INS; ++in) {
    const Float level = ::level(module, in) * rows[GAIN];
    const auto gains = CORE::STEREO::pan(panned ? rows[PAN + in] : 0);
    const Float sides[STEREO] = {gains.left, gains.right};
    for (Whole side = 0; side < STEREO; ++side)
      module.weights[in][side] = level * sides[side];
  }
}

void SOUND::PLUGINS::MIXER::prime(Module &module) {
  for (Whole in = 0; in < INS; ++in)
    for (Whole side = 0; side < STEREO; ++side) {
      CORE::MODULATOR::Smoother &smoother = module.smoothers[in][side];
      smoother.time = GLIDE;
      CORE::MODULATOR::settle(smoother, module.rate);
      CORE::MODULATOR::jump(smoother, module.weights[in][side]);
    }
}

void SOUND::PLUGINS::MIXER::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}
