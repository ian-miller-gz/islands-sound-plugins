// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto pass(NOISE::Source &source, NOISE::Colour colour) -> Float {
  return colour(source);
}

auto hold(NOISE::Source &source, NOISE::Colour colour) -> Float {
  return CORE::NOISE::tick(source.hold, colour(source));
}

auto walk(NOISE::Source &source, NOISE::Colour) -> Float {
  return CORE::NOISE::tick(source.walk);
}

constexpr NOISE::Mode MODES[] = {pass, hold, walk};
static_assert(std::size(MODES) == std::size(NOISE::MODES));

}  // namespace

auto SOUND::PLUGINS::NOISE::mode(const Module &module) -> Mode {
  const Whole chosen = Whole(module.rows[MODE]);
  return ::MODES[chosen < std::size(::MODES) ? chosen : PASS];
}
