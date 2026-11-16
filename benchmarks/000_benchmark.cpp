/*
Simple file to test if google benchmark is working.
*/

#include <benchmark/benchmark.h>
#include <string>
#include "000_hello.hpp"

static void benchmark_hello(benchmark::State& state) { // state is recieved from google benchmark
    for (auto _ : state) { // just means "iterate as many times as state (google benchmark) says to". Time is measured for everything in this loop.
        std::string message = get_hello();
        benchmark::DoNotOptimize(message);
    }
}

BENCHMARK(benchmark_hello); // register the function with google benchmark