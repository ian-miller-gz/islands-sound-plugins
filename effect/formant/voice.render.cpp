// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float UNITY = 1.0f;

auto pour(
  FORMANT::Formant &formant, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  const Float mix = formant.rows[FORMANT::MIX];
  Float peak = 0;
  for (Whole channel = 0; channel < formant.channels; ++channel) {
    const Float in = lanes[channel][frame];
    const Float wet = CORE::PITCH::tick(formant.shifters[channel], in);
    const Float out = in * (::UNITY - mix) + wet * mix;
    lanes[channel][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::FORMANT::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &formant = *static_cast<Formant *>(instance);
  const auto take = [&formant](const AUDIO::PLUGIN::Event &event) {
    apply(formant, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const Float size = ::pour(formant, lanes, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(formant.meter, peak);
}
