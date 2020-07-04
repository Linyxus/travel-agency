#include "Solver.h"
#include <utility>
#include <queue>

namespace tagc {
    Solver::Solver(const CityMap &city_map) : _city_map(city_map) {
    }

    Journey Solver::solve(CityId src, DepTime dep_time, CityId dest) {
        assert(src != dest); // sanity check
        _reset();
        return _solve(src, dep_time, dest);
    }

    void MinRiskSPFASolver::_reset() {
        // initialize _cache
        for (auto &x : _cache) {
            x = std::vector<std::shared_ptr<Journey>>{ HOURS_PER_DAY, std::shared_ptr<Journey>{ nullptr } };
        }

        // initialize _in_queue
        for (auto &x : _in_queue) {
            x = std::vector<bool>(HOURS_PER_DAY, false);
        }
    }

    typedef std::pair<CityId, DepTime> Node;
    Journey MinRiskSPFASolver::_solve(CityId src, DepTime dep_time, CityId dest) {
        std::queue<Node> q;
        _cache[src][dep_time] = std::make_shared<Journey>(src, dep_time, &_city_map);
        _in_queue[src][dep_time] = true;

        Node u{src, dep_time};
        q.push(u);

        // SPFA
        while (!q.empty()) {
            u = q.front(); q.pop();
            auto cid = u.first;
            auto now = u.second;
            _in_queue[cid][now] = false;

            for (auto it = _city_map.lines_of(cid).cbegin(); it != _city_map.lines_of(cid).cend(); it++) {
                auto line = *it;
                auto j = std::make_shared<Journey>(line.id, _cache[cid][now]);
                Node v { j->dest(), j->arrive_time() };
                if (!_cache[v.first][v.second] || j->risk() < _cache[v.first][v.second]->risk()) {
                    _cache[v.first][v.second] = j;
                    if (!_in_queue[v.first][v.second]) {
                        q.push(v);
                        _in_queue[v.first][v.second] = true;
                    }
                }
            }
        }

        // Find out the final answer
        Journey ret(0, 0, &_city_map);
        bool found = false;
        for (const auto &pj : _cache[dest]) {
            if (pj) {
                if (!found) {
                    ret = *pj;
                    found = true;
                } else {
                    if (pj->risk() < ret.risk()) {
                        ret = *pj;
                    }
                }
            }
        }

        assert(found); // MUST BE FOUND since city graph should be connected
        return ret;
    }

    MinRiskSPFASolver::MinRiskSPFASolver(const CityMap &city_map)
    : Solver(city_map), _cache(city_map.num_cities()), _in_queue(city_map.num_cities()) {
    }
}
