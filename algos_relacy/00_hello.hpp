/*
Simple file to make sure CMakeLists configures the tests correctly.

Only relevant if youre taking this entire repo and testing with it, 
otherwise just ignore this file.
*/

#pragma once
#include <string>

inline std::string get_hello() {
    return "hello";
}