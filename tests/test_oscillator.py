import pytest

TICKS = 3
HALF = 0.5
WIDTH = 0.5


@pytest.fixture(scope='module')
def oscillator(harness):
  return harness('oscillator')


def steps(oscillator):
  values = oscillator['step']
  return [values[at:at + TICKS] for at in range(0, len(values), TICKS)]


def test_a_step_at_the_coming_sample_start_reads_its_midpoint_first(oscillator):
  assert steps(oscillator)[0] == pytest.approx([HALF, 1, 1])


def test_a_step_at_the_middle_is_shared_by_the_two_samples_around_it(oscillator):
  assert steps(oscillator)[1] == pytest.approx([0.125, 0.875, 1])


def test_a_step_at_the_coming_sample_end_reads_its_midpoint_next(oscillator):
  assert steps(oscillator)[2] == pytest.approx([0, HALF, 1])


def test_a_blip_keeps_its_area_and_leaves_nothing_behind(oscillator):
  assert oscillator['sum'] == pytest.approx([WIDTH])
  assert oscillator['blip'][-1] == pytest.approx(0)


def test_a_sync_with_no_edge_ticks_as_the_free_oscillator(oscillator):
  assert oscillator['plain'] == pytest.approx([0, 0, 0], abs=1e-6)


def test_the_master_driven_sync_is_the_edge_driven_one_at_its_crossing(oscillator):
  assert oscillator['driven'] == pytest.approx([0, 0, 0], abs=1e-6)


def test_an_edge_resets_the_slave_at_its_sub_sample_place(oscillator):
  assert oscillator['placed'] == pytest.approx([1, 1, 1], rel=1e-3)

