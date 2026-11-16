/*
Simple file to make sure CMakeLists configures the tests correctly.
*/

#include <iostream>
#include <string>
#include "000_hello.hpp"

int main() {
    std::string result = get_hello();
    std::cout << "Test Received: " << result << "\n";

    if (result == "hello") {
        return 0;
    } else {
        std::cerr << "Assertion failed: expected 'hello', got '" << result << "'\n";
        return 1;
    }

}