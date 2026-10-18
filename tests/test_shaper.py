import pytest

POINTS = 7
SLOPE = 0.001
LIMIT = 1.001


@pytest.fixture(scope='module')
def shaper(harness):
  return harness('shaper')


def test_the_hard_area_rises_at_the_hard_curve(shaper):
  assert shaper['hard'] == pytest.approx([0] * POINTS, abs=SLOPE)


def test_the_fold_area_rises_at_the_fold_curve(shaper):
  assert shaper['fold'] == pytest.approx([0] * POINTS, abs=SLOPE)


def test_both_areas_start_at_zero(shaper):
  assert shaper['origin'] == pytest.approx([0, 0])


def test_a_step_reads_the_mean_of_the_clip_over_it(shaper):
  assert shaper['clip'] == pytest.approx([2.5 / 3])


def test_a_whole_fold_period_means_zero(shaper):
  assert shaper['period'] == pytest.approx([0], abs=1e-6)


def test_a_near_step_falls_back_to_the_curve_at_its_middle(shaper):
  assert shaper['near'] == pytest.approx([0], abs=1e-6)


def test_a_loud_fast_sweep_stays_within_the_curves_range(shaper):
  assert shaper['clipped'][0] <= LIMIT
  assert shaper['folded'][0] <= LIMIT
