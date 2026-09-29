// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float SEMITONE = 100.0f;

struct Voicing {
  Float tune = 0;
  Float decay = 0;
  Float open = 0;
  Float tone = 0;
  Float attack = 0;
  Float drive = 0;
  Float snappy = 0;
  Float spread = 0;
  Float bend = 0;
  Float splash = 0;
};

constexpr Voicing VOICINGS[KIT::DRUMS] = {
  {.tune = 55, .decay = 0.5f, .tone = 4000, .attack = 0.4f, .drive = 0.2f},
  {.tune = 180, .decay = 0.25f, .tone = 3000, .snappy = 0.6f},
  {.decay = 0.06f, .open = 0.45f, .tone = 6000},
  {.decay = 0.3f, .tone = 1100, .spread = 0.011f},
  {.tune = 90, .decay = 0.4f, .tone = 4000, .bend = 0.3f},
  {.decay = 1.5f, .tone = 0.5f, .splash = 0.5f},
  {.decay = 0.35f},
  {.tune = 2500, .decay = 0.06f},
  {.tune = 190, .decay = 0.3f, .bend = 0.2f}};

auto tuned(Float hertz, Float shift) -> Float {
  return hertz * CORE::PHASE::ratio(shift * SEMITONE);
}

using Settle = void (*)(KIT::Kit &kit, const Voicing &made, Float shift);

constexpr Settle SETTLES[KIT::DRUMS] = {
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.kick, tuned(made.tune, shift), made.decay, made.attack, made.drive,
      made.tone, kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.snare, tuned(made.tune, shift), made.tone, made.snappy, made.decay,
      kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.hat, shift, made.decay, made.open, made.tone, kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.clap, made.spread, made.decay, tuned(made.tone, shift), kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.tom, tuned(made.tune, shift), made.decay, made.bend, made.tone,
      kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.cymbal, shift, made.decay, made.tone, made.splash, kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(kit.cowbell, shift, made.decay, kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.clave, tuned(made.tune, shift), made.decay, kit.rate);
  },
  [](KIT::Kit &kit, const Voicing &made, Float shift) {
    CORE::PERCUSSION::settle(
      kit.conga, tuned(made.tune, shift), made.decay, made.bend, kit.rate);
  }};

}  // namespace

void SOUND::KIT::settle(Kit &kit, Whole drum) {
  if (drum >= DRUMS) return;
  ::SETTLES[drum](kit, ::VOICINGS[drum], kit.rows[place(drum, TUNE)]);
}

void SOUND::KIT::settle(Kit &kit) {
  for (Whole drum = 0; drum < DRUMS; ++drum) settle(kit, drum);
}
