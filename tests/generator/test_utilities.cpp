// Garamon unit tests: src/Utilities.cpp

#include <algorithm>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "Utilities.hpp"
#include "CliffordReference.hpp"

using garamon_test::CliffordReference;

TEST_CASE("factorial", "[utilities]") {
    CHECK(factorial(0) == 1u);
    CHECK(factorial(1) == 1u);
    CHECK(factorial(5) == 120u);
    CHECK(factorial(12) == 479001600u);
}

TEST_CASE("binomial coefficients", "[utilities]") {
    CHECK(bin_coeff(5, 0) == 1u);
    CHECK(bin_coeff(5, 2) == 10u);
    CHECK(bin_coeff(5, 5) == 1u);
    CHECK(bin_coeff(5, 6) == 0u);
    CHECK(bin_coeff(0, 0) == 1u);
    CHECK(bin_coeff(20, 10) == 184756u);
    CHECK(bin_coeff(25, 12) == 5200300u);
    CHECK(bin_coeff(31, 15) == 300540195u); // intermediate products overflow 32 bits
    CHECK(bin_coeff(32, 16) == 601080390u);

    // Pascal's triangle
    for(unsigned int n = 1; n <= 31; ++n)
        for(unsigned int k = 1; k <= n; ++k) {
            INFO("C(" << n << "," << k << ")");
            CHECK(bin_coeff(n, k) == bin_coeff(n - 1, k - 1) + bin_coeff(n - 1, k));
            CHECK(bin_coeff(n, k) == bin_coeff(n, n - k));
        }
}

TEST_CASE("per grade starting index", "[utilities]") {
    std::vector<int> c3ga = {0};
    computePerGradeStartingIndex(5, c3ga, 0, 0);
    CHECK(c3ga == std::vector<int>{0, 1, 6, 16, 26, 31});

    std::vector<int> e3ga = {0};
    computePerGradeStartingIndex(3, e3ga, 0, 0);
    CHECK(e3ga == std::vector<int>{0, 1, 4, 7});

    std::vector<int> e1ga = {0};
    computePerGradeStartingIndex(1, e1ga, 0, 0);
    CHECK(e1ga == std::vector<int>{0, 1});

    std::vector<int> e0ga = {0}; // must terminate
    computePerGradeStartingIndex(0, e0ga, 0, 0);
    CHECK(e0ga == std::vector<int>{0, 1});
}

TEST_CASE("hamming weight and outer product sign", "[utilities]") {
    CHECK(hammingWeight(0) == 0u);
    CHECK(hammingWeight(0b1011) == 3u);
    CHECK(hammingWeight(0xFFFFFFFFu) == 32u);

    for(unsigned int a = 0; a < 64; ++a)
        for(unsigned int b = 0; b < 64; ++b) {
            if(a & b)
                continue;
            INFO("blades " << a << " ^ " << b);
            CHECK(computeSign(a, b) == (int)CliffordReference::outerSign(a, b));

            int sign = 0;
            CHECK(outerProductBinaryComponents(a, b, sign) == (a ^ b));
            CHECK(sign == computeSign(a, b));

            unsigned int mvC = 0;
            double coefficient = 1.0;
            getSignAndBladeOuterProduct(mvC, a, b, coefficient, 2.0, 3.0);
            CHECK(mvC == (a | b));
            CHECK(coefficient == 1.0 + 6.0 * computeSign(a, b));
        }

    // blades with a common basis vector: the outer product is null
    unsigned int mvC = 42;
    double coefficient = 1.0;
    getSignAndBladeOuterProduct(mvC, 0b011, 0b110, coefficient, 2.0, 3.0);
    CHECK(coefficient == 1.0);
    CHECK(mvC == 42u);
}

TEST_CASE("combinations", "[utilities]") {
    const auto combinations = generateCombinations(4, 2);
    const std::vector<std::vector<unsigned int>> expected = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};
    CHECK(combinations == expected);

    for(unsigned int n = 1; n <= 8; ++n)
        for(unsigned int r = 0; r <= n; ++r) {
            const auto sequence = generateCombinations(n, r);
            INFO("n=" << n << ", r=" << r);
            CHECK(sequence.size() == bin_coeff(n, r));
            CHECK(std::is_sorted(sequence.begin(), sequence.end()));
            for(const auto &combination : sequence) {
                CHECK(combination.size() == r);
                CHECK(std::is_sorted(combination.begin(), combination.end()));
            }
        }

    const auto xorIndexToPosition = getSetOfCombinationsFromXorIndexation(3, generateCombinations(3, 2));
    REQUIRE(xorIndexToPosition.size() == 8u);
    CHECK(xorIndexToPosition[0b011] == 0u);
    CHECK(xorIndexToPosition[0b101] == 1u);
    CHECK(xorIndexToPosition[0b110] == 2u);
}

TEST_CASE("double to string conversion keeps the full precision", "[utilities]") {
    CHECK(doubleToString(2.0) == "2");
    CHECK(doubleToString(-1.0) == "-1");
    CHECK(doubleToString(0.5) == "0.5");
    CHECK(doubleToString(0.0) == "0");
    for(double value : {1.0 / 3.0, -2.0 / 7.0, 1.0e-10, 123456.789, 0.70710678118654757}) {
        INFO(value);
        CHECK(std::stod(doubleToString(value)) == value);
    }
}
