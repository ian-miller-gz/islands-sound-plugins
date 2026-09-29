// SPDX-License-Identifier: AGPL-3.0-or-later
#include "quantizer.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<QUANTIZER::Quantizer, QUANTIZER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *quantizer = new QUANTIZER::Quantizer{};
  CORE::TABLE::rest(QUANTIZER::SHEET, quantizer->rows);
  for (Whole &pitch : quantizer->sounding) pitch = CORE::NOTES::SILENT;
  return quantizer;
}

void destroy(void *instance) {
  delete static_cast<QUANTIZER::Quantizer *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = nullptr,
  .meter = nullptr,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::NOTES}}};

[[maybe_unused]] const Flag offered = PLUGIN::offer(
  {.name = "quantizer", .surface = &surface, .answer = QUANTIZER::answer});

}  // namespace

auto SOUND::QUANTIZER::answer(
  void *instance, const AUDIO::PLUGIN::Event *events, Whole count,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole {
  auto &quantizer = *static_cast<Quantizer *>(instance);
  CORE::NOTES::Out sent = {out, room, 0};
  for (Whole at = 0; at < count; ++at) apply(quantizer, events[at], sent);
  return sent.written;
}
