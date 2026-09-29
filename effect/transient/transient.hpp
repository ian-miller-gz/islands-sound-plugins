// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::TRANSIENT {

constexpr Whole ATTACK = 0;
constexpr Whole SUSTAIN = 1;
constexpr Whole PARAMETERS = 2;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Attack", "dB", 0, -24, 24, 0, nullptr},
  {"Sustain", "dB", 0, -24, 24, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::TRANSIENT
