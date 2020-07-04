#pragma once

#include <cassert>

#define AIR_RISK_FACTOR 9
#define SUBWAY_RISK_FACTOR 5
#define HIGHWAY_RISK_FACTOR 2
#define LEVEL_0_RPH 0.2
#define LEVEL_1_RPH 0.5
#define LEVEL_2_RPH 0.9

#define HOURS_PER_DAY 24

namespace tagc {
    typedef int CityId;
    typedef int LineId;
    typedef int Duration;
    typedef int DepTime;
    typedef unsigned long size_t;

    enum LineType {
        Air,
        Subway,
        Highway
    };

    inline double risk_of_level(int level) {
        assert(0 <= level && level <= 2);

        switch (level) {
            case 0:
                return LEVEL_0_RPH;
            case 1:
                return LEVEL_1_RPH;
            case 2:
                return LEVEL_2_RPH;
        }
        return -1;
    }

    inline double risk_of_line(LineType type) {
        switch (type) {
            case Air:
                return AIR_RISK_FACTOR;
            case Subway:
                return SUBWAY_RISK_FACTOR;
            case Highway:
                return HIGHWAY_RISK_FACTOR;
        }
    }

    inline int line_type_to_int(LineType type) {
        switch (type) {
            case Air:
                return 0;
            case Subway:
                return 1;
            case Highway:
                return 2;
        }
    }
} // namespace tagc
