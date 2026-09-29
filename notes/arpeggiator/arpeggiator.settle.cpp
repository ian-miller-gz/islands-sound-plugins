// SPDX-License-Identifier: AGPL-3.0-or-later
#include "arpeggiator.internal.hpp"

namespace {
using namespace SOUND;

auto beats(const ARPEGGIATOR::Arpeggio &arpeggio) -> Float {
  const Whole division = Whole(arpeggio.rows[ARPEGGIATOR::DIVISION]);
  return ARPEGGIATOR::BEATS
    [division < ARPEGGIATOR::DIVISIONS ? division : ARPEGGIATOR::SIXTEENTH];
}

void press(ARPEGGIATOR::Arpeggio &arpeggio, const AUDIO::PLUGIN::Event &event) {
  Float &velocity = arpeggio.velocities[event.index];
  if (velocity <= 0 && arpeggio.held++ == 0) {
    CORE::CLOCK::reset(arpeggio.clock);
    CORE::CLOCK::run(arpeggio.clock);
  }
  velocity = event.value;
}

void release(
  ARPEGGIATOR::Arpeggio &arpeggio, const AUDIO::PLUGIN::Event &event) {
  Float &velocity = arpeggio.velocities[event.index];
  if (velocity <= 0) return;
  velocity = 0;
  if (--arpeggio.held > 0) return;
  CORE::CLOCK::stop(arpeggio.clock);
  ARPEGGIATOR::silence(arpeggio, event.offset);
}

void turn(ARPEGGIATOR::Arpeggio &arpeggio, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= ARPEGGIATOR::PARAMETERS) return;
  arpeggio.rows[event.index] =
    CORE::TABLE::clamped(ARPEGGIATOR::SHEET, event.index, event.value);
  if (event.index == ARPEGGIATOR::SEED)
    CORE::NOTES::sow(arpeggio.white, arpeggio.rows[ARPEGGIATOR::SEED]);
  ARPEGGIATOR::settle(arpeggio);
}

}  // namespace

void SOUND::ARPEGGIATOR::settle(Arpeggio &arpeggio) {
  CORE::CLOCK::settle(
    arpeggio.clock, arpeggio.rows[TEMPO], ::beats(arpeggio), STRAIGHT,
    arpeggio.rate);
}

void SOUND::ARPEGGIATOR::apply(
  Arpeggio &arpeggio, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER)
    return ::turn(arpeggio, event);
  const Flag struck = CORE::NOTES::struck(event);
  if (!struck && !CORE::NOTES::lifted(event)) {
    CORE::NOTES::put(arpeggio.out, event);
    return;
  }
  if (event.index > CORE::NOTES::HIGHEST) return;
  if (struck) return ::press(arpeggio, event);
  ::release(arpeggio, event);
}
