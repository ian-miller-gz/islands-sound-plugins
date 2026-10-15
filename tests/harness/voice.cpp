// SPDX-License-Identifier: AGPL-3.0-or-later
#include <bit>
#include <cstdint>
#include <cstdio>

#include "../../core/voice/voice.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Bits = std::uint32_t;

constexpr Whole SLOTS = 16;
constexpr CORE::MODULATOR::Seed FIRST = 7;
constexpr CORE::MODULATOR::Seed SECOND = 8;

struct Draw {
  CORE::VOICE::Trim trims[SLOTS];
};

auto drawn(CORE::MODULATOR::Seed seed) -> Draw {
  Draw draw;
  CORE::VOICE::draw(draw.trims, seed);
  return draw;
}

auto same(const CORE::VOICE::Trim &one, const CORE::VOICE::Trim &two) -> Flag {
  return std::bit_cast<Bits>(one.pitch) == std::bit_cast<Bits>(two.pitch) &&
         std::bit_cast<Bits>(one.cutoff) == std::bit_cast<Bits>(two.cutoff) &&
         std::bit_cast<Bits>(one.time) == std::bit_cast<Bits>(two.time);
}

void show(const Draw &draw) {
  for (const CORE::VOICE::Trim &trim : draw.trims) {
    std::printf("pitch %.9g\n", double(trim.pitch));
    std::printf("cutoff %.9g\n", double(trim.cutoff));
    std::printf("time %.9g\n", double(trim.time));
  }
}

void repeat(const Draw &draw) {
  const Draw again = ::drawn(FIRST);
  Whole differing = 0;
  for (Whole slot = 0; slot < SLOTS; ++slot)
    differing += !::same(draw.trims[slot], again.trims[slot]);
  std::printf("differing %zu\n", differing);
}

void part(const Draw &draw) {
  const Draw other = ::drawn(SECOND);
  std::printf("apart %d\n", !::same(draw.trims[0], other.trims[0]));
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::VOICE {

void run() {
  const ::Draw draw = ::drawn(FIRST);
  ::show(draw);
  ::repeat(draw);
  ::part(draw);
}

}  // namespace SOUND::PLUGINS::HARNESS::VOICE
