// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole SIDES = 2;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Pair {
  Float left = 0;
  Float right = 0;
};

auto heard(AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame)
  -> Float {
  Float sum = 0;
  for (Whole channel = 0; channel < channels; ++channel)
    sum += lanes[channel][frame];
  return sum / Float(channels);
}

auto sing(DOUBLER::Doubler &doubler, Float in) -> Pair {
  CORE::LINE::write(doubler.line, in);
  Pair sum;
  for (Whole index = 0; index < DOUBLER::COPIES; ++index) {
    DOUBLER::Copy &copy = doubler.copies[index];
    const Float wobble = CORE::MODULATOR::tick(copy.wobble);
    const Float delay = copy.delay + doubler.swing * (::UNITY + wobble);
    const Float read = CORE::LINE::read(doubler.line, delay);
    const Float voice = CORE::PITCH::tick(copy.grains, read);
    if (index >= doubler.count) continue;
    sum.left += voice * copy.left;
    sum.right += voice * copy.right;
  }
  sum.left *= doubler.scale;
  sum.right *= doubler.scale;
  return sum;
}

auto wet(const Pair &pair, Whole channel, Whole channels) -> Float {
  if (channels == 1 || channel >= ::SIDES)
    return (pair.left + pair.right) * ::HALF;
  return channel == 0 ? pair.left : pair.right;
}

auto pour(
  DOUBLER::Doubler &doubler, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  const Pair pair = ::sing(doubler, ::heard(lanes, doubler.channels, frame));
  const Float mix = doubler.rows[DOUBLER::MIX];
  Float peak = 0;
  for (Whole channel = 0; channel < doubler.channels; ++channel) {
    const Float dry = lanes[channel][frame] * (::UNITY - mix);
    const Float out = dry + ::wet(pair, channel, doubler.channels) * mix;
    lanes[channel][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::PLUGINS::DOUBLER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &doubler = *static_cast<Doubler *>(instance);
  const auto take = [&doubler](const AUDIO::PLUGIN::Event &event) {
    apply(doubler, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const Float size = ::pour(doubler, lanes, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(doubler.meter, peak);
}
