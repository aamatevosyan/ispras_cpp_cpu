cmake_minimum_required(VERSION 3.17)
project(ispras_cpp_cpu)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wfloat-equal -fno-stack-protector)

add_subdirectory(tests)
add_executable(ispras_cpp_cpu src/main.cpp src/lib/mstack.hpp)