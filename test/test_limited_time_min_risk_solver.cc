#include "Solver.hh"
#include "gtest/gtest.h"

namespace tagc {

   TEST(LimitedTimeMinRiskSolverTest, Simple) {
        auto cm = io::load_city_map("../../test/assets/city_map_simple.txt");
        // Make sure that the file is properly loaded
        ASSERT_EQ(cm.num_cities(), 4);
        ASSERT_EQ(cm.lines().size(), 5);

        Solver *p = new LimitedTimeRiskSolver(cm);
        auto j = p->solve(0, 8, 3, 4);
        ASSERT_TRUE(j.second);
        ASSERT_EQ(j.first.to_line_id_list(), std::vector<int>({0, 3}));
   }

}