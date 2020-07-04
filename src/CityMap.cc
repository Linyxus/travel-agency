#include "CityMap.hh"

const tagc::CityInfo &tagc::CityMap::cities() const {
    return _city_info;
}

const std::vector<tagc::Line> &tagc::CityMap::lines() const {
    return _lines;
}

size_t tagc::CityMap::num_cities() const {
    return this->_city_info.size();
}

const tagc::CityLine &tagc::CityMap::lines_of(CityId city) const {
    assert(city < this->num_cities()); // city id SHOULD never go out of range
    return this->_line_info[city];
}

tagc::CityMap tagc::io::load_city_map(const std::string &path) {
    std::ifstream fs;
    fs.open(path, std::ios::in);

    CityInfo city_info = read_city_info(fs);
    size_t num_cities = city_info.size();
    auto res = read_line_info(fs, num_cities);
    auto lines = res.first;
    auto line_info = res.second;

    return CityMap{ city_info, lines, line_info };
}

tagc::CityInfo tagc::io::read_city_info(std::ifstream &fs) {
    CityInfo ret;

    int n; fs >> n;
    for (int i = 0; i < n; i++) {
        std::string name;
        int level;
        double lat, lnt;
        fs >> name >> level >> lat >> lnt;
        ret.push_back(City{ i, name, lat, lnt, level });
    }

    return ret;
}

std::pair<std::vector<tagc::Line>, tagc::LineInfo> tagc::io::read_line_info(std::ifstream &fs, size_t num_cities) {
    std::vector<Line> lines;
    LineInfo ret(num_cities, CityLine{});

    int n; fs >> n;
    for (int i = 0; i < n; i++) {
        CityId from, to;
        std::string typeName;
        int duration;
        int dep_time;
        fs >> from >> to >> typeName >> dep_time >> duration;
        LineType type;
        if (typeName == "air") {
            type = Air;
        } else if (typeName == "subway") {
            type = Subway;
        } else if (typeName == "highway") {
            type = Highway;
        }
        assert(from < num_cities && from >= 0);
        assert(to < num_cities && to >= 0);
        ret[from].push(Line{ i, from, to, type, static_cast<DepTime>(dep_time), static_cast<Duration>(duration) });
        lines.push_back(Line{ i, from, to, type, static_cast<DepTime>(dep_time), static_cast<Duration>(duration) });
    }

    return { lines, ret };
}
