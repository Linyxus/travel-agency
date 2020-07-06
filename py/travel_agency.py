from collections import namedtuple
from enum import Enum
import logging
import time

from _travel_agency import lib

Pos = namedtuple('Pos', ['lat', 'lnt'])
City = namedtuple('City', ['name', 'level', 'pos'])
Line = namedtuple('Line', ['src', 'dest', 'type', 'dep_time', 'duration'])


class LineType(Enum):
    Air = 0
    Subway = 1
    Highway = 2


def int_to_line_type(x):
    if x == 0:
        return LineType.Air
    elif x == 1:
        return LineType.Subway
    elif x == 2:
        return LineType.Highway


def get_city(cm, i):
    return City(
        name=lib.get_city_name(cm, i).decode('utf-8'),
        level=lib.get_city_level(cm, i),
        pos=Pos(lib.get_city_lat(cm, i), lib.get_city_lnt(cm, i))
    )


def get_line(cm, i):
    return Line(
        src=lib.get_line_from(cm, i),
        dest=lib.get_line_to(cm, i),
        type=int_to_line_type(lib.get_line_type(cm, i)),
        dep_time=lib.get_line_dep_time(cm, i),
        duration=lib.get_line_duration(cm, i)
    )


class CityMap:
    def __init__(self, path):
        super().__init__()
        self.handle = lib.read_city_map(path.encode('utf-8'))
        self.cities = [get_city(self.handle, i)
                       for i in range(lib.extract_city_num(self.handle))]
        self.lines = [get_line(self.handle, i)
                      for i in range(lib.extract_line_num(self.handle))]

    def free(self):
        lib.free_city_map(self.handle)


def vec_to_list(handle):
    xs = [lib.get_int_at(handle, i)
          for i in range(lib.get_vector_length(handle))]
    lib.deinit_int_vector(handle)

    return xs


def dvec_to_list(handle):
    xs = [int(lib.get_double_at(handle, i) * 10) /
          10 for i in range(lib.get_dvector_length(handle))]
    lib.deinit_double_vector(handle)

    return xs


class Journey:
    def __init__(self, handle):
        super().__init__()
        self.handle = handle
        self.src = lib.get_journey_src(self.handle)
        self.dest = lib.get_journey_dest(self.handle)
        self.length = lib.get_journey_length(self.handle)
        self.risk = lib.get_journey_risk(self.handle)
        self.route = vec_to_list(lib.get_journey_route(self.handle))
        self.step_risk = dvec_to_list(lib.get_journey_step_risk(self.handle))
        self.dep_time = lib.get_journey_dep_time(self.handle)

        lib.deinit_journey(self.handle)
        self.handle = None


class MinRiskJourneySolver:
    def __init__(self, city_map: CityMap):
        super().__init__()
        self.handle = lib.init_min_risk_spfa_solver(city_map.handle)

    def __call__(self, src, dep_time, dest, time_limit):
        prev = time.perf_counter()
        pres = lib.maybe_run_solver(self.handle, src, dep_time, dest, 0)
        t = time.perf_counter() - prev
        logging.info(
            f'run min risk spfa solver for {dep_time}:00 {src} --> {dest}, in {time_limit} hours, {t:.8f} secs used')
        return MaybeJourney(pres)

    def free(self):
        lib.deinit_solver(self.handle)
        self.handle = None


class MaybeJourney:
    def __init__(self, handle):
        self._just = lib.get_result_valid(handle)
        if self._just:
            self._val = Journey(lib.get_result_journey(handle))

        lib.deinit_result(handle)


class LimitedTimeMinRiskJourneySolver:
    def __init__(self, city_map: CityMap):
        super().__init__()
        self.handle = lib.init_limited_time_min_risk_solver(city_map.handle)

    def __call__(self, src, dep_time, dest, time_limit):
        prev = time.perf_counter()
        pres = lib.maybe_run_solver(
            self.handle, src, dep_time, dest, time_limit)
        t = time.perf_counter() - prev
        logging.info(
            f'run limited time min risk solver for {dep_time}:00 {src} --> {dest}, in {time_limit} hours, {t:.8f} secs used')

        return MaybeJourney(pres)

    def free(self):
        lib.deinit_solver(self.handle)
        self.handle = None


JourneyRecord = namedtuple('JourneyRecord', ['tid', 'journey', 'day', 'jid'])
