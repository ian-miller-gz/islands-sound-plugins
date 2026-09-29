// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float UNITY = 1.0f;

auto band(Float in, Float high, Float gain) -> Float {
  return in - (::UNITY - gain) * high;
}

auto wide(Float in, Float, Float gain) -> Float { return in * gain; }

using Shape = auto (*)(Float in, Float high, Float gain) -> Float;

constexpr Shape SHAPES[DEESSER::MODES] = {band, wide};

auto split(
  DEESSER::Deesser &deesser, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  Float loudest = 0;
  for (Whole channel = 0; channel < deesser.channels; ++channel) {
    const Float high =
      CORE::FILTER::tick(deesser.filters[channel], lanes[channel][frame]);
    deesser.bands[channel] = high;
    const Float size = CORE::BLOCK::magnitude(high);
    if (size > loudest) loudest = size;
  }
  return loudest;
}

auto pour(
  DEESSER::Deesser &deesser, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  const Float gain = DEESSER::reduced(deesser, ::split(deesser, lanes, frame));
  const Shape shape = ::SHAPES[deesser.mode];
  Float peak = 0;
  for (Whole channel = 0; channel < deesser.channels; ++channel) {
    const Float in = lanes[channel][frame];
    const Float high = deesser.bands[channel];
    const Float out = deesser.listen ? high : shape(in, high, gain);
    lanes[channel][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::DEESSER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &deesser = *static_cast<Deesser *>(instance);
  const auto take = [&deesser](const AUDIO::PLUGIN::Event &event) {
    apply(deesser, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const Float size = ::pour(deesser, lanes, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(deesser.meter, peak);
}
