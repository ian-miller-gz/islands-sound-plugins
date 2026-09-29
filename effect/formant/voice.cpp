// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<FORMANT::Formant, FORMANT::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *formant = new FORMANT::Formant{.rate = rate, .channels = channels};
  FORMANT::build(*formant);
  CORE::TABLE::rest(FORMANT::SHEET, formant->rows);
  FORMANT::settle(*formant);
  return formant;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<FORMANT::Formant *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<FORMANT::Formant *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = FORMANT::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::AUDIO}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered =
  PLUGIN::offer({.name = "formant", .surface = &surface});

}  // namespace
