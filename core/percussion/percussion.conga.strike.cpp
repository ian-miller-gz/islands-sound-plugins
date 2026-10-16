// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SOFT = 0.75f;
constexpr Float ONE = 1.0f;
constexpr Float WIDEST = 2.0f;
constexpr Float CEILING = 0.2f;

auto slapped(Float velocity) -> Float {
  const Float over = (velocity - SOFT) / (ONE - SOFT);
  return over < 0 ? 0 : over;
}

auto bounded(Whole height) -> Whole {
  return height < PERCUSSION::HEIGHT::HEIGHTS ? height
                                              : PERCUSSION::HEIGHT::MID;
}

void hit(PERCUSSION::Conga &conga, Float base, Float velocity, Flag muted) {
  conga.velocity = velocity;
  conga.slap = ::slapped(conga.velocity);
  const Float hand = muted ? ONE : conga.slap;
  conga.sweep.depth = conga.bend * (ONE + conga.slap);
  PERCUSSION::strike(conga.sweep, base, conga.velocity);
  conga.sweep.body.damp = conga.open + (conga.muted - conga.open) * hand;
  for (PERCUSSION::Mode &mode : conga.modes) {
    PERCUSSION::strike(mode.resonator, conga.velocity);
    mode.resonator.damp = mode.open + (mode.muted - mode.open) * hand;
  }
  ENVELOPE::strike(conga.click, conga.snap, conga.velocity);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Conga &conga, Whole height, Float velocity) {
  ::hit(conga, conga.bases[::bounded(height)], struck(velocity), false);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Conga &conga, const Stroke &stroke, Float velocity) {
  const Float base = conga.bases[::bounded(stroke.height)];
  ::hit(conga, base, struck(velocity, conga.curve), stroke.muted);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Conga &conga, Float hertz, Float velocity) {
  const Float top = Float(conga.rate) * CEILING / (ONE + conga.bend * WIDEST);
  const Float held = hertz > top ? top : hertz;
  ::hit(conga, turn(held, conga.rate), struck(velocity, conga.curve), false);
}
