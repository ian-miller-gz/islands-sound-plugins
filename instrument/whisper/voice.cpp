// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<WHISPER::Mouth, WHISPER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *mouth = new WHISPER::Mouth{.rate = rate, .channels = channels};
  mouth->allocator.count = WHISPER::VOICES;
  for (Whole at = 0; at < WHISPER::VOICES; ++at)
    WHISPER::seed(
      mouth->hisses[at],
      CORE::NOISE::SEED ^ (CORE::NOISE::Register(at + 1) * WHISPER::SCATTER));
  CORE::TABLE::rest(WHISPER::SHEET, mouth->rows);
  WHISPER::settle(*mouth);
  return mouth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<WHISPER::Mouth *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<WHISPER::Mouth *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = WHISPER::render,
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
  {.name = "whisper",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
