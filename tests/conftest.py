import concurrent.futures
import os
import pathlib
import re
import subprocess

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]
SNAPSHOT = ROOT.parent
HARNESSES = ROOT / 'tests' / 'harness'
ENGINE = pathlib.Path('~/Repositories/mine/islands-private').expanduser()
COMPILE = ['g++', '-std=c++23', '-O1']
SIBLING = re.compile(r'^#include\s*"\.\./(\w+)/\1\.hpp"', re.M)
ITEM = re.compile(r'^\s*-\s*(\S+)')
TOP = re.compile(r'^[a-z]')
ENTRY = 'namespace SOUND::PLUGINS::HARNESS::{0} {{ void run(); }}\nauto main() -> int {{ SOUND::PLUGINS::HARNESS::{0}::run(); }}\n'


def engine():
  stated = os.environ.get('ISLANDS_ENGINE')
  return pathlib.Path(stated) if stated else ENGINE


def roots(home):
  source = home / 'src'
  found = sorted({str(path.parent) for path in source.rglob('*.hpp')})
  flags = [flag for folder in found + [str(source), str(source / 'core')] for flag in ('-isystem', folder)]
  flags += ['-I', str(home / 'build' / 'include')]
  return flags + [flag for entry in includes(home) for flag in ('-I', str(home / entry))]


def includes(home):
  entries, inside = [], False
  for line in (home / 'configs' / 'make.yaml').read_text().splitlines():
    if line.startswith('includes:'):
      inside = True
    elif inside and TOP.match(line):
      break
    elif inside and ITEM.match(line):
      entries.append(ITEM.match(line).group(1))
  return entries


def closure(name, extra):
  seen, due = set(), [name, *extra]
  while due:
    family = due.pop()
    if family in seen:
      continue
    seen.add(family)
    for path in sorted((ROOT / 'core' / family).iterdir()):
      due.extend(SIBLING.findall(path.read_text()))
  return sorted(seen)


def stale(target, source):
  if not target.exists() or target.stat().st_mtime < source.stat().st_mtime:
    return True
  marks = target.with_suffix('.d')
  if not marks.exists():
    return True
  for header in marks.read_text().replace('\\\n', ' ').split(':', 1)[-1].split():
    seen = pathlib.Path(header)
    if not seen.exists() or target.stat().st_mtime < seen.stat().st_mtime:
      return True
  return False


def compiled(flags, unit, target):
  if stale(target, unit):
    target.parent.mkdir(parents=True, exist_ok=True)
    command = [*COMPILE, *flags, '-c', str(unit), '-MMD', '-MF', str(target.with_suffix('.d')), '-o', str(target)]
    done = subprocess.run(command, capture_output=True, text=True)
    assert done.returncode == 0, done.stderr
  return target


def parsed(output):
  words = {}
  for line in output.splitlines():
    word, *numbers = line.split()
    words.setdefault(word, []).extend(float(number) for number in numbers)
  return words


@pytest.fixture(scope='session')
def harness(request):
  home = engine()
  for needed in (home / 'src' / 'cartridge' / 'plugin.hpp', SNAPSHOT / 'plugin.hpp'):
    if not needed.is_file():
      pytest.skip(f'no {needed}: the harness needs the engine and the sound snapshot')
  cache = pathlib.Path(request.config.cache.mkdir('core'))
  flags = roots(home)

  def run(name, *extra):
    units = [unit for family in closure(name, extra) for unit in sorted((ROOT / 'core' / family).glob('*.cpp'))]
    targets = [cache / unit.parent.name / f'{unit.name}.o' for unit in units]
    with concurrent.futures.ThreadPoolExecutor() as pool:
      objects = list(pool.map(lambda pair: compiled(flags, *pair), zip(units, targets)))
    binary = cache / 'harness' / name
    binary.parent.mkdir(parents=True, exist_ok=True)
    sources = [HARNESSES / f'{name}.cpp', *sorted(HARNESSES.glob(f'{name}.*.cpp'))]
    command = [*COMPILE, *flags, '-x', 'c++', '-', '-x', 'none', *map(str, sources), *map(str, objects), '-o', str(binary)]
    done = subprocess.run(command, input=ENTRY.format(name.upper()), capture_output=True, text=True)
    assert done.returncode == 0, done.stderr
    ran = subprocess.run([str(binary)], capture_output=True, text=True, check=True)
    return parsed(ran.stdout)

  return run
