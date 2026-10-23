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
    ${CMAKE_CURRENT_SOURCE_DIR}/algos_relacy
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/external/relacy
    )


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
