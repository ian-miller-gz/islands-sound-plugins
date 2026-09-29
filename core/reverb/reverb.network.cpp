// SPDX-License-Identifier: AGPL-3.0-or-later
#include "reverb.hpp"

namespace {
using namespace SOUND;

constexpr Float MILLISECOND = 0.001f;
constexpr Float FOLD = 2.0f;

constexpr Float FOURS[CORE::REVERB::FOUR] = {29.7f, 37.1f, 41.1f, 43.7f};
constexpr Float EIGHTS[CORE::REVERB::EIGHT] = {29.7f, 33.1f, 37.1f, 41.1f,
                                               43.7f, 47.3f, 53.9f, 59.1f};

auto bases(Whole count) -> const Float * {
  return count > CORE::REVERB::FOUR ? EIGHTS : FOURS;
}

auto frames(Float milliseconds, Float size, Whole rate) -> Float {
  return milliseconds * MILLISECOND * size * Float(rate);
}

}  // namespace

void SOUND::CORE::REVERB::build(Network &network, Whole count, Whole rate) {
  network.count = count > FOUR ? EIGHT : FOUR;
  network.rate = rate;
  const Float *base = ::bases(network.count);
  for (Whole at = 0; at < network.count; ++at) {
    const Float most = ::frames(base[at], LARGEST, rate);
    LINE::build(network.lines[at], Whole(most) + 1);
  }
}

void SOUND::CORE::REVERB::settle(
  Network &network, Float size, Float seconds, Float cutoff) {
  const Float *base = ::bases(network.count);
  const Float scale = clamped(size);
  for (Whole at = 0; at < network.count; ++at) {
    const Float delay = ::frames(base[at], scale, network.rate);
    network.delays[at] = delay;
    const Float gain = decay(delay, seconds, network.rate);
    LINE::settle(network.loops[at], gain, cutoff, network.rate);
  }
}

auto SOUND::CORE::REVERB::tick(Network &network, Float in) -> Stereo {
  Float outs[EIGHT] = {};
  Float sum = 0;
  for (Whole at = 0; at < network.count; ++at) {
    const Float heard = LINE::read(network.lines[at], network.delays[at]);
    outs[at] = LINE::damp(network.loops[at], heard);
    sum += outs[at];
  }
  const Float fold = sum * FOLD / Float(network.count);
  const Float share = Float(SIDES) / Float(network.count);
  Stereo out;
  for (Whole at = 0; at < network.count; ++at) {
    const Float mixed = (outs[at] - fold) * network.loops[at].feedback;
    LINE::write(network.lines[at], in + mixed);
    (at % SIDES == 0 ? out.left : out.right) += outs[at] * share;
  }
  return out;
}
