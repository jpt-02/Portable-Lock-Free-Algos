/*
Implementation of a mutex for read & write. Used as a baseline of comparison.
*/

#pragma once

#include <mutex>

template<typename T>
class RegularMutex {
    public:
        RegularMutex() = default;

        T read() const { // const means nothing in here is modified
            std::lock_guard<std::mutex> lock(mutex_); // acquire mutex lock
            return data_;
        } // lock is automatically relinquished here

        void write(const T& value) { // T& prevents performance loss from immediately making an unnecessary copy
            std::lock_guard<std::mutex> lock(mutex_); // acquire mutex lock
            data_ = value;
        } // lock is automatically relinquished here

    private:
        mutable std::mutex mutex_; // mutex declaration, mutable so its internal state can be changed even in a const function like read()
        T data_{}; // data with data type T, default initialization
};