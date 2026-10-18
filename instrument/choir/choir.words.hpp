// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/reverb/reverb.hpp"

namespace SOUND::PLUGINS::CHOIR {

constexpr Whole STEPS = CORE::FILTER::VOWELS - 1;
constexpr Float LAST = Float(STEPS);
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 4.0f;
constexpr Float STILLEST = 0.01f;
constexpr Float RESTLESS = 2.0f;
constexpr Float WIDEST = 50.0f;
constexpr Float LATEST = 0.25f;
constexpr Float WANDER = 20.0f;
constexpr Float TRAVEL = 2.0f;
constexpr Float BRIEFEST = 0.3f;
constexpr Float LONGEST = 8.0f;
constexpr Whole SEATS = 8;
constexpr Whole FEWEST = 1;

namespace VIBRATO {
constexpr Float SLOWEST = 0.1f;
constexpr Float FASTEST = 12.0f;
constexpr Float WIDEST = 100.0f;
}  // namespace VIBRATO

inline constexpr STRING::Hot VOWELS[] = {"A", "E", "I", "O", "U"};
static_assert(std::size(VOWELS) == CORE::FILTER::VOWELS);

inline constexpr STRING::Hot VOICES[] = {
  "Bass", "Tenor", "Alto", "Soprano", "Mixed"};
static_assert(std::size(VOICES) == CORE::FILTER::REGISTERS + 1);

inline constexpr STRING::Hot MODES[] = {"Poly", "Mono", "Legato"};

constexpr Whole MIXED = CORE::FILTER::REGISTERS;
constexpr Whole VOICINGS = std::size(MODES) - 1;
constexpr Whole SECTIONS = SEATS - FEWEST;

}  // namespace SOUND::PLUGINS::CHOIR
