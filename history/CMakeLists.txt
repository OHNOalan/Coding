cmake_minimum_required(VERSION 3.20)
project(CodeForce)

set(CMAKE_CXX_STANDARD 23)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined,thread -fno-omit-frame-pointer -g -O1")
endif()

add_executable(CodeForce
        c.cpp)

set(Benchmark_ROOT /opt/homebrew/Cellar/google-benchmark/1.9.0)
#message("Benchmark_ROOT = ${Benchmark_ROOT}")
include_directories(${Benchmark_ROOT}/include)
link_directories(${Benchmark_ROOT}/lib)
find_package(benchmark REQUIRED)
target_link_libraries(CodeForce benchmark::benchmark)

