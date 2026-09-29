// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<CONGA::Voice, CONGA::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *voice = new CONGA::Voice{.rate = rate, .channels = channels};
  CORE::TABLE::rest(CONGA::SHEET, voice->rows);
  CONGA::settle(*voice);
  return voice;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<CONGA::Voice *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<CONGA::Voice *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = CONGA::render,
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
  PLUGIN::offer({.name = "conga", .surface = &surface});

}  // namespace
