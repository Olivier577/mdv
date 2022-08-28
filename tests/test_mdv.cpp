#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <array>
#include <complex>
#include <numeric>   // std::iota
#include <algorithm> // std::equal

#include "../src/mdv.hpp"

TEST_CASE("Test constructors", "[MDV(...)]")
{
    MDV<double, 3, 0>();
    MDV<std::complex<double>, 4, 0>();
    MDV<std::complex<double>, 3, 0>(std::array<size_t, 3>{2, 3, 4});
    std::array<size_t, 3> shape{2, 3, 4};
    MDV<std::complex<double>, 3, 0>(std::move(shape));
    MDV<double, 3, 0>(std::array<size_t, 3>{2, 3, 4},
                      std::vector<double>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                          13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23});
    std::vector<double> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                          13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    MDV<double, 3, 0> a1(std::array<size_t, 3>{2, 3, 4}, std::move(v));
    MDV<double, 3, 0> a2(a1);
}

TEST_CASE("Test resize", "[resize]")
{
    MDV<double, 3, 0> a1 (std::array<size_t, 3>{2, 3, 4},
                      std::vector<double>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                          13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23});
    a1.resize(std::array<size_t, 3>{1, 3, 4});
}

TEST_CASE("Test get_id", "[get_id]")
{
    MDV<double, 2, 0> a1 (std::array<size_t, 2>{2, 3},
                      std::vector<double>{0, 1, 2, 3, 4, 5});

    MDV<double, 2, 1> a2 (std::array<size_t, 2>{2, 3},
                      std::vector<double>{0, 1, 2, 3, 4, 5});

    std::cout << a1.get_id(std::array<size_t, 2>{1, 1}) << std::endl;

    std::cout << a2.get_id(std::array<size_t, 2>{1, 1}) << std::endl;
}

TEST_CASE("Test get & set", "[get & set]")
{
    MDV<double, 2, 0> a1 (std::array<size_t, 2>{2, 3},
                      std::vector<double>{0, 1, 2, 3, 4, 5});

    a1.set(std::array<size_t, 2>{1, 2}, -9);
    std::cout << a1.get(std::array<size_t, 2>{1, 2}) << std::endl;
}

