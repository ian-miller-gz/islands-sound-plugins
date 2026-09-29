// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float CENTRE = 0.012f;
constexpr Float SWING = 0.004f;
constexpr Float GLIDE = 0;
constexpr Float PACE = 0.7f;
constexpr Float SIZE = 0.8f;
constexpr Float DECAY = 1.4f;
constexpr Float DAMP = 5000.0f;
constexpr Float SIGNS[CHOIR::SIDES] = {1.0f, -1.0f};

auto side(CHOIR::Side &side, Float in, const Float *swings, Float sign)
  -> Float {
  Float sum = 0;
  for (Whole tap = 0; tap < CHOIR::TAPS; ++tap)
    sum += CORE::LINE::read(side.line, side.sweeps[tap], swings[tap] * sign);
  CORE::LINE::write(side.line, in);
  return sum / Float(CHOIR::TAPS);
}

auto blend(CHOIR::Stereo dry, CHOIR::Stereo wet, Float mix) -> CHOIR::Stereo {
  return {
    dry.left + (wet.left - dry.left) * mix,
    dry.right + (wet.right - dry.right) * mix};
}

}  // namespace

void SOUND::PLUGINS::CHOIR::settle(
  Ensemble &ensemble, Room &room, const Float *rows, Whole rate) {
  ensemble.mix = rows[ENSEMBLE];
  for (Side &side : ensemble.sides)
    for (CORE::LINE::Sweep &sweep : side.sweeps)
      CORE::LINE::settle(sweep, CENTRE, SWING, GLIDE, rate);
  for (CORE::MODULATOR::Lfo &lfo : ensemble.lfos) {
    lfo.hertz = PACE;
    CORE::MODULATOR::settle(lfo, rate);
  }
  room.mix = rows[ROOM];
  CORE::REVERB::settle(room.network, SIZE, DECAY, DAMP);
}

auto SOUND::PLUGINS::CHOIR::spread(Choir &choir, Stereo dry) -> Stereo {
  Ensemble &ensemble = choir.ensemble;
  Float swings[TAPS];
  for (Whole tap = 0; tap < TAPS; ++tap)
    swings[tap] = CORE::MODULATOR::tick(ensemble.lfos[tap]);
  const Stereo wet = {
    ::side(ensemble.sides[0], dry.left, swings, SIGNS[0]),
    ::side(ensemble.sides[1], dry.right, swings, SIGNS[1])};
  const Stereo sung = ::blend(dry, wet, ensemble.mix);
  const Float mid = (sung.left + sung.right) * HALF;
  const Stereo room = CORE::REVERB::tick(choir.room.network, mid);
  return ::blend(sung, room, choir.room.mix);
}
