#pragma once

#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <utility>
#include "LinkedList.hh"
#include "Const.hh"

namespace tagc
{
    struct Line
    {
        LineId id;
        CityId from, to;
        LineType type;
        DepTime dep_time;
        Duration duration;
        bool operator==(const Line &other) const
        {
            return other.id == id && other.from == from && other.to == to && other.type == type && other.dep_time == dep_time && other.duration == duration;
        }
    };
    typedef LinkedList<Line> CityLine;
    typedef std::vector<CityLine> LineInfo;

    struct City
    {
        CityId id;
        std::string name;
        double lat, lnt;
        int level;
        bool operator==(const City &other) const
        {
            return other.id == id && other.name == name && other.lat == lat && other.lnt == lnt && other.level == level;
        }
    };
    typedef std::vector<City> CityInfo;

    class CityMap
    {
    public:
        CityMap(CityInfo city_info, std::vector<Line> lines, LineInfo line_info)
            : _city_info(std::move(city_info)), _lines(std::move(lines)), _line_info(std::move(line_info)) {}

        size_t num_cities() const;
        const CityInfo &cities() const;
        const std::vector<Line> &lines() const;
        const CityLine &lines_of(CityId city) const;

    private:
        CityInfo _city_info;
        LineInfo _line_info;
        std::vector<Line> _lines;
    };

    namespace io
    {
        CityMap load_city_map(const std::string &path);
        CityInfo read_city_info(std::ifstream &fs);
        std::pair<std::vector<Line>, LineInfo> read_line_info(std::ifstream &fs, size_t num_cities);
    } // namespace io
} // namespace tagc
