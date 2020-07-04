#include "CityMap.hh"
#include "gtest/gtest.h"

namespace tagc {

TEST(CityMapTest, LoadCityMapWorks) {
    CityMap cm = io::load_city_map("../../test/assets/city_map_1.txt");
    ASSERT_EQ(cm.num_cities(), 3);

    CityInfo gt{
        City{0, "Beijing", 10, 20, 2},
        City{1, "Shanghai", 20, 10, 1},
        City{2, "Wuxi", 5, 0, 0},
    };

    for (int i = 0; i < gt.size(); i++) {
        ASSERT_EQ(cm.cities()[i], gt[i]);
    }

    for (auto city : cm.cities()) {
        ASSERT_EQ(cm.lines_of(city.id).length(), 0);
    }

    cm = io::load_city_map("../../test/assets/city_map_2.txt");
    ASSERT_EQ(cm.num_cities(), 2);
    EXPECT_EQ(cm.lines_of(0).length(), 2);
    EXPECT_EQ(cm.lines_of(1).length(), 2);

    auto iter = cm.lines_of(0).cbegin();
    Line line = Line{2, 0, 1, Highway, 7, 3};
    ASSERT_EQ(*iter, line);
    iter++;
    line = Line{0, 0, 1, Air, 8, 1};
    ASSERT_EQ(*iter, line);

    std::vector<Line> gt2 {
        Line{ 0, 0, 1, Air, 8, 1 },
        Line{ 1, 1, 0, Subway, 10, 2 },
        Line{ 2, 0, 1, Highway, 7, 3 },
        Line{ 3, 1, 0, Air, 20, 2 }
    };

    for (int i = 0; i < gt2.size(); i++) {
        ASSERT_EQ(cm.lines()[i], gt2[i]);
    }
}

}