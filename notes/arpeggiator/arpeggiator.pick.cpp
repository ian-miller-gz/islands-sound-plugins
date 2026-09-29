// SPDX-License-Identifier: AGPL-3.0-or-later
#include "arpeggiator.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Pick = auto (*)(ARPEGGIATOR::Arpeggio &, Whole, Whole) -> Whole;

constexpr Whole PAIR = 2;

auto ascend(ARPEGGIATOR::Arpeggio &, Whole step, Whole total) -> Whole {
  return step % total;
}

auto descend(ARPEGGIATOR::Arpeggio &, Whole step, Whole total) -> Whole {
  return total - 1 - step % total;
}

auto bounce(ARPEGGIATOR::Arpeggio &, Whole step, Whole total) -> Whole {
  if (total < ::PAIR) return 0;
  const Whole cycle = ::PAIR * total - ::PAIR;
  const Whole at = step % cycle;
  return at < total ? at : cycle - at;
}

auto scatter(ARPEGGIATOR::Arpeggio &arpeggio, Whole, Whole total) -> Whole {
  const Float drawn = CORE::NOTES::draw(arpeggio.white);
  const Whole at = Whole(drawn * Float(total));
  return at < total ? at : total - 1;
}

constexpr Pick PICKS[] = {ascend, descend, bounce, scatter};
static_assert(sizeof(PICKS) / sizeof(PICKS[0]) == ARPEGGIATOR::MODES);

}  // namespace

auto SOUND::PLUGINS::ARPEGGIATOR::pick(
  Arpeggio &arpeggio, Whole step, Whole total) -> Whole {
  const Whole mode = Whole(arpeggio.rows[MODE]);
  return ::PICKS[mode < MODES ? mode : 0](arpeggio, step, total);
}
