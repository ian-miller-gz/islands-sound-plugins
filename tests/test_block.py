import pytest

ROSE = 1
FELL = 2
STILL = 0


@pytest.fixture(scope='module')
def block(harness):
  return harness('block')


def test_a_stride_is_due_at_its_first_call_then_every_sixteen(block):
  assert block['due'] == [0, 16, 32, 48]


def test_a_reset_makes_the_next_call_due(block):
  assert block['reset'] == [0, 5, 21, 37, 53]


def test_a_lane_is_read_in_major_channel_minor(block):
  assert block['lane'] == [5]


def test_an_edge_rises_and_falls_with_hysteresis(block):
  assert block['edge'] == [STILL, STILL, ROSE, STILL, FELL, STILL, ROSE]


def test_a_rise_is_placed_at_its_crossing(block):
  assert block['since'][0] == pytest.approx(0.8333, abs=1e-4)


def test_a_cv_scales_by_octaves_within_its_bounds(block):
  assert block['scaled'] == pytest.approx([200, 25, 20000, 20])
