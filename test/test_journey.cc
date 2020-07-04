#include "Journey.hh"
#include "CityMap.hh"
#include "gtest/gtest.h"

namespace tagc {

    TEST(JourneyTest, NullPtrIsFalse) {
        std::shared_ptr<Journey> p(nullptr);

        ASSERT_TRUE(!p);
    }

    TEST(JourneyTest, ToVectorWorks) {
        auto cm = io::load_city_map("../../test/assets/city_map_2.txt");

        auto j1 = std::make_shared<Journey>(0, 10, &cm);
        EXPECT_EQ(j1->to_line_id_list().size(), 0);

        auto j2 = std::make_shared<Journey>(0, j1);
        ASSERT_EQ(j2->to_line_id_list().size(), 1);
        EXPECT_EQ(j2->to_line_id_list()[0], 0);

        auto j3 = std::make_shared<Journey>(1, j2);
        auto j4 = std::make_shared<Journey>(2, j3);
        auto j5 = std::make_shared<Journey>(2, j3);
        ASSERT_EQ(j4->to_line_id_list().size(), 3);
        EXPECT_EQ(j4->to_line_id_list(), (std::vector<int>{0, 1, 2}));
        ASSERT_EQ(j5->to_line_id_list().size(), 3);
        EXPECT_EQ(j5->to_line_id_list(), (std::vector<int>{0, 1, 2}));
    }

    TEST(JourneyTest, LengthWorks) {
        auto cm = io::load_city_map("../../test/assets/city_map_2.txt");

        auto j1 = std::make_shared<Journey>(0, 10, &cm);
        auto j2 = std::make_shared<Journey>(0, j1);

        EXPECT_EQ(j1->length(), 0);
        EXPECT_EQ(j2->length(), 23);
        EXPECT_EQ(j2->arrive_time(), 9);

        auto j3 = std::make_shared<Journey>(3, j2);
        EXPECT_EQ(j3->arrive_time(), 22);
        EXPECT_EQ(j3->length(), 36);
    }

    TEST(JourneyTest, RiskWorks) {
        auto cm = io::load_city_map("../../test/assets/city_map_2.txt");

        auto j1 = std::make_shared<Journey>(0, 10, &cm);
        auto j2 = std::make_shared<Journey>(0, j1);

        EXPECT_EQ(j1->risk(), 0);
        EXPECT_DOUBLE_EQ(j2->risk(), 27.9);

        auto j3 = std::make_shared<Journey>(3, j2);
        EXPECT_DOUBLE_EQ(j3->risk(), 42.4);
    }
}
