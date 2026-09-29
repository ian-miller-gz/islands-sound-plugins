// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<VOCODER::Vocoder, VOCODER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *vocoder = new VOCODER::Vocoder{.rate = rate, .channels = channels};
  VOCODER::build(*vocoder);
  CORE::TABLE::rest(VOCODER::SHEET, vocoder->rows);
  VOCODER::settle(*vocoder);
  return vocoder;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<VOCODER::Vocoder *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<VOCODER::Vocoder *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = VOCODER::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins =
    {{AUDIO::PLUGIN::Port::AUDIO},
     {AUDIO::PLUGIN::Port::NOTES},
     {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "vocoder", .type = "effect", .surface = &surface});

}  // namespace
