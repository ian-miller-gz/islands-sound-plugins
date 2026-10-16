// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/filter/filter.hpp"

namespace SOUND::PLUGINS::WHISPER {

constexpr Whole WHITE = 0;
constexpr Whole PINK = 1;
constexpr Whole BROWN = 2;
constexpr Whole BLUE = 3;
constexpr Whole COLOURS = 4;

constexpr Whole STEPS = CORE::FILTER::VOWELS - 1;
constexpr Float LAST = Float(STEPS);
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 4.0f;
constexpr Float STILLEST = 0.01f;
constexpr Float RESTLESS = 2.0f;
constexpr Float NARROWEST = 0.5f;
constexpr Float WIDEST = 4.0f;
constexpr Float TRAVEL = 2.0f;

inline constexpr STRING::Hot NOISES[] = {"White", "Pink", "Brown", "Blue"};
static_assert(std::size(NOISES) == COLOURS);

inline constexpr STRING::Hot VOWELS[] = {"A", "E", "I", "O", "U"};
static_assert(std::size(VOWELS) == CORE::FILTER::VOWELS);

inline constexpr STRING::Hot VOICES[] = {"Bass", "Tenor", "Alto", "Soprano"};
static_assert(std::size(VOICES) == CORE::FILTER::REGISTERS);

inline constexpr STRING::Hot MODES[] = {"Poly", "Mono", "Legato"};

constexpr Whole VOICINGS = std::size(MODES) - 1;

}  // namespace SOUND::PLUGINS::WHISPER
