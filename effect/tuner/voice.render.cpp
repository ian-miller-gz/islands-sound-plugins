// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto heard(AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame)
  -> Float {
  Float sum = 0;
  for (Whole channel = 0; channel < channels; ++channel)
    sum += lanes[channel][frame];
  return sum / Float(channels);
}

auto shifted(const TUNER::Tuner &tuner, TUNER::Shifter &shifter, Float in)
  -> Float {
  return tuner.keep ? CORE::PITCH::tick(shifter.formant, in)
                    : CORE::PITCH::tick(shifter.grains, in);
}

auto pour(TUNER::Tuner &tuner, AUDIO::PLUGIN::Sample *const *lanes, Whole frame)
  -> Float {
  TUNER::follow(tuner, ::heard(lanes, tuner.channels, frame));
  if (++tuner.count >= TUNER::STRIDE) {
    tuner.count = 0;
    TUNER::steer(tuner);
  }
  Float peak = 0;
  for (Whole channel = 0; channel < tuner.channels; ++channel) {
    const Float out =
      ::shifted(tuner, tuner.shifters[channel], lanes[channel][frame]);
    lanes[channel][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::TUNER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &tuner = *static_cast<Tuner *>(instance);
  const auto take = [&tuner](const AUDIO::PLUGIN::Event &event) {
    apply(tuner, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const Float size = ::pour(tuner, lanes, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(tuner.meter, peak);
}
