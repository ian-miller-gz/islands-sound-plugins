// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "../../core/line/line.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto mean(const Vector<double> &values) -> double {
  double sum = 0;
  for (const double value : values) sum += value;
  return sum / double(values.size());
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::LINE {

auto correlation(const Vector<double> &left, const Vector<double> &right)
  -> double {
  const double middle = ::mean(left), centre = ::mean(right);
  double cross = 0, lefts = 0, rights = 0;
  for (Whole at = 0; at < left.size(); ++at) {
    const double one = left[at] - middle, two = right[at] - centre;
    cross += one * two;
    lefts += one * one;
    rights += two * two;
  }
  return cross / std::sqrt(lefts * rights);
}

}  // namespace SOUND::PLUGINS::HARNESS::LINE
