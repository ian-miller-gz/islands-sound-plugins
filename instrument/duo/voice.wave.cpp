// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;
using CORE::OSCILLATOR::Oscillator;

constexpr Whole FORMS[] = {CORE::OSCILLATOR::SAW, CORE::OSCILLATOR::PULSE};
static_assert(std::size(FORMS) == std::size(DUO::WAVES));
static_assert(std::size(FORMS) == std::size(DUO::PULSES));

auto form(const DUO::Synth &synth, Whole index) -> Whole {
  return FORMS[Whole(synth.rows[index])];
}

auto slave(DUO::Synth &synth, Float &ring) -> Float {
  CORE::OSCILLATOR::Sync &sync = synth.second;
  Oscillator &oscillator = sync.slave;
  const Whole wave = ::form(synth, DUO::WAVE2);
  if (synth.rows[DUO::SYNC] > 0) {
    ring = CORE::OSCILLATOR::NAIVE::pulse(oscillator.phase, oscillator.edge);
    return CORE::OSCILLATOR::tick(sync, synth.first, wave);
  }
  sync.pending = 0;
  ring = CORE::OSCILLATOR::pulse(oscillator);
  return CORE::OSCILLATOR::tick(oscillator, wave);
}

}  // namespace

auto SOUND::DUO::sing(Synth &synth, Float first, Float second, Float width)
  -> Tones {
  const Float *rows = synth.rows;
  Oscillator &master = synth.first;
  CORE::OSCILLATOR::settle(
    master, CORE::PHASE::hertz(first), width, synth.rate);
  CORE::OSCILLATOR::settle(
    synth.second.slave, CORE::PHASE::hertz(second), width, synth.rate);
  const Float saw = CORE::OSCILLATOR::saw(master);
  const Float square = CORE::OSCILLATOR::pulse(master);
  const Float one = CORE::OSCILLATOR::tick(master, ::form(synth, WAVE1));
  Float ring = 0;
  const Float two = ::slave(synth, ring);
  const Float mixed =
    rows[LEVEL1] * one + rows[LEVEL2] * two + rows[RING] * square * ring;
  return {mixed, saw};
}
