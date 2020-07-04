#pragma once

#include <memory>
#include <vector>
#include "Const.hh"
#include "CityMap.hh"
#include "LinkedList.hh"

namespace tagc
{
    class Journey;

    class Journey
    {
    public:
        Journey(CityId cid, DepTime dep_time, const CityMap *city_map);
        Journey(LineId lid, std::shared_ptr<const Journey> rem);

        CityId src() const { return _src; }
        CityId dest() const { return _dest; }
        DepTime dep_time() const { return _dep_time; }
        DepTime arrive_time() const { return _arrive_time; }
        double risk() const { return _risk; }
        int length() const { return _length; }

        std::vector<LineId> to_line_id_list() const;

      private:
        const CityMap *_city_map;
        CityId _src;
        CityId _dest;
        LineId _line_id;
        DepTime _dep_time;
        DepTime _arrive_time;
        double _risk;
        Duration _length;
        std::shared_ptr<const Journey> _next;
    };
} // namespace tagc
