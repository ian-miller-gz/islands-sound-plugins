// SPDX-License-Identifier: AGPL-3.0-or-later
#include "quantizer.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void lift(
  QUANTIZER::Quantizer &quantizer, Whole key, Whole offset,
  CORE::NOTES::Out &out) {
  Whole &pitch = quantizer.sounding[key];
  if (pitch == CORE::NOTES::SILENT) return;
  CORE::NOTES::lift(quantizer.tally, out, pitch, offset);
  pitch = CORE::NOTES::SILENT;
}

void strike(
  QUANTIZER::Quantizer &quantizer, const AUDIO::PLUGIN::Event &event,
  CORE::NOTES::Out &out) {
  ::lift(quantizer, event.index, event.offset, out);
  const Whole pitch = QUANTIZER::snapped(quantizer, event.index);
  if (pitch == CORE::NOTES::SILENT) return;
  quantizer.sounding[event.index] = pitch;
  CORE::NOTES::strike(quantizer.tally, out, pitch, event.offset, event.value);
}

void turn(QUANTIZER::Quantizer &quantizer, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= QUANTIZER::PARAMETERS) return;
  quantizer.rows[event.index] =
    CORE::TABLE::clamped(QUANTIZER::SHEET, event.index, event.value);
}

}  // namespace

void SOUND::PLUGINS::QUANTIZER::apply(
  Quantizer &quantizer, const AUDIO::PLUGIN::Event &event,
  CORE::NOTES::Out &out) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER)
    return ::turn(quantizer, event);
  const Flag struck = CORE::NOTES::struck(event);
  if (!struck && !CORE::NOTES::lifted(event)) {
    CORE::NOTES::put(out, event);
    return;
  }
  if (event.index > CORE::NOTES::HIGHEST) return;
  if (struck) return ::strike(quantizer, event, out);
  ::lift(quantizer, event.index, event.offset, out);
}
