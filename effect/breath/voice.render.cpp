// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNITY = 1.0f;

struct Heard {
  Float whole = 0;
  Float high = 0;
};

auto split(
  BREATH::Breath &breath, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Heard {
  Heard sum;
  for (Whole channel = 0; channel < breath.channels; ++channel) {
    const Float in = lanes[channel][frame];
    const Float high = CORE::FILTER::tick(breath.filters[channel], in);
    breath.bands[channel] = high;
    sum.whole += in;
    sum.high += high;
  }
  const Float share = ::UNITY / Float(breath.channels);
  return {sum.whole * share, sum.high * share};
}

auto pour(
  BREATH::Breath &breath, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  const Heard heard = ::split(breath, lanes, frame);
  const Float gain = BREATH::gated(breath, heard.whole, heard.high);
  Float peak = 0;
  for (Whole channel = 0; channel < breath.channels; ++channel) {
    const Float high = breath.bands[channel];
    const Float out = lanes[channel][frame] - (::UNITY - gain) * high;
    lanes[channel][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::PLUGINS::BREATH::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &breath = *static_cast<Breath *>(instance);
  const auto take = [&breath](const AUDIO::PLUGIN::Event &event) {
    apply(breath, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const Float size = ::pour(breath, lanes, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(breath.meter, peak);
}
