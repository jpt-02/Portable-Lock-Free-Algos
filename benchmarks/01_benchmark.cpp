/*
Benchmarks performance of 01_triplebuffer.hpp
*/

#include <benchmark/benchmark.h>
#include <01_triplebuffer.hpp>

static void bm_read(benchmark::State& state) { // state is recieved from google benchmark
    
    TripleBuffer<int> triple;
    triple.get_write() = 42; // give it a random int
    triple.commit_write();

    for (auto _ : state) { // just means "iterate as many times as state (google benchmark) says to". Time is measured for everything in this loop.
        auto* result = &triple.read();
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(bm_read); // register the function with google benchmark
