// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole STICK = 37;
constexpr Whole ACOUSTIC = 38;
constexpr Whole ELECTRIC = 40;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {STICK, CORE::PERCUSSION::SNARE::SIDE},
  {ACOUSTIC, CORE::PERCUSSION::SNARE::HEAD},
  {ELECTRIC, CORE::PERCUSSION::SNARE::RIM}};

}  // namespace

void SOUND::PLUGINS::SNARE::strike(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  const Float level = CORE::PERCUSSION::struck(event.value, voice.rows[CURVE]);
  const Whole stroke =
    CORE::PERCUSSION::keyed(KEYS, event.index, CORE::PERCUSSION::SNARE::HEAD);
  CORE::PERCUSSION::strike(voice.snare, stroke, level);
}
