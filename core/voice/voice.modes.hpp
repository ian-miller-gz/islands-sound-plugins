// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "voice.hpp"

namespace SOUND::PLUGINS::CORE::VOICE::POLYPHONY {
void strike(Allocator &allocator, Whole pitch, Float velocity);
void lift(Allocator &allocator, Whole pitch);
}  // namespace SOUND::PLUGINS::CORE::VOICE::POLYPHONY

namespace SOUND::PLUGINS::CORE::VOICE::MONOPHONY {
auto choose(const Allocator &allocator, Whole priority) -> Whole;
auto choose(const Allocator &allocator) -> Whole;
void strike(Allocator &allocator, Whole pitch, Float velocity);
void lift(Allocator &allocator, Whole pitch);
}  // namespace SOUND::PLUGINS::CORE::VOICE::MONOPHONY

namespace SOUND::PLUGINS::CORE::VOICE::DUOPHONY {
void strike(Allocator &allocator, Whole pitch, Float velocity);
void lift(Allocator &allocator, Whole pitch);
}  // namespace SOUND::PLUGINS::CORE::VOICE::DUOPHONY
