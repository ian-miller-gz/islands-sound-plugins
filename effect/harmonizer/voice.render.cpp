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

auto shifted(
  const HARMONIZER::Harmonizer &harmonizer, HARMONIZER::Part &part,
  Float in) -> Float {
  return harmonizer.keep ? CORE::PITCH::tick(part.formant, in)
                         : CORE::PITCH::tick(part.grains, in);
}

auto sing(HARMONIZER::Harmonizer &harmonizer, Float in) -> Pair {
  Pair sum;
  for (Whole at = 0; at < HARMONIZER::VOICES; ++at) {
    HARMONIZER::Part &part = harmonizer.parts[at];
    const CORE::VOICE::Note &note = harmonizer.allocator.notes[at];
    const Float voice = ::shifted(harmonizer, part, in);
    if (!note.sounding) continue;
    const Float gate = note.held ? ::UNITY : 0;
    const Float level = CORE::MODULATOR::tick(part.gate, gate) * voice;
    sum.left += level * part.left;
    sum.right += level * part.right;
  }
  return sum;
}

auto wet(const Pair &pair, Whole channel, Whole channels) -> Float {
  if (channels == 1 || channel >= ::SIDES)
    return (pair.left + pair.right) * ::HALF;
  return channel == 0 ? pair.left : pair.right;
}

auto pour(
  HARMONIZER::Harmonizer &harmonizer, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  const Float in = ::heard(lanes, harmonizer.channels, frame);
  HARMONIZER::follow(harmonizer, in);
  if (++harmonizer.count >= HARMONIZER::STRIDE) {
    harmonizer.count = 0;
    HARMONIZER::steer(harmonizer);
  }
  const Pair pair = ::sing(harmonizer, in);
  const Float dry = harmonizer.rows[HARMONIZER::DRY];
  Float peak = 0;
  for (Whole channel = 0; channel < harmonizer.channels; ++channel) {
    const Float out =
      lanes[channel][frame] * dry + ::wet(pair, channel, harmonizer.channels);
    lanes[channel][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::PLUGINS::HARMONIZER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &harmonizer = *static_cast<Harmonizer *>(instance);
  const auto take = [&harmonizer](const AUDIO::PLUGIN::Event &event) {
    apply(harmonizer, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const Float size = ::pour(harmonizer, lanes, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(harmonizer.meter, peak);
}
