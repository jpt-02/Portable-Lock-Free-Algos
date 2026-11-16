/*
Benchmarks performance of 00_mutex.hpp, which uses a mutex to establish a baseline of comparison
*/

#include <benchmark/benchmark.h>
#include <00_mutex.hpp>

static void bm_read(benchmark::State& state) { // state is recieved from google benchmark
    
    RegularMutex<int> regular_mutex;
    regular_mutex.write(42); // give it a random int

    for (auto _ : state) { // just means "iterate as many times as state (google benchmark) says to". Time is measured for everything in this loop.
        int result = regular_mutex.read();
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(bm_read); // register the function with google benchmark
