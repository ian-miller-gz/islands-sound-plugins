// SPDX-License-Identifier: AGPL-3.0-or-later
#include "oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float UNIT = 1.0f;
constexpr Float HALF = 0.5f;

auto jump(const OSCILLATOR::Oscillator &slave, OSCILLATOR::Wheel at, Whole wave)
  -> Float {
  return OSCILLATOR::NAIVE::shaped(0, slave.edge, wave) -
         OSCILLATOR::NAIVE::shaped(at, slave.edge, wave);
}

}  // namespace

auto SOUND::PLUGINS::CORE::OSCILLATOR::tick(
  Sync &sync, const Oscillator &master, Whole wave) -> Float {
  Oscillator &slave = sync.slave;
  Float value = shaped(slave, wave) + sync.pending;
  sync.pending = 0;
  slave.phase += slave.step;
  if (master.step == 0 || master.phase >= master.step) return value;
  const Float since = Float(master.phase) / Float(master.step);
  const Float until = UNIT - since;
  const Wheel at = slave.phase - Wheel(Float(slave.step) * since);
  const Float step = ::jump(slave, at, wave);
  const Float natural = ::jump(slave, Wheel(0) - 1, wave);
  value += step * HALF * since * since;
  sync.pending = -(step - natural) * HALF * until * until;
  slave.phase = Wheel(Float(slave.step) * since);
  return value;
}
