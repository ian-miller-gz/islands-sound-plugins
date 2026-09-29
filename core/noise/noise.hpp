// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <cstdint>

#include "../../../plugin.hpp"
#include "../phase/phase.hpp"

namespace SOUND::PLUGINS::CORE::NOISE {

using Register = std::uint32_t;

constexpr Register SEED = 0x9E3779B9u;
constexpr Whole POLES = 6;

struct White {
  Register state = SEED;
};

struct Pink {
  White white;
  Float poles[POLES] = {};
  Float last = 0;
};

struct Brown {
  White white;
  Float level = 0;
};

struct Blue {
  Pink pink;
  Float last = 0;
};

struct Hold {
  PHASE::Wheel phase = 0;
  PHASE::Wheel step = 0;
  Float value = 0;
};

struct Burst {
  White white;
  Float level = 0;
  Float decay = 0;
  Float colour = 1;
  Float low = 0;
};

void seed(White &white, Register state);
void seed(Pink &pink, Register state);
void seed(Brown &brown, Register state);
void seed(Blue &blue, Register state);
void seed(Burst &burst, Register state);

auto tick(White &white) -> Float;
auto tick(Pink &pink) -> Float;
auto tick(Brown &brown) -> Float;
auto tick(Blue &blue) -> Float;

void settle(Hold &hold, Float hertz, Whole rate);
void reset(Hold &hold);
auto tick(Hold &hold, Float input) -> Float;

void settle(Burst &burst, Float seconds, Float hertz, Whole rate);
void strike(Burst &burst, Float level);
auto tick(Burst &burst) -> Float;

}  // namespace SOUND::PLUGINS::CORE::NOISE
