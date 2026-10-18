// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::OSCILLATOR {

struct Edges {
  Float level = 0;
  Float now = 0;
  Float next = 0;
};

void jump(Edges &edges, Float height, Float fraction);
auto tick(Edges &edges, Float level) -> Float;

}  // namespace SOUND::PLUGINS::CORE::OSCILLATOR
