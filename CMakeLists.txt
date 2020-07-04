cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(TravelAgency)

add_library(travel_agency
        include/CityMap.hh
        include/LinkedList.hh
        include/Journey.hh
        include/TravelAgency.hh
        include/travel_agency.h
        include/Const.hh
        src/CityMap.cc
        src/Journey.cc
        include/Solver.h
        src/Solver.cc src/c_wrapper.cc)

target_include_directories(
        travel_agency
        PUBLIC
        "include"
)

#! <<< BEGIN CONFIGURE GoogleTest >>>

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

#! <<< END CONFIGURE GoogleTest >>>

add_executable(test_main
        test/main.cc
        test/test_linked_list.cc
        test/test_city_map.cc
        test/test_journey.cc
        test/test_min_risk_spfa_solver.cc)

target_link_libraries(test_main gtest_main)
target_link_libraries(test_main travel_agency)
add_test(NAME test_main COMMAND test_main)
