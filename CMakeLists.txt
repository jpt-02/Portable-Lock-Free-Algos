cmake_minimum_required(VERSION 3.18)
project(LockFreeAlgos CXX)

# Disable treating pointer-to-integer truncation errors as fatal for GCC/MinGW
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wno-error=pointer-to-int-cast -Wno-error=int-to-pointer-cast -fpermissive)
endif()

set(CMAKE_CXX_STANDARD 17) # Minimum C++ version
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Requires minimum version for compilation

# Create header-only library target
add_library(algos_lib INTERFACE)
target_include_directories(algos_lib INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/algos
    ${CMAKE_CURRENT_SOURCE_DIR}/algos_relacy
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/external/relacy # Relacy is header-only so this is sufficient to make it work
    )


# SETUP TESTING

enable_testing()
# Find all test source files in src/cpp/tests/. CONFIGURE_DEPENDS means this updates every time the directory is changed.
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")

foreach(TEST_SRC ${TEST_SOURCES})
    # Extract filename without extension (e.g. test)
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    # Register executable target
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE algos_lib)
    # Register target with CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()


# SETUP GOOGLE BENCHMARK

# Build Google Benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(
    benchmarks/external/google-benchmark
    ${CMAKE_BINARY_DIR}/google-benchmark
)

file(GLOB BENCHMARK_SOURCES CONFIGURE_DEPENDS "benchmarks/*.cpp")

foreach(BENCHMARK_SRC ${BENCHMARK_SOURCES})
    get_filename_component(BENCHMARK_NAME ${BENCHMARK_SRC} NAME_WE)
    add_executable(${BENCHMARK_NAME} ${BENCHMARK_SRC})
    target_link_libraries(${BENCHMARK_NAME} PRIVATE algos_lib benchmark::benchmark_main)
    
    # generate intermediate assembly files (GCC only) - this is to inspect the assembly of your benchmarks 
    # to ensure nothing important is optimized away. Can be commented out.
    # generated file is in build/CMakeFiles/benchmarkname.dir/benchmarks, looks like benchmarkname.cpp.s
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${BENCHMARK_NAME} PRIVATE -save-temps=obj)
    endif()

endforeach()