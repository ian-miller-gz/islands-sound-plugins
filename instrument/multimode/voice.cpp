// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<MULTIMODE::Synth, MULTIMODE::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new MULTIMODE::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(MULTIMODE::SHEET, synth->rows);
  MULTIMODE::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<MULTIMODE::Synth *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<MULTIMODE::Synth *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = MULTIMODE::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered =
  PLUGIN::offer({.name = "multimode", .surface = &surface});

}  // namespace
