// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Resonator &resonator)
  -> Flag {
  const Float sine = resonator.sine * resonator.sine;
  const Float cosine = resonator.cosine * resonator.cosine;
  return sine + cosine > FLOOR * FLOOR;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Sweep &sweep) -> Flag {
  return sounding(sweep.body) || ENVELOPE::sounding(sweep.gate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Kick &kick) -> Flag {
  return sounding(kick.body) || ENVELOPE::sounding(kick.bend) ||
         ENVELOPE::sounding(kick.click);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Snare &snare) -> Flag {
  return sounding(snare.low) || sounding(snare.high) ||
         ENVELOPE::sounding(snare.gate);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Hat &hat) -> Flag {
  return ENVELOPE::sounding(hat.shut.gate) ||
         (ENVELOPE::sounding(hat.open.gate) && sounding(hat.choke));
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Clap &clap) -> Flag {
  return ENVELOPE::sounding(clap.gate) || clap.left > 0 || clap.burst > FLOOR;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Tom &tom) -> Flag {
  return sounding(tom.sweep) || ENVELOPE::sounding(tom.hit);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Cymbal &cymbal) -> Flag {
  for (const ENVELOPE::Gate &gate : cymbal.gates)
    if (ENVELOPE::sounding(gate)) return true;
  return ENVELOPE::sounding(cymbal.burst);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Cowbell &cowbell)
  -> Flag {
  return ENVELOPE::sounding(cowbell.hit) || ENVELOPE::sounding(cowbell.ring);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Clave &clave) -> Flag {
  for (const Resonator &rim : clave.rims)
    if (sounding(rim)) return true;
  return sounding(clave.bar);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::sounding(const Conga &conga) -> Flag {
  return sounding(conga.sweep) || ENVELOPE::sounding(conga.click);
}
