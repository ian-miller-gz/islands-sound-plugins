// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "arpeggiator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/clock/clock.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/notes/notes.hpp"

namespace SOUND::ARPEGGIATOR {

constexpr Float STRAIGHT = 0.0f;

struct Arpeggio {
  Whole rate = 0;
  Float rows[PARAMETERS] = {};
  CORE::CLOCK::Clock clock;
  CORE::NOISE::White white;
  Float velocities[CORE::NOTES::KEYS] = {};
  Whole keys[CORE::NOTES::KEYS] = {};
  Whole held = 0;
  Whole sounding = CORE::NOTES::SILENT;
  Whole left = 0;
  AUDIO::PLUGIN::Event notes[PLUGIN::ROOM] = {};
  CORE::NOTES::Out out;
};

void settle(Arpeggio &arpeggio);
void apply(Arpeggio &arpeggio, const AUDIO::PLUGIN::Event &event);

auto pick(Arpeggio &arpeggio, Whole step, Whole total) -> Whole;
void silence(Arpeggio &arpeggio, Whole frame);
void tick(Arpeggio &arpeggio, Whole frame);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);
auto answer(
  void *instance, const AUDIO::PLUGIN::Event *events, Whole count,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole;

}  // namespace SOUND::ARPEGGIATOR
