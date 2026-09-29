// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float ONE = 1.0f;

auto process(BUS::Bus &bus, Whole lane, Float in, Float squeezed) -> Float {
  const Float mix = bus.rows[BUS::MIX];
  const Float glued = in * (::ONE - mix + mix * squeezed);
  const Float push = bus.push;
  const Float taped = CORE::SHAPER::tick(
    bus.tapes[lane], glued,
    [push](Float sample) { return CORE::SHAPER::soft(sample * push) / push; });
  return CORE::FILTER::tick(bus.tilts[lane], taped) * bus.trim;
}

auto frame(BUS::Bus &bus, AUDIO::PLUGIN::Sample *const *lanes, Whole frame)
  -> Float {
  const Float heard = CORE::BLOCK::loudest(lanes, bus.channels, frame);
  const Float lift = BUS::shape(bus, heard);
  const Float squeezed = BUS::squeeze(bus, heard * lift);
  Float peak = 0;
  for (Whole lane = 0; lane < bus.channels; ++lane) {
    const Float out = ::process(bus, lane, lanes[lane][frame] * lift, squeezed);
    lanes[lane][frame] = out;
    const Float size = CORE::BLOCK::magnitude(out);
    if (size > peak) peak = size;
  }
  return peak;
}

}  // namespace

void SOUND::BUS::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &bus = *static_cast<Bus *>(instance);
  const auto take = [&bus](const AUDIO::PLUGIN::Event &event) {
    apply(bus, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole at = 0; at < frames; ++at) {
    CORE::BLOCK::due(cursor, events, count, at, take);
    const Float size = ::frame(bus, lanes, at);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(bus.meter, peak);
}
