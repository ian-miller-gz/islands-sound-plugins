// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::OSCILLATOR::Oscillator;
using CORE::OSCILLATOR::Sync;

constexpr Float CENT = 100;
constexpr Float DROP = 72;
constexpr Float SWING = 36;
constexpr Float SPAN = POLYMOD::SQUARE - POLYMOD::NARROW;

auto slave(Sync &sync, const Oscillator &master, Whole wave, Flag synced)
  -> Float {
  if (synced) return CORE::OSCILLATOR::tick(sync, master, wave);
  sync.pending = 0;
  return CORE::OSCILLATOR::tick(sync.slave, wave);
}

auto mixed(const Float *rows, const Oscillator &master) -> Float {
  const Float saw = CORE::OSCILLATOR::saw(master);
  const Float triangle = CORE::OSCILLATOR::triangle(master);
  const Float pulse = CORE::OSCILLATOR::pulse(master);
  return rows[POLYMOD::SAWB] * saw + rows[POLYMOD::TRIANGLEB] * triangle +
         rows[POLYMOD::PULSEB] * pulse;
}

}  // namespace

auto SOUND::PLUGINS::POLYMOD::modulate(Synth &synth, Voice &voice, Float pitch)
  -> Float {
  const Float *rows = synth.rows;
  const Float low = rows[LOW] > 0 ? DROP : 0;
  const Float tuned = pitch + rows[FREQUENCYB] + rows[FINE] / CENT - low;
  Oscillator &master = voice.master;
  CORE::OSCILLATOR::settle(
    master, CORE::PHASE::hertz(tuned), rows[WIDTHB], synth.rate);
  const Float out = ::mixed(rows, master);
  master.phase += master.step;
  return out;
}

auto SOUND::PLUGINS::POLYMOD::sing(
  Synth &synth, Voice &voice, Float pitch, Float mod) -> Float {
  const Float *rows = synth.rows;
  const Float bent = rows[PITCH] > 0 ? SWING * mod : 0;
  const Float narrowed = rows[DUTY] > 0 ? SPAN * mod : 0;
  const Float hertz = CORE::PHASE::hertz(pitch + rows[FREQUENCYA] + bent);
  for (Sync *sync : {&voice.saw, &voice.pulse})
    CORE::OSCILLATOR::settle(
      sync->slave, hertz, rows[WIDTHA] - narrowed, synth.rate);
  const Flag synced = rows[SYNC] > 0;
  const Oscillator &master = voice.master;
  const Float saw = ::slave(voice.saw, master, CORE::OSCILLATOR::SAW, synced);
  const Float pulse =
    ::slave(voice.pulse, master, CORE::OSCILLATOR::PULSE, synced);
  return rows[SAWA] * saw + rows[PULSEA] * pulse;
}
