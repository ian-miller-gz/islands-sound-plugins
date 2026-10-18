// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::CLICK::CLAVE;
using CORE::PERCUSSION::CLICK::RIM;
using CORE::PERCUSSION::HEIGHT::HIGH;
using CORE::PERCUSSION::HEIGHT::LOW;
using CORE::PERCUSSION::HEIGHT::MID;
using CORE::PERCUSSION::OPENING::CLOSED;
using CORE::PERCUSSION::OPENING::OPEN;
using CORE::PERCUSSION::PLATE::CRASH;
using CORE::PERCUSSION::PLATE::RIDE;

constexpr Whole WHOLE = 0;

constexpr KIT::Pad PADS[] = {
  {35, KIT::KICK, WHOLE},  {36, KIT::KICK, WHOLE},   {37, KIT::CLAVE, RIM},
  {38, KIT::SNARE, WHOLE}, {39, KIT::CLAP, WHOLE},   {40, KIT::SNARE, WHOLE},
  {41, KIT::TOM, LOW},     {42, KIT::HAT, CLOSED},   {43, KIT::TOM, LOW},
  {44, KIT::HAT, CLOSED},  {45, KIT::TOM, MID},      {46, KIT::HAT, OPEN},
  {47, KIT::TOM, MID},     {48, KIT::TOM, HIGH},     {49, KIT::CYMBAL, CRASH},
  {50, KIT::TOM, HIGH},    {51, KIT::CYMBAL, RIDE},  {52, KIT::CYMBAL, CRASH},
  {53, KIT::CYMBAL, RIDE}, {55, KIT::CYMBAL, CRASH}, {56, KIT::COWBELL, WHOLE},
  {62, KIT::CONGA, HIGH},  {63, KIT::CONGA, MID},    {64, KIT::CONGA, LOW},
  {75, KIT::CLAVE, CLAVE}};

}  // namespace

auto SOUND::PLUGINS::KIT::found(Whole pitch) -> const Pad * {
  for (const Pad &pad : ::PADS)
    if (pad.pitch == pitch) return &pad;
  return nullptr;
}
