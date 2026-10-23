#include <cstdint>
#include <relacy/relacy.hpp>
#include "01_relacy.hpp"

// Will create 2 "threads":
// One for consumer, one for producer
static constexpr size_t kThreads = 2;

// Each buffer is just a single integer.
// Testing for:
// - Order of updates is the same for producer and consumer.
// - Last written value by the producer is never lost
// - Other conditions that relacy checks automatically:
//   data races, deadlocks, livelocks.
struct Test01 : rl::test_suite<Test01, kThreads> {
    static constexpr size_t kLastNumber = 25;
    TripleBuffer_Relacy<rl::var<int>> trio{};

    void thread(unsigned thread_index) {
        if (thread_index == 0) { // Producer
            // Writing 25 integers
            for (int i = 1; i <= kLastNumber; ++i) {
                auto rlval = trio.Write()($);
                rlval.store(i);
                trio.Commit();
            }
        } else { // Consumer
            // Read until we get 25. If 25 is never read
            // relacy will report livelock.
            int prev_read = 0;
            while (prev_read != kLastNumber) {
                auto& rlval = trio.Read();
                int val = rlval($);
                // Check order: new value must be >= than the previous
                // It will be > when it's new value, and will
                // be == to previous when we read stale data. 
                RL_ASSERT(val >= prev_read);
                prev_read = val;
            }
        }
    }
};

int main() {
    rl::simulate<Test01>();
    return 0;
}