import pytest

TOLERANCE = 1e-5
CLOSE = 1e-4
STRIDE = 2049
SPAN = 4097
FRAMES = 3
LEVELS = 9


@pytest.fixture(scope='module')
def oscillator(harness):
  return harness('oscillator')


def test_a_single_level_table_reads_as_before(oscillator):
  assert oscillator['single'] == pytest.approx([1, STRIDE, 1], abs=TOLERANCE)


def test_a_levelled_table_at_most_doubles(oscillator):
  assert oscillator['levels'] == [LEVELS, FRAMES * SPAN]


def test_the_level_keeps_the_top_partial_under_half_a_turn(oscillator):
  assert oscillator['level'] == [0, 1, 3, 8, 8]


def test_the_transform_draws_the_sum_of_sines(oscillator):
  assert oscillator['error'][0] < CLOSE


def test_a_level_holds_its_partials_and_no_more(oscillator):
  whole, kept, cut = oscillator['content']
  assert whole > CLOSE
  assert kept == pytest.approx(whole, rel=CLOSE)
  assert abs(cut) < TOLERANCE


def test_frames_between_are_blended_on_every_level(oscillator):
  assert oscillator['blend'] == pytest.approx([0.5, 0.75, 0], abs=TOLERANCE)
