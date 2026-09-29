// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;
using CORE::OSCILLATOR::Oscillator;
using CORE::OSCILLATOR::Sync;

auto blend(const Oscillator &oscillator, Float shape) -> Float {
  const Float saw = CORE::OSCILLATOR::saw(oscillator);
  return saw + (CORE::OSCILLATOR::pulse(oscillator) - saw) * shape;
}

auto slave(Sync &sync, const Oscillator &master, Whole wave, Flag synced)
  -> Float {
  if (synced) return CORE::OSCILLATOR::tick(sync, master, wave);
  sync.pending = 0;
  return CORE::OSCILLATOR::tick(sync.slave, wave);
}

}  // namespace

auto SOUND::MULTIMODE::sing(
  Synth &synth, Voice &voice, Float pitch, Float width) -> Float {
  const Float *rows = synth.rows;
  const Float hertz = CORE::PHASE::hertz(pitch + rows[FREQUENCY1]);
  const Float other = CORE::PHASE::hertz(pitch + rows[FREQUENCY2]);
  Oscillator &master = voice.first;
  CORE::OSCILLATOR::settle(master, hertz, width, synth.rate);
  for (Sync *sync : {&voice.saw, &voice.pulse})
    CORE::OSCILLATOR::settle(sync->slave, other, width, synth.rate);
  const Float one = ::blend(master, rows[SHAPE1]);
  master.phase += master.step;
  const Flag synced = rows[SYNC] > 0;
  const Float saw = ::slave(voice.saw, master, CORE::OSCILLATOR::SAW, synced);
  const Float pulse =
    ::slave(voice.pulse, master, CORE::OSCILLATOR::PULSE, synced);
  const Float two = saw + (pulse - saw) * rows[SHAPE2];
  return rows[LEVEL1] * one + rows[LEVEL2] * two;
}
