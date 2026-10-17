// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float CLICKS = 3;

auto wait(DRAWBAR::Contacts &contacts) -> Whole {
  const Float drawn = CORE::MODULATOR::draw(contacts.seed);
  const Float fraction = HALF * (drawn + CORE::MODULATOR::FULL);
  return Whole(fraction * contacts.spread) + 1;
}

auto due(DRAWBAR::Contacts &contacts, Whole pitch) -> Whole {
  Whole closing = 0;
  for (Whole &wait : contacts.waits[pitch])
    if (wait > 0 && --wait == 0) ++closing;
  return closing;
}

}  // namespace

void SOUND::PLUGINS::DRAWBAR::arm(Organ &organ, Whole pitch) {
  Contacts &contacts = organ.contacts;
  for (Whole &wait : contacts.waits[pitch]) wait = ::wait(contacts);
  contacts.pending += BARS;
}

void SOUND::PLUGINS::DRAWBAR::lift(Organ &organ, Whole pitch) {
  Contacts &contacts = organ.contacts;
  for (Whole &wait : contacts.waits[pitch]) {
    if (wait > 0) --contacts.pending;
    wait = 0;
  }
}

void SOUND::PLUGINS::DRAWBAR::close(Organ &organ) {
  Contacts &contacts = organ.contacts;
  if (contacts.pending == 0) return;
  Whole closing = 0;
  for (Whole pitch = 0; pitch < CORE::PHASE::PITCHES; ++pitch)
    if (organ.keys[pitch]) closing += ::due(contacts, pitch);
  if (closing == 0) return;
  contacts.pending -= closing;
  CORE::NOISE::strike(organ.click, organ.rows[CLICK] / ::CLICKS);
  pull(organ);
}

auto SOUND::PLUGINS::DRAWBAR::closed(const Organ &organ, Whole pitch, Whole bar)
  -> Flag {
  return organ.keys[pitch] && organ.contacts.waits[pitch][bar] == 0;
}
