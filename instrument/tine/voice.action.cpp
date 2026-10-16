// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float BASE = 4;

}  // namespace

auto SOUND::PLUGINS::TINE::curve(const Synth &synth, Float velocity) -> Float {
  return std::pow(velocity, std::pow(::BASE, -synth.rows[CURVE]));
}
