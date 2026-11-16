/*
Test for 01_relacy.hpp, which is the instrumented version of 01_triplebuffer.hpp. 
Stores single integers in each buffer. Tests to ensure that:
- Both producer and consumer see updates in the correct order
- The final value written by the producer isn't lost
- Default Relacy Tests (races, deadlocks, livelocks)
Does NOT ensure that updates aren't skipped, as we don't care about this for a triple buffer.
*/

// Imports

#include <cstdint>
#include <relacy/relacy.hpp>
#include "01_relacy.hpp"

// Code

static constexpr size_t num_threads = 2; // Number of fake threads that relacy creates. If 2, it does 1 producer and 1 consumer.
                                         // If this is changed to a larger number, this file will test for SPMC with no necessary further adjustments.

struct Test01 : rl::test_suite<Test01, num_threads> { // struct build on top of Relacy's test suite. Done like this because it makes all methods public by default, same as writinng class --> public
    static constexpr size_t final_testval = 25; // define the final test value
    TripleBuffer_Relacy<rl::var<int>> triple{}; // define triple buffer that we are testing

    void thread(unsigned thread_index) {
        
        // PRODUCER

        if (thread_index == 0) { // Producer, writes integers starting at 1 and ending at 25
            for (int i = 1; i <= final_testval; ++i) {
                auto rlval = triple.get_write()($); // get back buffer, use $ so relacy can track it. Use auto to keep it as a relacy data type so we can use
                                                    // rlval.store(i) in the next line. If we declared it as an int (like in consumer section) this would be impossible.
                rlval.store(i);
                triple.commit_write();
            }
        
        // CONSUMER

        } else { // Consumer, reads until it sees 25. If 25 is never seen, indicates a livelock which Relacy will catch.
            int prev_read = 0;
            while (prev_read != final_testval) {
                int val = triple.read()($); // .read() returns rl::var<int>, ($) instruments read, result is copied into val as an integer.
                RL_ASSERT(val >= prev_read); // assert that this val is greater than or equal to the previous one
                prev_read = val; // set prev_val for next iteration
            }
        }
    }
};

int main() {
    rl::simulate<Test01>();
    return 0;
}