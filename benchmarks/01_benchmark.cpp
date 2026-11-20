/*
Benchmarks performance of 01_triplebuffer.hpp
*/

#include <benchmark/benchmark.h>
#include <01_triplebuffer.hpp>

// CLEAN READ

static void bm_read(benchmark::State& state) { // state is recieved from google benchmark
    
    TripleBuffer<int> triple;
    triple.get_write() = 42; // give it a random int
    triple.commit_write();
    int initial_result = triple.read(); // clear dirty state
    benchmark::DoNotOptimize(initial_result); // attempt to ensure compiler doesn't optimize away our clearing of the dirty state.
                                              // if it does anyway, not a huge deal because its a single dirty read among over a 
                                              // billion clean ones from the loop.

    for (auto _ : state) { // just means "iterate as many times as state (google benchmark) says to". Time is measured for everything in this loop.
        int& result = triple.read();
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(bm_read); // register the function with google benchmark

// DIRTY READ

