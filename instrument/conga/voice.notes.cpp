// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

namespace HEIGHT = CORE::PERCUSSION::HEIGHT;

constexpr Float MIDDLE = 60;
constexpr Float HALF = 0.5f;

enum : Whole { LOW, MID, HIGH, MUTE };

constexpr CORE::PERCUSSION::Stroke STROKES[] = {
  {.height = HEIGHT::LOW},
  {.height = HEIGHT::MID},
  {.height = HEIGHT::HIGH},
  {.height = HEIGHT::HIGH, .muted = true}};

constexpr CORE::PERCUSSION::Key KEYS[] = {{62, MUTE}, {63, HIGH}, {64, LOW}};

void hit(CONGA::Voice &voice, Whole note, Float velocity) {
  if (voice.rows[CONGA::KEY] > HALF) {
    const Float shift = (Float(note) - MIDDLE) / CORE::PHASE::OCTAVE;
    const Float hertz = voice.rows[CONGA::TUNE] * std::exp2(shift);
    CORE::PERCUSSION::strike(voice.conga, hertz, velocity);
    return;
  }
  const Whole stroke = CORE::PERCUSSION::keyed(KEYS, note, MID);
  CORE::PERCUSSION::strike(voice.conga, STROKES[stroke], velocity);
}

}  // namespace

void SOUND::PLUGINS::CONGA::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value > 0)
    ::hit(voice, event.index, event.value);
}
