// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float GAIN = 4.0f;

void hush(PERCUSSION::Cymbal &cymbal) {
  for (ENVELOPE::Gate &gate : cymbal.gates) ENVELOPE::choke(gate);
  ENVELOPE::choke(cymbal.burst);
}

}  // namespace

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Cymbal &cymbal) -> Float {
  if (!sounding(cymbal)) return 0;
  const Float fade = tick(cymbal.choke);
  if (!sounding(cymbal.choke)) {
    ::hush(cymbal);
    return 0;
  }
  const Float ring = tick(cymbal.metal);
  const Float air = NOISE::tick(cymbal.white) - ring;
  Float bands[TAIL::TAILS] = {};
  Float sum = 0;
  for (Whole tail = 0; tail < TAIL::TAILS; ++tail) {
    const ENVELOPE::Envelope &envelope = cymbal.tails[cymbal.plate][tail];
    const Float level = ENVELOPE::tick(cymbal.gates[tail], envelope);
    const Float in = ring + air * cymbal.airs[tail];
    bands[tail] = FILTER::tick(cymbal.bands[tail], in);
    sum += bands[tail] * level * cymbal.weights[tail] * cymbal.gains[tail];
  }
  const Float flash = ENVELOPE::tick(cymbal.burst, cymbal.flash);
  const Float splash = flash * cymbal.splash * cymbal.flare;
  const Float mix = (sum + bands[TAIL::WASH] * splash) * cymbal.velocity;
  return SHAPER::soft(FILTER::tick(cymbal.edge, mix * ::GAIN)) * fade;
}
