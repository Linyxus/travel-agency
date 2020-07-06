#include "Journey.hh"
#include "Const.hh"
#include <utility>

using namespace std;

tagc::Duration time_between(tagc::DepTime a, tagc::DepTime b)
{
    return (b - a + 24) % 24;
}

namespace tagc {

    Journey::Journey(CityId cid, DepTime dep_time, const CityMap *city_map)
    {
        _city_map = city_map;
        _src = cid;
        _dest = cid;
        _line_id = -1;
        _dep_time = dep_time;
        _arrive_time = dep_time;
        _risk = 0;
        _step_risk = 0;
        _length = 0;
        _next = shared_ptr<const Journey>(nullptr);
    }

    Journey::Journey(LineId lid, shared_ptr<const Journey> rem)
    {
        const CityMap &cm = *(rem->_city_map);
        _city_map = rem->_city_map;
        const Line &line = cm.lines()[lid];
        assert(line.from == rem->_dest);  // SANITY CHECK
        const City &city = cm.cities()[line.from];

        _src = rem->_src;
        _dest = line.to;
        _line_id = lid;
        _dep_time = rem->_dep_time;
        _arrive_time = (line.dep_time + line.duration) % 24;

        _risk = rem->_risk;
        _risk += time_between(rem->_arrive_time, line.dep_time) * risk_of_level(city.level);
        _risk += risk_of_line(line.type) * line.duration * risk_of_level(city.level);
        _step_risk = _risk - rem->_risk;

        _length = rem->_length + line.duration;
        _length += time_between(rem->_arrive_time, line.dep_time);

        _next = rem;
    }

    std::vector<LineId> Journey::to_line_id_list() const {
        if (!_next) {
            return std::vector<LineId>{};
        }

        auto ret = _next->to_line_id_list();
        ret.push_back(_line_id);

        return ret;
    }

    std::vector<double> Journey::to_step_risk_list() const {
        if (!_next) {
            return std::vector<double>{};
        }

        auto ret = _next->to_step_risk_list();
        ret.push_back(_step_risk);

        return ret;
    }

} // namespace tagc
