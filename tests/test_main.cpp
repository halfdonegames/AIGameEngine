#include "test_framework.hpp"
int main() { for (const auto& test : gaia::test::Cases()) { test.run(); std::cout << "PASS " << test.name << '\n'; } return 0; }
