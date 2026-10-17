// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {HAT::PITCH::CLOSED, CORE::PERCUSSION::OPENING::CLOSED},
  {HAT::PITCH::PEDAL, CORE::PERCUSSION::OPENING::PEDAL},
  {HAT::PITCH::OPEN, CORE::PERCUSSION::OPENING::OPEN}};

}  // namespace

void SOUND::PLUGINS::HAT::strike(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  const Float level = CORE::PERCUSSION::struck(event.value, voice.rows[CURVE]);
  const Whole opening = CORE::PERCUSSION::keyed(
    KEYS, event.index, CORE::PERCUSSION::OPENING::CLOSED);
  CORE::PERCUSSION::strike(voice.hat, opening, level);
}
