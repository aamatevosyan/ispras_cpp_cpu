cmake_minimum_required(VERSION 3.17)
project(ispras_cpp_cpu)

set(CMAKE_CXX_STANDARD 14)

if (UNIX)
        set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")
endif()

add_compile_options(-Wall -Wextra -pedantic -Wfloat-equal -fno-stack-protector)

add_subdirectory(tests)
add_executable(asm src/lib/mstack.hpp src/main_asm.cpp src/lib/assembly.cpp src/lib/assembly.h src/lib/custom_exceptions.hpp src/lib/utils.hpp)
add_executable(dasm src/lib/mstack.hpp src/main_dasm.cpp src/lib/custom_exceptions.hpp src/lib/utils.hpp src/lib/disassembly.cpp src/lib/disassembly.h)
add_executable(cpu src/lib/mstack.hpp src/main_cpu.cpp src/lib/custom_exceptions.hpp src/lib/utils.hpp src/lib/cpu.cpp src/lib/cpu.h)