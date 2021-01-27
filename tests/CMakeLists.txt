include_directories(../src)

include_directories(.)

add_executable(tests
        mtest.cpp
        ../src/lib/mstack.hpp
# gtest sources
    gtest/gtest-all.cc
    gtest/gtest_main.cc
        ../src/lib/custom_exceptions.hpp ../src/lib/utils.hpp ../src/lib/disassembly.cpp ../src/lib/disassembly.h)

# add pthread for unix systems
if (UNIX)
    target_link_libraries(tests pthread)
endif ()

# for coverage
 TARGET_COMPILE_OPTIONS(tests PUBLIC --coverage)
 target_link_libraries(tests gcov)
