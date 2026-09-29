// SPDX-License-Identifier: AGPL-3.0-or-later
#include "chord.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<CHORD::Chord, CHORD::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *chord = new CHORD::Chord{};
  CORE::TABLE::rest(CHORD::SHEET, chord->rows);
  for (Whole *pitches : chord->voicings)
    for (Whole voice = 0; voice < CHORD::VOICES; ++voice)
      pitches[voice] = CORE::NOTES::SILENT;
  return chord;
}

void destroy(void *instance) { delete static_cast<CHORD::Chord *>(instance); }

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
  {.name = "chord", .surface = &surface, .answer = CHORD::answer});

}  // namespace

auto SOUND::CHORD::answer(
  void *instance, const AUDIO::PLUGIN::Event *events, Whole count,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole {
  auto &chord = *static_cast<Chord *>(instance);
  CORE::NOTES::Out sent = {out, room, 0};
  for (Whole at = 0; at < count; ++at) apply(chord, events[at], sent);
  return sent.written;
}
