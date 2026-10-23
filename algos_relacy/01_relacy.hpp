/*
Relacy instrumentation of the triple buffer. Uses uintptr_t, for dirty bit logic, which may not be available on certain microcontrollers.
*/

#include <relacy/relacy.hpp> // takes place of #include <atomic>
#include <cstdint> // Provides uintptr_t when implementation supports it. Not necessary on my machine because atomic includes everything transitively, but may be necessary for other machines.
#include <cstddef> // Provides size_t. Not necessary on my machine because atomic includes everything transitively, but may be necessary for other machines.

template<typename T>
class TripleBuffer_Relacy {
public:
    TripleBuffer_Relacy() = default;

    T& Read() {
        /*
        Returns the reference (not the pointer) of an object of type T.
        */
        uintptr_t possibly_dirty_ptr = middle_buffer_ptr_int_($).load(rl::mo_relaxed); //uintptr_t represents a pointer as an integer so we can manipulate its bits
        if ((possibly_dirty_ptr & kDirtyBit) == 1) { // check if the lowest bit is set to 1 by using the AND operator. kDirtyBit is all zeroes except for 1 in the rightmost place
			uintptr_t prev = middle_buffer_ptr_int_($).exchange(reinterpret_cast<uintptr_t>(front_buffer_ptr_), rl::mo_acq_rel); // turn front into an integer, swap it with middle
			front_buffer_ptr_ = reinterpret_cast<T*>(prev & kDirtyBitMask); // strip the dirty bit from new front using AND operator, turn it back into a regular pointer for object T
        }

        return *front_buffer_ptr_; // dereference pointer using * and return, so it returns a reference to object T that the front buffer points to
    }

    T& Write() {
        /*
        Returns the reference (not the pointer) of an object of type T.
        */
        return *back_buffer_ptr_;
    }

    void Commit() {
        /*
        Commits any changes to the back buffer into the middle buffer
        */
        uintptr_t dirty_ptr = kDirtyBit | reinterpret_cast<uintptr_t>(back_buffer_ptr_); // turns back buffer into integer, uses OR operator to set rightmost bit to 1
        uintptr_t prev = middle_buffer_ptr_int_($).exchange(dirty_ptr, rl::mo_acq_rel); // swap back and middle buffer
        back_buffer_ptr_ = reinterpret_cast<T*>(prev & kDirtyBitMask); // strip dirty bit from new back using AND operator, turn it back into a regular pointer for object T
    }
private:
    static constexpr size_t kNoSharing = 64; // used to align to 64-byte boundary (typical consumer CPU), can be changed for other CPU architectures
    static constexpr uintptr_t kDirtyBit = 1; // integer representing a 'memory address' that is all zeroes except for 1 in rightmost place. Used with OR operator to set dirty bit
    static constexpr uintptr_t kDirtyBitMask = ~uintptr_t() ^ kDirtyBit; // same as above, but all 1s except for a 0 in rightmost place. Used with AND operator to get rid of dirty bit

    struct alignas(kNoSharing) Buffer {
        T data{0}; // whatever data for type T initialized with default values. Changed to 0 for relacy test.
    };

    Buffer buffers_[3]; // array of 3 buffers

    rl::atomic<uintptr_t> middle_buffer_ptr_int_{reinterpret_cast<uintptr_t>(&buffers_[1].data)}; // get the address of middle buffer data, turn it into integer

    T* back_buffer_ptr_{&buffers_[0].data}; // pointer to back buffer data, NOT in int form

    T* front_buffer_ptr_{&buffers_[2].data}; // pointer to front buffer data, NOT in int form


    // Dont think these actually need alignment - commented out for now TODO

    // alignas(kNoSharing) T* back_buffer_ptr_{&buffers_[0].data}; // aligned pointer to back buffer data, NOT in int form

    // alignas(kNoSharing) T* front_buffer_ptr_{&buffers_[2].data}; // aligned pointer to front buffer data, NOT in int form
};