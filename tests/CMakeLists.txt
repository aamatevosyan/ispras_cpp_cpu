include_directories(../src)

include_directories(.)

add_executable(tests
        mtest.cpp
        ../src/lib/mstack.hpp
# gtest sources
    gtest/gtest-all.cc
    gtest/gtest_main.cc
)

# add pthread for unix systems
if (UNIX)
    target_link_libraries(tests pthread)
endif ()

# for coverage
 TARGET_COMPILE_OPTIONS(tests PUBLIC --coverage)
 target_link_libraries(tests gcov)
