// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole FIRST = 0;
constexpr Float UNIT = 1.0f;

auto at(
  const OSCILLATOR::Module &module, AUDIO::PLUGIN::Sample *const *lanes,
  Whole in, Whole frame) -> Float {
  return CORE::BLOCK::lane(lanes, in, FIRST, module.channels)[frame];
}

auto edged(OSCILLATOR::Module &module, Float sync) -> Float {
  const Flag rose =
    CORE::BLOCK::crossed(module.edge, sync) == CORE::BLOCK::ROSE;
  return rose ? module.edge.since : CORE::OSCILLATOR::NONE;
}

}  // namespace

auto SOUND::PLUGINS::OSCILLATOR::read(
  Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) -> Ins {
  const Float pitch = ::at(module, lanes, IN::PITCH, frame);
  const Float fm = ::at(module, lanes, IN::FM, frame);
  const Float sync = ::at(module, lanes, IN::SYNC, frame);
  return {pitch, fm, ::edged(module, sync)};
}

auto SOUND::PLUGINS::OSCILLATOR::pitched(Module &module, const Ins &ins)
  -> Float {
  const Float *rows = module.rows;
  CORE::VOICE::Note &lead = module.allocator.notes[LEAD];
  const Float key = CORE::VOICE::tick(lead, module.allocator.glide);
  return key + rows[COARSE] + rows[FINE] / CENTS + rows[PITCH] * ins.pitch;
}

auto SOUND::PLUGINS::OSCILLATOR::modulated(
  const Module &module, Float pitch, Float fm) -> Float {
  const Float depth = UNIT + module.rows[INDEX] * fm;
  const Float hertz = CORE::PHASE::hertz(pitch) * depth;
  return hertz < 0 ? 0 : hertz;
}
