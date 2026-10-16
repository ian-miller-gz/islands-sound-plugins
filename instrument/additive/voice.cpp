// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<ADDITIVE::Synth, ADDITIVE::SHEET>;

constexpr Float TAU = 6.2831853f;
constexpr Whole NOTES = 128;

auto increment(Whole pitch, Whole rate) -> uint32_t {
  const Float hertz = 440.0f * std::pow(2.0f, (Float(pitch) - 69.0f) / 12.0f);
  const Float turns = hertz / Float(rate);
  return static_cast<uint32_t>(turns * 4294967296.0f);
}

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new ADDITIVE::Synth{.rate = rate, .channels = channels};
  synth->table.resize(ADDITIVE::TABLE);
  for (Whole slot = 0; slot < ADDITIVE::TABLE; ++slot)
    synth->table[slot] = std::sin(TAU * Float(slot) / Float(ADDITIVE::TABLE));
  synth->steps.resize(NOTES);
  for (Whole pitch = 0; pitch < NOTES; ++pitch)
    synth->steps[pitch] = increment(pitch, rate);
  CORE::TABLE::rest(ADDITIVE::SHEET, synth->rows);
  ADDITIVE::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  auto &synth = *static_cast<ADDITIVE::Synth *>(instance);
  return synth.level[synth.face.load(std::memory_order_acquire)];
}

void destroy(void *instance) {
  delete static_cast<ADDITIVE::Synth *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = ADDITIVE::render,
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
  {.name = "additive",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
