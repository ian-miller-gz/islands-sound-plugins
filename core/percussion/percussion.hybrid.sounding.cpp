// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

auto swept(const PERCUSSION::HYBRID::Swept &swept) -> Flag {
  return ENVELOPE::sounding(swept.head) || ENVELOPE::sounding(swept.tail);
}

}  // namespace

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::sounding(const Kick &kick)
  -> Flag {
  return ::swept(kick.body) || ENVELOPE::sounding(kick.gate) ||
         ENVELOPE::sounding(kick.click);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::sounding(const Snare &snare)
  -> Flag {
  return ::swept(snare.low) || ::swept(snare.high) ||
         ENVELOPE::sounding(snare.tone) || ENVELOPE::sounding(snare.snap);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::sounding(const Tom &tom)
  -> Flag {
  return ::swept(tom.body) || ENVELOPE::sounding(tom.gate) ||
         ENVELOPE::sounding(tom.hit);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::sounding(const Hat &hat)
  -> Flag {
  return ENVELOPE::sounding(hat.gate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Pad &pad) -> Flag {
  return ::swept(pad.tone) || ENVELOPE::sounding(pad.gate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::sounding(const Cymbal &cymbal)
  -> Flag {
  return ENVELOPE::sounding(cymbal.gate);
}
