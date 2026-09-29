// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<RHYTHM::Box, RHYTHM::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *box = new RHYTHM::Box{.rate = rate, .channels = channels};
  CORE::TABLE::rest(RHYTHM::SHEET, box->rows);
  RHYTHM::settle(*box);
  return box;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<RHYTHM::Box *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<RHYTHM::Box *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = RHYTHM::render,
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
  {.name = "rhythm",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
