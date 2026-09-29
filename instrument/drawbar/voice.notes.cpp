// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole KINDS = AUDIO::PLUGIN::Event::PROGRAM + 1;
constexpr Float LETTING = 0.5f;

void release(DRAWBAR::Organ &organ, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= CORE::PHASE::PITCHES || !organ.keys[event.index]) return;
  organ.keys[event.index] = false;
  --organ.held;
  CORE::NOISE::strike(organ.click, organ.rows[DRAWBAR::CLICK] * LETTING);
  DRAWBAR::pull(organ);
}

void press(DRAWBAR::Organ &organ, const AUDIO::PLUGIN::Event &event) {
  if (event.value <= 0) return release(organ, event);
  if (event.index >= CORE::PHASE::PITCHES || organ.keys[event.index]) return;
  if (organ.held == 0 && organ.rows[DRAWBAR::PERCUSSION] > 0)
    CORE::ENVELOPE::strike(organ.gate, organ.percussion, CORE::ENVELOPE::FULL);
  organ.keys[event.index] = true;
  ++organ.held;
  CORE::NOISE::strike(organ.click, organ.rows[DRAWBAR::CLICK]);
  DRAWBAR::pull(organ);
}

void steer(DRAWBAR::Organ &organ, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= DRAWBAR::PARAMETERS) return;
  organ.rows[event.index] =
    CORE::TABLE::clamped(DRAWBAR::SHEET, event.index, event.value);
  DRAWBAR::settle(organ);
}

void ignore(DRAWBAR::Organ &, const AUDIO::PLUGIN::Event &) {}

using Handler = void (*)(DRAWBAR::Organ &, const AUDIO::PLUGIN::Event &);

constexpr Handler HANDLERS[KINDS] = {press,  release, steer,
                                     ignore, ignore,  ignore};

}  // namespace

void SOUND::PLUGINS::DRAWBAR::apply(
  Organ &organ, const AUDIO::PLUGIN::Event &event) {
  if (event.kind < ::KINDS) ::HANDLERS[event.kind](organ, event);
}
