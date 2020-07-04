#pragma once
#include "Const.hh"
#include "Journey.hh"
#include "CityMap.hh"
#include <vector>

namespace tagc {
    class Solver;
    class MinRiskSPFASolver;

    class Solver {
    public:
        explicit Solver(const CityMap &city_map);
        virtual ~Solver() = default;
        Journey solve(CityId src, DepTime dep_time, CityId dest);
    protected:
        virtual void _reset() = 0;
        virtual Journey _solve(CityId src, DepTime dep_time, CityId dest) = 0;
        const CityMap &_city_map;
    };

    class MinRiskSPFASolver : public Solver {
    public:
        explicit MinRiskSPFASolver(const CityMap &city_map);
        ~MinRiskSPFASolver() override = default;
    protected:
        void _reset() override;
        Journey _solve(CityId src, DepTime dep_time, CityId dest) override;
    private:
        std::vector<std::vector<std::shared_ptr<Journey>>> _cache;
        std::vector<std::vector<bool>> _in_queue;
    };

}
