// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::FORMANT::build(Formant &formant) {
  formant.shifters.resize(formant.channels);
  for (CORE::PITCH::Formant &shifter : formant.shifters)
    CORE::PITCH::build(shifter, WINDOW, formant.rate);
}

void SOUND::FORMANT::settle(Formant &formant) {
  const Float pitch = formant.rows[PITCH] * CENT;
  const Float shift = formant.rows[SHIFT] * CENT;
  for (CORE::PITCH::Formant &shifter : formant.shifters)
    CORE::PITCH::settle(shifter, pitch, shift, WINDOW, formant.rate);
}

void SOUND::FORMANT::apply(
  Formant &formant, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  formant.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(formant);
}
