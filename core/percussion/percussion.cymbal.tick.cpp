// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {

constexpr Float GAIN = 4.0f;

}  // namespace

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Cymbal &cymbal) -> Float {
  if (!sounding(cymbal)) return 0;
  const Float ring = tick(cymbal.metal);
  Float bands[TAIL::TAILS] = {};
  Float sum = 0;
  for (Whole tail = 0; tail < TAIL::TAILS; ++tail) {
    const ENVELOPE::Envelope &envelope = cymbal.tails[cymbal.plate][tail];
    const Float level = ENVELOPE::tick(cymbal.gates[tail], envelope);
    bands[tail] = FILTER::tick(cymbal.bands[tail], ring);
    sum += bands[tail] * level * cymbal.weights[tail] * cymbal.gains[tail];
  }
  const Float flash = ENVELOPE::tick(cymbal.burst, cymbal.flash);
  const Float splash = flash * cymbal.splash * cymbal.flare;
  return SHAPER::soft(
    (sum + bands[TAIL::WASH] * splash) * cymbal.velocity * ::GAIN);
}
