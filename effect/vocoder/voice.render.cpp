// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto heard(AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame)
  -> Float {
  Float sum = 0;
  for (Whole channel = 0; channel < channels; ++channel)
    sum += lanes[channel][frame];
  return sum / Float(channels);
}

auto pour(
  VOCODER::Vocoder &vocoder, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  const Float modulator = ::heard(lanes, vocoder.channels, frame);
  const Float carrier = VOCODER::play(vocoder);
  const Float wet = VOCODER::vocode(vocoder, modulator, carrier) * vocoder.gain;
  const Float dry = vocoder.rows[VOCODER::DRY];
  Float peak = 0;
  for (Whole channel = 0; channel < vocoder.channels; ++channel) {
    const Float out = wet + lanes[channel][frame] * dry;
    lanes[channel][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::PLUGINS::VOCODER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &vocoder = *static_cast<Vocoder *>(instance);
  const auto take = [&vocoder](const AUDIO::PLUGIN::Event &event) {
    apply(vocoder, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const Float size = ::pour(vocoder, lanes, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(vocoder.meter, peak);
}
