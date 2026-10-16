// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float LOWEST = 48.0f;
constexpr Float BAND = 8.0f;
constexpr Float TOP = Float(CORE::FILTER::REGISTERS - 1);

auto fixed(Float voice, Float) -> Float { return voice; }

auto keyed(Float, Float key) -> Float {
  const Float place = (key - ::LOWEST) / ::BAND;
  return place < 0 ? 0 : place > ::TOP ? ::TOP : place;
}

using Placer = auto (*)(Float voice, Float key) -> Float;

constexpr Placer PLACERS[] = {fixed, fixed, fixed, fixed, keyed};
static_assert(std::size(PLACERS) == std::size(CHOIR::VOICES));

}  // namespace

auto SOUND::PLUGINS::CHOIR::cast(
  const Choir &choir, const CORE::VOICE::Note &note) -> Float {
  const Whole voice = Whole(choir.rows[VOICE]);
  const Whole at = voice < std::size(::PLACERS) ? voice : MIXED;
  return ::PLACERS[at](Float(voice), Float(note.pitch));
}
