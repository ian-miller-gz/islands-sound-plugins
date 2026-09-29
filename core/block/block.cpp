// SPDX-License-Identifier: AGPL-3.0-or-later
#include <xmmintrin.h>

#include "block.hpp"

namespace {

constexpr Float FULL = 1.0f;

}  // namespace

void SOUND::CORE::BLOCK::denormals() {
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
}

void SOUND::CORE::BLOCK::publish(Meter &meter, Float peak) {
  const Whole idle = meter.face.load(std::memory_order_relaxed) ^ 1u;
  meter.level[idle] = peak;
  meter.face.store(idle, std::memory_order_release);
}

auto SOUND::CORE::BLOCK::read(const Meter &meter) -> Float {
  return meter.level[meter.face.load(std::memory_order_acquire)];
}

auto SOUND::CORE::BLOCK::magnitude(Float value) -> Float {
  return value < 0 ? -value : value;
}

auto SOUND::CORE::BLOCK::clipped(Float value) -> Float {
  return value < -FULL ? -FULL : value > FULL ? FULL : value;
}

auto SOUND::CORE::BLOCK::loudest(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame) -> Float {
  Float heard = 0;
  for (Whole channel = 0; channel < channels; ++channel) {
    const Float size = magnitude(lanes[channel][frame]);
    if (size > heard) heard = size;
  }
  return heard;
}
