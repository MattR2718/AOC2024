#include <fmt/core.h>
#include <ctre.hpp>  // Include the ctre library

int main() {
    // Example using fmt
    fmt::print("Hello, {}!\n", "world");

    // Example using ctre for compile-time regex matching
    constexpr auto regex_pattern = ctre::regex<"\\d+">;  // Matches digits only

    // Test a few strings
    const char* test_str1 = "12345";
    const char* test_str2 = "abc123";

    if (ctre::match<regex_pattern>(test_str1)) {
        fmt::print("\"{}\" matches the regex pattern.\n", test_str1);
    }
    else {
        fmt::print("\"{}\" does NOT match the regex pattern.\n", test_str1);
    }

    if (ctre::match<regex_pattern>(test_str2)) {
        fmt::print("\"{}\" matches the regex pattern.\n", test_str2);
    }
    else {
        fmt::print("\"{}\" does NOT match the regex pattern.\n", test_str2);
    }

    return 0;
}
