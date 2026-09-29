// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<KICK::Voice, KICK::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *voice = new KICK::Voice{.rate = rate, .channels = channels};
  CORE::TABLE::rest(KICK::SHEET, voice->rows);
  KICK::settle(*voice);
  return voice;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<KICK::Voice *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<KICK::Voice *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = KICK::render,
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
  PLUGIN::offer({.name = "kick", .surface = &surface});

}  // namespace
