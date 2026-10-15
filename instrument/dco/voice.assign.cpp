// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole SINGLE = 1;
constexpr Whole FIRST = 0;
constexpr Float WHOLE = 1;
constexpr Float HUSHED = 0.40824829f;

struct Assignment {
  Whole mode;
  Whole unison;
  Float level;
};

constexpr Assignment ASSIGNMENTS[] = {
  {CORE::VOICE::POLY, SINGLE, WHOLE},
  {CORE::VOICE::MONO, SINGLE, WHOLE},
  {CORE::VOICE::MONO, DCO::VOICES, HUSHED}};
static_assert(std::size(ASSIGNMENTS) == std::size(DCO::ASSIGNS));

}  // namespace

void SOUND::PLUGINS::DCO::assign(Synth &synth) {
  const Whole at = Whole(synth.rows[ASSIGN]);
  const Assignment &chosen =
    ::ASSIGNMENTS[at < std::size(::ASSIGNMENTS) ? at : ::FIRST];
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.mode = chosen.mode;
  allocator.unison = chosen.unison;
  allocator.glide.time = synth.rows[GLIDE];
  synth.level = chosen.level;
  CORE::VOICE::settle(allocator, synth.rate);
}
