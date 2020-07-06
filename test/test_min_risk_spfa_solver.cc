#include "Solver.hh"
#include "gtest/gtest.h"

namespace tagc {

    TEST(MinRiskSPFASolverTest, Simple) {
        auto cm = io::load_city_map("../../test/assets/city_map_simple.txt");
        // Make sure that the file is properly loaded
        ASSERT_EQ(cm.num_cities(), 4);
        ASSERT_EQ(cm.lines().size(), 5);

        MinRiskSPFASolver solver(cm);
        auto j = solver.solve(0, 8, 3);
        ASSERT_TRUE(j.second);
        ASSERT_EQ(j.first.to_line_id_list(), std::vector<int>({0, 2, 4}));

        Solver *p = &solver;
        j = p->solve(0, 8, 3);
        ASSERT_TRUE(j.second);
        ASSERT_EQ(j.first.to_line_id_list(), std::vector<int>({0, 2, 4}));
    }

}
