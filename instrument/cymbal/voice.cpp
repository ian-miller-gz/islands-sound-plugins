// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<CYMBAL::Voice, CYMBAL::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *voice = new CYMBAL::Voice{.rate = rate, .channels = channels};
  CORE::TABLE::rest(CYMBAL::SHEET, voice->rows);
  CYMBAL::settle(*voice);
  return voice;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<CYMBAL::Voice *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<CYMBAL::Voice *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = CYMBAL::render,
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
  PLUGIN::offer({.name = "cymbal", .surface = &surface});

}  // namespace
