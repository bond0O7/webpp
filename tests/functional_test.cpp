// Created by moisrex on 12/10/19.
#include <array>
#include <gtest/gtest.h>
#include <webpp/utils/functional.h>

using namespace webpp;

void test(int limit) {
    static auto i = 0;
    i++;
    EXPECT_TRUE(i < limit - 1) << "i is: " << i << "; limit: " << limit;
}

TEST(FunctionalTests, DebouncedFunctions) {
    using namespace std::chrono;
    constexpr auto checking_deduction_for_function_pointers = debounce(test);
    constexpr auto debounced_test =
        debounce<decltype(test), debounce_type::leading, milliseconds(10)>(
            test);
    constexpr auto limit = 1000;
    for (int i = 0; i < limit; i++)
        debounced_test(limit);
}