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
    // 线路信息数据结构
    struct Line
    {
        LineId id; // 线路编号，是唯一标识符
        CityId from, to; // 出发城市、目的城市
        LineType type; // 线路类型
        DepTime dep_time; // 出发时间，0 <= dep_time < 24
        Duration duration; // 时长
        bool operator==(const Line &other) const
        {
            return other.id == id && other.from == from && other.to == to && other.type == type && other.dep_time == dep_time && other.duration == duration;
        }
    };
    typedef LinkedList<Line> CityLine;
    typedef std::vector<CityLine> LineInfo;

    // 城市信息数据结构
    struct City
    {
        CityId id; // 城市编号，是唯一标识符
        std::string name; // 城市名称
        double lat, lnt; // 城市坐标，经度纬度
        int level; // 城市风险等级，0 - 低风险，1 - 中风险，2 - 高风险
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

        size_t num_cities() const; // 城市个数
        const CityInfo &cities() const; // 城市信息
        const std::vector<Line> &lines() const; // 线路信息
        const CityLine &lines_of(CityId city) const; // 获得从某一城市出发的线路链表

    private:
        CityInfo _city_info;
        LineInfo _line_info;
        std::vector<Line> _lines;
    };

    namespace io
    {
        CityMap load_city_map(const std::string &path); // 加载城市线路信息
        CityInfo read_city_info(std::ifstream &fs); // 读取城市信息
        std::pair<std::vector<Line>, LineInfo> read_line_info(std::ifstream &fs, size_t num_cities); // 读取线路信息
    } // namespace io
    
} // namespace tagc
