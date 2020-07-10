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
    typedef int CityId; // 城市编号的类型
    typedef int LineId; // 线路编号的类型
    typedef int Duration; // 时间长度
    typedef int DepTime; // 出发时间
    typedef unsigned long size_t;

    enum LineType { // 线路类型
        Air, // 飞机
        Subway, // 火车
        Highway // 客车
    };

    // 内联函数，获取城市风险等级对应的风险系数
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

    // 内联函数，获取线路类型对应的风险系数
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

    // 将线路类型转换为整数
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
