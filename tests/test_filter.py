import math

import pytest

PEAK, ROOT, CROSSINGS, LAST = range(4)
STEP = 0.01
WINDOW = 0.1
CUTOFF = 1000
TENOR_A = [750, 1181.25, 2900, 3500, 4100]
WIDTHS = [70, 87.25, 156, 180, 204]
HERTZ = 0.5
EFFORT = 12


@pytest.fixture(scope='module')
def filtered(harness):
  return harness('filter', 'noise')


def decibels(ratio):
  return 20 * math.log10(ratio)


def test_a_plain_cascade_passes_dc_at_unity(filtered):
  assert 0.98 <= filtered['dc'][LAST] / STEP <= 1.02


def test_a_cascade_stops_a_decade_above_its_cutoff(filtered):
  assert decibels(filtered['stop'][PEAK] / STEP) <= -60


def test_half_emphasis_leaves_a_third_of_dc(filtered):
  assert filtered['half'][LAST] / STEP == pytest.approx(1 / 3, abs=0.03)


def test_bass_restores_the_passband(filtered):
  assert 0.9 <= filtered['lifted'][LAST] / STEP <= 1.1


def test_full_emphasis_sings_at_its_cutoff(filtered):
  ring = filtered['ring']
  assert ring[ROOT] >= 0.01
  assert ring[CROSSINGS] / 2 / WINDOW == pytest.approx(CUTOFF, rel=0.05)


def test_a_driven_cascade_stays_bounded(filtered):
  assert filtered['driven'][PEAK] <= 1.05


def test_no_saturation_is_the_five_argument_settle_bit_for_bit(filtered):
  assert filtered['same'] == [0]


def test_saturation_tames_a_resonant_peak(filtered):
  plain, saturated = filtered['loud']
  assert saturated < plain
  assert saturated <= 3


def test_saturation_leaves_a_faint_signal_alone(filtered):
  plain, saturated = filtered['faint']
  assert saturated == pytest.approx(plain, rel=0.01)


def test_a_tenor_a_is_derived_from_its_tube(filtered):
  assert filtered['centres'] == pytest.approx(TENOR_A, abs=HERTZ)
  assert filtered['widths'] == pytest.approx(WIDTHS, abs=HERTZ)
  assert filtered['levels'][0] == 0
  assert filtered['levels'][1] == pytest.approx(-3.93, abs=0.01)


def test_a_tenor_i_closes_and_fronts(filtered):
  assert filtered['close'][:2] == pytest.approx([275, 2250], abs=HERTZ)


def test_a_voice_between_registers_blends_their_tubes(filtered):
  assert filtered['between'][0] == pytest.approx(795.45, abs=HERTZ)


def test_a_sung_pitch_lifts_the_first_formant(filtered):
  assert filtered['bent'][:2] == pytest.approx([345, 2250], abs=HERTZ)


def test_effort_lifts_the_upper_levels_only(filtered):
  plain, effort = filtered['plain'], filtered['effort']
  assert filtered['kept'] == filtered['bent']
  assert effort[:2] == plain[:2]
  lifts = [lifted - level for lifted, level in zip(effort[2:], plain[2:])]
  assert lifts == pytest.approx([EFFORT] * 3)


def test_a_glide_moves_by_its_pole_and_comes_to_rest(filtered):
  assert filtered['glide'] == [512.5, 1]
  assert filtered['glided'] == [0]


def test_makeup_leaves_white_noise_at_its_own_level(filtered):
  assert abs(decibels(filtered['makeup'][0])) <= 1.5


def test_the_standing_settle_lands_on_tenor(filtered):
  assert filtered['settled'] == pytest.approx(TENOR_A, abs=HERTZ)
