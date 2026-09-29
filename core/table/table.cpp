// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "table.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr STRING::Hot SWITCH[] = {"Off", "On"};

auto words(const TABLE::Row &row) -> const STRING::Hot * {
  if (row.words != nullptr) return row.words;
  return row.steps == 1 ? SWITCH : nullptr;
}

auto place(const TABLE::Row &row, Float value) -> Whole {
  const Float span = (row.most - row.least) / Float(row.steps);
  const Float places = (value - row.least) / span + 0.5f;
  const Whole at = Whole(places < 0 ? 0 : places);
  return at > row.steps ? row.steps : at;
}

auto snapped(const TABLE::Row &row, Float value) -> Float {
  const Float span = (row.most - row.least) / Float(row.steps);
  return row.least + span * Float(::place(row, value));
}

}  // namespace

auto SOUND::PLUGINS::CORE::TABLE::found(const Sheet &sheet, Whole index)
  -> const Row * {
  return index < sheet.count ? &sheet.rows[index] : nullptr;
}

auto SOUND::PLUGINS::CORE::TABLE::label(const Sheet &sheet, Whole index)
  -> String {
  const Row *row = found(sheet, index);
  return row == nullptr ? String() : String(row->name);
}

auto SOUND::PLUGINS::CORE::TABLE::resting(const Sheet &sheet, Whole index)
  -> Float {
  const Row *row = found(sheet, index);
  return row == nullptr ? 0 : row->resting;
}

auto SOUND::PLUGINS::CORE::TABLE::clamped(
  const Sheet &sheet, Whole index, Float value) -> Float {
  const Row *row = found(sheet, index);
  if (row == nullptr) return value;
  const Float held = value < row->least  ? row->least
                     : value > row->most ? row->most
                                         : value;
  return row->steps == 0 ? held : ::snapped(*row, held);
}

auto SOUND::PLUGINS::CORE::TABLE::notation(
  const Sheet &sheet, Whole index, Float value) -> String {
  const Row *row = found(sheet, index);
  if (row == nullptr) return {};
  const STRING::Hot *named = ::words(*row);
  if (row->steps != 0 && named != nullptr)
    return String(named[::place(*row, clamped(sheet, index, value))]);
  char buffer[DIGITS];
  const char *shape = row->most >= COARSE ? "%.0f%s" : "%.2f%s";
  std::snprintf(
    buffer, sizeof(buffer), shape, static_cast<double>(value), row->unit);
  return buffer;
}

auto SOUND::PLUGINS::CORE::TABLE::control(
  const Sheet &sheet, Whole index, AUDIO::PLUGIN::Control &out) -> Flag {
  const Row *row = found(sheet, index);
  if (row == nullptr) return false;
  Vector<String> positions;
  const STRING::Hot *named = ::words(*row);
  if (row->steps != 0 && named != nullptr)
    positions = {named, named + row->steps + 1};
  out = {row->unit, row->steps, positions, row->resting, row->least, row->most};
  return true;
}

void SOUND::PLUGINS::CORE::TABLE::rest(const Sheet &sheet, Float *rows) {
  for (Whole index = 0; index < sheet.count; ++index)
    rows[index] = sheet.rows[index].resting;
}
