// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<POLYMOD::Synth, POLYMOD::SHEET>;

constexpr CORE::MODULATOR::Seed WALK = 0x2F6B1D35u;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new POLYMOD::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(POLYMOD::SHEET, synth->rows);
  CORE::LINE::build(synth->chorus, rate);
  CORE::VOICE::draw(synth->trims, CORE::MODULATOR::SEED);
  for (Whole at = 0; at < POLYMOD::VOICES; ++at)
    CORE::MODULATOR::seed(synth->voices[at].drift, WALK * (at + 1));
  POLYMOD::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<POLYMOD::Synth *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<POLYMOD::Synth *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = POLYMOD::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "polymod",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
