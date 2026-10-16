// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "voice.voicings.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SEMITONE = 100.0f;
constexpr Float UNTONED = 0;

struct Dial {
  Float shift = 0;
  Float decay = 0;
  Float tone = 0;
};

auto tuned(Float hertz, Float shift) -> Float {
  return hertz * CORE::PHASE::ratio(shift * SEMITONE);
}

using Settle =
  void (*)(KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial);

constexpr Settle SETTLES[KIT::DRUMS] = {
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.kick, tuned(made.tune, dial.shift), dial.decay, made.attack,
      made.drive, dial.tone, kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.snare, tuned(made.tune, dial.shift), dial.tone, made.snappy,
      dial.decay, kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.hat, dial.shift, made.closed, dial.decay, dial.tone, kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.clap, made.spread, dial.decay, tuned(dial.tone, dial.shift),
      kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.tom, tuned(made.tune, dial.shift), dial.decay, made.bend, dial.tone,
      kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.cymbal, dial.shift, dial.decay, dial.tone, made.splash, kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &, const Dial &dial) {
    CORE::PERCUSSION::settle(kit.cowbell, dial.shift, dial.decay, kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.clave, tuned(made.tune, dial.shift), dial.decay, kit.rate);
  },
  [](KIT::Kit &kit, const KIT::Voicing &made, const Dial &dial) {
    CORE::PERCUSSION::settle(
      kit.conga, tuned(made.tune, dial.shift), dial.decay, made.bend, kit.rate);
  }};

auto dialled(const KIT::Kit &kit, Whole drum) -> Dial {
  const Float tone = drum < KIT::TONED ? kit.rows[KIT::TONES + drum] : UNTONED;
  return {
    kit.rows[KIT::place(drum, KIT::TUNE)], kit.rows[KIT::DECAYS + drum], tone};
}

}  // namespace

auto SOUND::PLUGINS::KIT::drummed(Whole index) -> Whole {
  if (index < ACCENT) return index % KNOBS == TUNE ? index / KNOBS : DRUMS;
  if (index >= DECAYS && index < TONES) return index - DECAYS;
  if (index >= TONES && index < PANS) return index - TONES;
  return DRUMS;
}

void SOUND::PLUGINS::KIT::settle(Kit &kit, Whole drum) {
  if (drum >= DRUMS) return;
  ::SETTLES[drum](kit, VOICINGS[drum], ::dialled(kit, drum));
}

void SOUND::PLUGINS::KIT::settle(Kit &kit) {
  for (Whole drum = 0; drum < DRUMS; ++drum) {
    settle(kit, drum);
    pan(kit, drum);
  }
}
