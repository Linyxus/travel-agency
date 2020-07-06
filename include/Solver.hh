#pragma once
#include "Const.hh"
#include "Journey.hh"
#include "CityMap.hh"
#include <vector>

namespace tagc {
    class Solver;
    class MinRiskSPFASolver;
    typedef std::pair<Journey, bool> Result;

    class Solver {
    public:
        explicit Solver(const CityMap &city_map);
        virtual ~Solver() = default;
        Result solve(CityId src, DepTime dep_time, CityId dest, Duration time_limit = 0);
    protected:
        virtual void _reset() = 0;
        virtual Result _solve(CityId src, DepTime dep_time, CityId dest, Duration time_limit) = 0;
        const CityMap &_city_map;
    };

    class MinRiskSPFASolver : public Solver {
    public:
        explicit MinRiskSPFASolver(const CityMap &city_map);
        ~MinRiskSPFASolver() override = default;
    protected:
        void _reset() override;
        Result _solve(CityId src, DepTime dep_time, CityId dest, Duration time_limit) override;
    private:
        std::vector<std::vector<std::shared_ptr<Journey>>> _cache;
        std::vector<std::vector<bool>> _in_queue;
    };

    typedef std::pair<Journey, bool> Result;
    class LimitedTimeRiskSolver : public Solver {
    public:
        explicit LimitedTimeRiskSolver(const CityMap &city_map);
        ~LimitedTimeRiskSolver() override = default;

    protected:
        void _reinit(Duration time_limit);
        void _reset() override;
        Result _solve(CityId src, DepTime dep_time, CityId dest, Duration time_limit) override;

    private:
        std::vector<std::vector<std::shared_ptr<Journey>>> _memo;
        std::vector<std::vector<bool>> _in_queue;
    };

}
