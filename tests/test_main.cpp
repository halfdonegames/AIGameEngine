#include "test_framework.hpp"

#include <iostream>

int main() {
    int failures = 0;
    for (const auto& test : gaia::test::Cases()) {
        try { test.Function(); std::cout << "PASS " << test.Name << '\n'; }
        catch (const std::exception& exception) { ++failures; std::cerr << "FAIL " << test.Name << ": " << exception.what() << '\n'; }
    }
    return failures == 0 ? 0 : 1;
}
