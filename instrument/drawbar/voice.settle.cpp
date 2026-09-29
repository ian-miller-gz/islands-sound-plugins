// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float ONSET = 0.001f;
constexpr Float FAST = 0.2f;
constexpr Float SLOW = 1.0f;
constexpr Float NORMAL = 1.0f;
constexpr Float SOFT = 0.5f;
constexpr Float CONTACT = 0.002f;
constexpr Float SNAP = 0.004f;
constexpr Float BRIGHT = 4000;

auto percussion(const Float *rows, Whole rate) -> CORE::ENVELOPE::Envelope {
  const Float decay = rows[DRAWBAR::DECAY] > 0 ? SLOW : FAST;
  auto envelope = CORE::ENVELOPE::AD::create(ONSET, decay);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

auto accent(const Float *rows) -> Float {
  if (rows[DRAWBAR::PERCUSSION] <= 0) return 0;
  return rows[DRAWBAR::LEVEL] > 0 ? SOFT : NORMAL;
}

}  // namespace

void SOUND::DRAWBAR::settle(Organ &organ) {
  const Float *rows = organ.rows;
  organ.percussion = ::percussion(rows, organ.rate);
  organ.accent = ::accent(rows);
  organ.pole = CORE::MODULATOR::pole(CONTACT, organ.rate);
  CORE::NOISE::settle(organ.click, SNAP, BRIGHT, organ.rate);
  settle(organ.scanner, Whole(rows[SCANNER]), organ.rate);
  settle(organ.rotary, Whole(rows[ROTARY]), organ.rate);
  tune(organ);
  pull(organ);
}
