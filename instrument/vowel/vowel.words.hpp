// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::VOWEL {

constexpr Whole THROATS = 8;
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 4.0f;
constexpr Float LONGEST = 2.0f;
constexpr Float WANDER = 20.0f;

namespace VIBRATO {
constexpr Float SLOWEST = 0.1f;
constexpr Float FASTEST = 12.0f;
constexpr Float WIDEST = 100.0f;
}  // namespace VIBRATO

namespace SWAY {
constexpr Float SLOWEST = 0.01f;
constexpr Float FASTEST = 2.0f;
}  // namespace SWAY

inline constexpr STRING::Hot VOWELS[] = {"A", "E", "I", "O", "U"};
inline constexpr STRING::Hot VOICES[] = {"Bass", "Tenor", "Alto", "Soprano"};
inline constexpr STRING::Hot MODES[] = {"Poly", "Mono", "Legato"};
static_assert(std::size(VOWELS) == CORE::FILTER::VOWELS);
static_assert(std::size(VOICES) == CORE::FILTER::REGISTERS);

template <Whole COUNT>
constexpr auto steps(const STRING::Hot (&)[COUNT]) -> Whole {
  return COUNT - 1;
}

template <Whole COUNT>
constexpr auto last(const STRING::Hot (&words)[COUNT]) -> Float {
  return Float(steps(words));
}

}  // namespace SOUND::PLUGINS::VOWEL
