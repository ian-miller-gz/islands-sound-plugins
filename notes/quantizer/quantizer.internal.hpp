// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "quantizer.hpp"
#include "../../core/notes/notes.hpp"

namespace SOUND::QUANTIZER {

struct Quantizer {
  Float rows[PARAMETERS] = {};
  Whole sounding[CORE::NOTES::KEYS] = {};
  CORE::NOTES::Tally tally;
};

auto snapped(const Quantizer &quantizer, Whole pitch) -> Whole;
void apply(
  Quantizer &quantizer, const AUDIO::PLUGIN::Event &event,
  CORE::NOTES::Out &out);

auto answer(
  void *instance, const AUDIO::PLUGIN::Event *events, Whole count,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole;

}  // namespace SOUND::QUANTIZER
