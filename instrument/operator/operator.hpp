// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <utility>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "operator.indices.hpp"
#include "operator.rows.hpp"

namespace SOUND::PLUGINS::OPERATOR {

template <Whole... AT>
inline constexpr CORE::TABLE::Row LISTED[] = {describe(AT)...};

template <Whole... AT>
constexpr auto list(std::index_sequence<AT...>)
  -> const CORE::TABLE::Row (&)[sizeof...(AT)] {
  return LISTED<AT...>;
}

inline constexpr const CORE::TABLE::Row (&ROWS)[PARAMETERS] =
  list(std::make_index_sequence<PARAMETERS>());

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::OPERATOR
