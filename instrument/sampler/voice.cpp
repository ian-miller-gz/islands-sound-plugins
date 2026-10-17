// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new SAMPLER::Synth{.rate = rate, .channels = channels};
  const Whole pace = rate / SAMPLER::STRIDE;
  synth->pace = pace == 0 ? 1 : pace;
  CORE::TABLE::rest(SAMPLER::SHEET, synth->rows);
  SAMPLER::take(*synth);
  SAMPLER::tone(*synth);
  SAMPLER::prepare(*synth);
  for (SAMPLER::Voice &voice : synth->voices) {
    voice.downs.resize(channels);
    voice.filters.resize(channels);
  }
  SAMPLER::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<SAMPLER::Synth *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<SAMPLER::Synth *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = SAMPLER::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = SAMPLER::Surface::parameters,
  .name = SAMPLER::Surface::name,
  .reading = SAMPLER::reading,
  .held = SAMPLER::Surface::held,
  .control = SAMPLER::control,
  .ins =
    {{AUDIO::PLUGIN::Port::NOTES, "notes"},
     {AUDIO::PLUGIN::Port::CONTROL, "turns"},
     {AUDIO::PLUGIN::Port::DATA, "material", "sample"},
     {AUDIO::PLUGIN::Port::PROGRAM, "programs"}},
  .outs = {
    {AUDIO::PLUGIN::Port::AUDIO, "signal"},
    {AUDIO::PLUGIN::Port::DATA, "material", "sample"}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "sampler",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
