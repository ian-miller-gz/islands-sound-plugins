import pytest

READS = 3
DEEPEST = 0.003
CLOSE = 1e-6
LOUDEST = 2.5
APART = 0.99


@pytest.fixture(scope='module')
def line(harness):
  return harness('line')


def test_a_dry_chorus_passes_its_input_exactly(line):
  assert line['dry'] == [0]


def test_the_mono_sum_is_the_mean_of_the_three_reads(line):
  assert line['mono'][0] <= CLOSE


def test_the_two_sides_differ(line):
  assert line['correlation'][0] < APART


def test_no_output_passes_the_sum_of_its_gains(line):
  assert line['peak'][0] <= LOUDEST


def test_a_deep_settle_is_held_within_the_line(line):
  assert line['deep'] == pytest.approx([DEEPEST] * READS, rel=CLOSE)
  far, length, reach = line['reach']
  assert far <= length <= reach
