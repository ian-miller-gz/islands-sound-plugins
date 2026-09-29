// SPDX-License-Identifier: AGPL-3.0-or-later
#include "quantizer.internal.hpp"

namespace {
using namespace SOUND;

constexpr Whole REACH = QUANTIZER::DEGREES / 2;

auto chosen(const QUANTIZER::Quantizer &quantizer, Whole index, Whole count)
  -> Whole {
  const Whole place = Whole(quantizer.rows[index]);
  return place < count ? place : 0;
}

auto member(const Flag *members, Whole root, Integer pitch) -> Flag {
  if (CORE::NOTES::placed(pitch) == CORE::NOTES::SILENT) return false;
  const Whole degree = (static_cast<Whole>(pitch) + QUANTIZER::DEGREES - root) %
                       QUANTIZER::DEGREES;
  return members[degree];
}

}  // namespace

auto SOUND::QUANTIZER::snapped(const Quantizer &quantizer, Whole pitch)
  -> Whole {
  const Flag *members = MEMBERS[::chosen(quantizer, SCALE, SCALES)];
  const Whole root = ::chosen(quantizer, ROOT, DEGREES);
  const Integer from = static_cast<Integer>(pitch);
  for (Integer reach = 0; reach <= static_cast<Integer>(::REACH); ++reach) {
    if (::member(members, root, from - reach))
      return static_cast<Whole>(from - reach);
    if (::member(members, root, from + reach))
      return static_cast<Whole>(from + reach);
  }
  return CORE::NOTES::SILENT;
}
