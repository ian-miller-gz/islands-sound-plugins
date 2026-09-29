// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "humanizer.hpp"
#include "../../core/block/block.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/notes/notes.hpp"

namespace SOUND::HUMANIZER {

constexpr Whole WAITING = PLUGIN::ROOM;
constexpr Float FAINTEST = 0.01f;
constexpr Float LOUDEST = 1.0f;

struct Pending {
  AUDIO::PLUGIN::Event event;
  Whole due = 0;
};

struct Humanizer {
  Float rows[PARAMETERS] = {};
  CORE::NOISE::White white;
  Whole now = 0;
  Whole delays[CORE::NOTES::KEYS] = {};
  Flag passed[CORE::NOTES::KEYS] = {};
  Pending queue[WAITING] = {};
  Whole queued = 0;
  CORE::NOTES::Tally tally;
  AUDIO::PLUGIN::Event notes[PLUGIN::ROOM] = {};
  CORE::NOTES::Out out;
};

void apply(Humanizer &humanizer, const AUDIO::PLUGIN::Event &event);
void send(Humanizer &humanizer, const AUDIO::PLUGIN::Event &event, Whole frame);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);
auto answer(
  void *instance, const AUDIO::PLUGIN::Event *events, Whole count,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole;

}  // namespace SOUND::HUMANIZER
