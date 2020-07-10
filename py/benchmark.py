import random
import math
import tempfile
import os
import psutil
from time import perf_counter
from argparse import ArgumentParser
import numpy as np

from travel_agency import *

def show_bytes(size, decimal_places=3):
    for unit in ['B','KiB','MiB','GiB','TiB']:
        if size < 1024.0:
            break
        size /= 1024.0
    return f"{size:.{decimal_places}f}{unit}"

def get_mem_usage():
    process = psutil.Process()
    return process.memory_info().rss

def gen_random_city(cid):
    lat = random.random() * 30 + 90
    lnt = random.random() * 20 + 20
    level = random.randint(0, 2)
    return [cid, level, lat, lnt]

def gen_random_cities(n):
    return [gen_random_city(i) for i in range(n)]

def dist(c1, c2):
    return math.sqrt((c1[0] - c2[0]) ** 2 + (c1[1] - c2[1]) ** 2)

def air_len(p1, p2):
    return dist(p1, p2) / 5 + 1

def subway_len(p1, p2):
    return dist(p1, p2) / 5 * 2 + 1

def highway_len(p1, p2):
    return dist(p1, p2) / 2 * 3 + 1

line_len = {
    'air': air_len,
    'subway': subway_len,
    'highway': highway_len
}

def gen_random_line(cities):
    n = len(cities)
    i1, i2 = random.sample(range(n), k=2)
    p1, p2 = cities[i1][2:], cities[i2][2:]
    line_type = random.choice(['air', 'subway', 'highway'])
    length = line_len[line_type](p1, p2)
    return [i1, i2, line_type, random.randint(0, 23), length]

def gen_random_lines(cities, n):
    return [gen_random_line(cities) for _ in range(n)]

def gen_random_map(n, d):
    cities = gen_random_cities(n)
    lines = gen_random_lines(cities, n * d)
    fd, path = tempfile.mkstemp()
    with os.fdopen(fd, 'w') as f:
        f.write(f'{len(cities)}\n')
        f.write('\n'.join([f'{x[0]} {x[1]} {x[2]} {x[3]}' for x in cities]))
        f.write(f'\n{len(lines)}\n')
        f.write('\n'.join([f'{x[0]} {x[1]} {x[2]} {x[3]} {x[4]}' for x in lines]))
    return path

def gen_random_req(n, strategy):
    src, dest = random.sample(range(n), k=2)
    dep_time = random.randint(0, 23)
    time_limit = 0 if strategy == 'min_risk' else random.randint(1, 100)
    return src, dep_time, dest, time_limit, strategy

def do_random_test_case(n, mr_solver, ltmr_solver, strategy):
    src, dep_time, dest, time_limit, strategy = gen_random_req(n, strategy)
    solver = mr_solver if strategy == 'min_risk' else ltmr_solver
    t = perf_counter()
    _ = solver(src, dep_time, dest, time_limit)
    t = perf_counter() - t
    mem = get_mem_usage()
    print(f'finished test case for {strategy}, time {t} sec, mem {show_bytes(mem)}')
    return t, mem

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('-n', '--city', type=int, default=10000)
    parser.add_argument('-d', '--degree', type=int, default=10)
    parser.add_argument('-r','--repeat', type=int, default=100)
    args = parser.parse_args()

    print(f'generating city map ...')
    path = gen_random_map(args.city, args.degree)
    cm = CityMap(path)
    print(f'city map generated and loaded, mem {show_bytes(get_mem_usage())}')
    mr_solver = MinRiskJourneySolver(cm)
    ltmr_solver = LimitedTimeMinRiskJourneySolver(cm)
    print(f'solvers initialized, mem {show_bytes(get_mem_usage())}')
    
    n = len(cm.cities)
    print('=== Min Risk Test Begin ===')
    mr_stats = [do_random_test_case(n, mr_solver, ltmr_solver, 'min_risk') for _ in range(args.repeat)]
    print('=== Min Risk Test End ===')

    print('=== Limited Time Min Risk Test Begin ===')
    ltmr_stats = [do_random_test_case(n, mr_solver, ltmr_solver, 'limited_min_risk') for _ in range(args.repeat)]
    print('=== Limited Min Risk Test End ===')

    mr_stats = np.array(mr_stats)
    ltmr_stats = np.array(ltmr_stats)
    stats = np.concatenate([mr_stats, ltmr_stats], 0)
    print(f'min risk: time avg {mr_stats.mean(0)[0]} max {mr_stats.max(0)[0]} min {mr_stats.min(0)[0]}')
    print(f'min risk: mem avg {show_bytes(mr_stats.mean(0)[1])} max {show_bytes(mr_stats.max(0)[1])} min {show_bytes(mr_stats.min(0)[1])}')

    print(f'limited time min risk: time avg {ltmr_stats.mean(0)[0]} max {ltmr_stats.max(0)[0]} min {ltmr_stats.min(0)[0]}')
    print(f'limited time min risk: mem avg {show_bytes(ltmr_stats.mean(0)[1])} max {show_bytes(ltmr_stats.max(0)[1])} min {show_bytes(ltmr_stats.min(0)[1])}')

    print(f'all: time avg {stats.mean(0)[0]} max {stats.max(0)[0]} min {stats.min(0)[0]}')
    print(f'all: mem avg {show_bytes(stats.mean(0)[1])} max {show_bytes(stats.max(0)[1])} min {show_bytes(stats.min(0)[1])}')

    mr_solver.free()
    ltmr_solver.free()
    cm.free()