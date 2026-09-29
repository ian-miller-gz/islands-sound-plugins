// SPDX-License-Identifier: AGPL-3.0-or-later
#include "humanizer.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void schedule(
  HUMANIZER::Humanizer &humanizer, const AUDIO::PLUGIN::Event &event,
  Whole delay) {
  if (humanizer.queued >= HUMANIZER::WAITING)
    return HUMANIZER::send(humanizer, event, event.offset);
  const Whole due = humanizer.now + event.offset + delay;
  humanizer.queue[humanizer.queued++] = {event, due};
}

void lift(HUMANIZER::Humanizer &humanizer, AUDIO::PLUGIN::Event event) {
  Flag &passed = humanizer.passed[event.index];
  if (!passed) return;
  passed = false;
  event.kind = AUDIO::PLUGIN::Event::NOTE_OFF;
  event.value = 0;
  ::schedule(humanizer, event, humanizer.delays[event.index]);
}

auto loudness(const HUMANIZER::Humanizer &humanizer, Float velocity, Float lean)
  -> Float {
  const Float moved = velocity + lean * humanizer.rows[HUMANIZER::VELOCITY];
  return moved < HUMANIZER::FAINTEST  ? HUMANIZER::FAINTEST
         : moved > HUMANIZER::LOUDEST ? HUMANIZER::LOUDEST
                                      : moved;
}

void strike(HUMANIZER::Humanizer &humanizer, AUDIO::PLUGIN::Event event) {
  ::lift(humanizer, event);
  const Float chance = CORE::NOTES::draw(humanizer.white) * HUMANIZER::PERCENT;
  const Float lean = CORE::NOISE::tick(humanizer.white);
  const Float late = CORE::NOTES::draw(humanizer.white);
  if (chance >= humanizer.rows[HUMANIZER::PROBABILITY]) return;
  const Whole delay = Whole(late * humanizer.rows[HUMANIZER::TIMING]);
  event.value = ::loudness(humanizer, event.value, lean);
  humanizer.delays[event.index] = delay;
  humanizer.passed[event.index] = true;
  ::schedule(humanizer, event, delay);
}

void turn(HUMANIZER::Humanizer &humanizer, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= HUMANIZER::PARAMETERS) return;
  humanizer.rows[event.index] =
    CORE::TABLE::clamped(HUMANIZER::SHEET, event.index, event.value);
  if (event.index == HUMANIZER::SEED)
    CORE::NOTES::sow(humanizer.white, humanizer.rows[HUMANIZER::SEED]);
}

}  // namespace

void SOUND::PLUGINS::HUMANIZER::send(
  Humanizer &humanizer, const AUDIO::PLUGIN::Event &event, Whole frame) {
  if (CORE::NOTES::struck(event))
    return CORE::NOTES::strike(
      humanizer.tally, humanizer.out, event.index, frame, event.value);
  CORE::NOTES::lift(humanizer.tally, humanizer.out, event.index, frame);
}

void SOUND::PLUGINS::HUMANIZER::apply(
  Humanizer &humanizer, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER)
    return ::turn(humanizer, event);
  const Flag struck = CORE::NOTES::struck(event);
  if (!struck && !CORE::NOTES::lifted(event)) {
    CORE::NOTES::put(humanizer.out, event);
    return;
  }
  if (event.index > CORE::NOTES::HIGHEST) return;
  if (struck) return ::strike(humanizer, event);
  ::lift(humanizer, event);
}
