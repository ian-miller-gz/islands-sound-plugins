// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<MONO::Synth, MONO::SHEET>;

constexpr CORE::MODULATOR::Seed WALK = 0x2545F491u;
constexpr CORE::MODULATOR::Seed ROAM = 0x6C8E9CF5u;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new MONO::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(MONO::SHEET, synth->rows);
  CORE::MODULATOR::seed(synth->drift, WALK);
  CORE::MODULATOR::seed(synth->wander, ROAM);
  MONO::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<MONO::Synth *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<MONO::Synth *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = MONO::render,
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
  {.name = "mono",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::MONO,
   .surface = &surface});

}  // namespace
