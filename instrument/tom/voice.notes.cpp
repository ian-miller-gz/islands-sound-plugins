// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

namespace HEIGHT = CORE::PERCUSSION::HEIGHT;

constexpr Float MIDDLE = 60;
constexpr Float HALF = 0.5f;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {41, HEIGHT::LOW}, {43, HEIGHT::LOW},  {45, HEIGHT::MID},
  {47, HEIGHT::MID}, {48, HEIGHT::HIGH}, {50, HEIGHT::HIGH}};

void hit(TOM::Voice &voice, Whole note, Float velocity) {
  if (voice.rows[TOM::KEY] > HALF) {
    const Float cents =
      (Float(note) - MIDDLE) * CORE::PHASE::CENTS / CORE::PHASE::OCTAVE;
    const Float hertz = voice.rows[TOM::TUNE] * CORE::PHASE::ratio(cents);
    CORE::PERCUSSION::strike(voice.tom, hertz, velocity);
    return;
  }
  const Whole height = CORE::PERCUSSION::keyed(KEYS, note, HEIGHT::MID);
  CORE::PERCUSSION::strike(voice.tom, height, velocity);
}

}  // namespace

void SOUND::PLUGINS::TOM::apply(
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
