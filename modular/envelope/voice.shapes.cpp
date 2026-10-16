// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float BASE = 10.0f;
constexpr Float CEILING = 2.0f;
constexpr Float SPAN = 5.0f;

auto adsr(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::ADSR::create(
    rows[ENVELOPE::ATTACK], rows[ENVELOPE::DECAY], rows[ENVELOPE::SUSTAIN],
    rows[ENVELOPE::RELEASE]);
}

auto ad(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::AD::create(
    rows[ENVELOPE::ATTACK], rows[ENVELOPE::DECAY]);
}

auto ar(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::AR::create(
    rows[ENVELOPE::ATTACK], rows[ENVELOPE::RELEASE]);
}

auto gate(const Float *rows) -> CORE::ENVELOPE::Envelope {
  return CORE::ENVELOPE::GATE::create(rows[ENVELOPE::VELOCITY]);
}

using Builder = auto (*)(const Float *) -> CORE::ENVELOPE::Envelope;

constexpr Builder BUILDERS[] = {adsr, ad, ar, gate};
static_assert(std::size(BUILDERS) == std::size(ENVELOPE::SHAPES));

auto ratio(Float bow) -> Float { return std::pow(BASE, CEILING - SPAN * bow); }

}  // namespace

auto SOUND::PLUGINS::ENVELOPE::shaped(const Float *rows)
  -> CORE::ENVELOPE::Envelope {
  const Whole shape = Whole(rows[SHAPE]);
  CORE::ENVELOPE::Envelope envelope =
    ::BUILDERS[shape <= GATE ? shape : ADSR](rows);
  envelope.curve = Whole(rows[CURVE]);
  envelope.trigger = Whole(rows[TRIGGER]);
  envelope.depth = rows[VELOCITY];
  envelope.rise = ::ratio(rows[RISE]);
  envelope.fall = ::ratio(rows[FALL]);
  return envelope;
}
