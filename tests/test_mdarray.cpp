#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <complex>
#include <numeric>   // std::iota
#include <algorithm> // std::equal

#include "../src/mdv.hpp"
namespace stdex = std::experimental;

TEST_CASE("Test the layout-specialized get_coeff function", "[get_coeff]")
{
    auto g = mdv::iota<double, 3>(0, 2, 3, 4);
    g.print();
    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 4; ++k)
            {
                // std::cout << g.get(i, j, k) << std::endl;
                REQUIRE(g.get(i, j, k) == g.get_mdspan()(i, j, k));
            }
        }
    }
}

TEST_CASE("Test the layout-specialized get_layout_left_coeff function", "[get_layout_left_coeff]")
{
    auto g = mdv::iota<double, 3, 'L'>(0, 2, 3, 4);
    // g.print();
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 2; ++k)
            {
                // std::cout << g.get(i, j, k) << std::endl;
                REQUIRE(g.get(k, i, j) == g.get_mdspan()(k, i, j));
            }
        }
    }
}

TEST_CASE("Test the get_colmajor_copy function", "[get_colmajor_copy]")
{
    auto g = mdv::randn<double, 2>(2, 3);
    auto h = g.get_colmajor_copy();
    REQUIRE(!std::equal(g.get_vector().begin(),
                        g.get_vector().begin(),
                        h.get_vector().begin()));

    for (size_t i = 0; i < g.get_shape()[0]; ++i)
    {
        for (size_t j = 0; j < g.get_shape()[1]; ++j)
        {
            REQUIRE(g.get_mdspan()(i, j) == h.get_mdspan()(i, j));
        }
    }
}

TEST_CASE("Test the get_rowmajor_copy function", "[get_rowmajor_copy]")
{
    auto g = mdv::randn<double, 2, 'L'>(2, 3);
    auto h = g.get_rowmajor_copy();
    REQUIRE(!std::equal(g.get_vector().begin(),
                        g.get_vector().begin(),
                        h.get_vector().begin()));

    for (size_t i = 0; i < g.get_shape()[0]; ++i)
    {
        for (size_t j = 0; j < g.get_shape()[1]; ++j)
        {
            REQUIRE(g.get_mdspan()(i, j) == h.get_mdspan()(i, j));
        }
    }
}

TEST_CASE("Test the dot function", "[dot]")
{
    auto f = mdv::iota<double, 2, 'R'>(0, 4, 3);
    auto g = mdv::iota<double, 2, 'L'>(0, 3, 2);
    auto h = f.dot(g);
    auto g_mod = g.get_rowmajor_copy();
    // f.print();
    // g_mod.print();
    // h.print();
    std::vector<double> v_np{5, 14, 14, 50, 23, 86, 32, 122};
    mdv::MDV<double, 2, 'R'> h_np(v_np, 4, 2);
    for (size_t i = 0; i < h.get_shape()[0]; ++i)
    {
        for (size_t j = 0; j < h.get_shape()[1]; ++j)
        {
            REQUIRE(h.get_mdspan()(i, j) == h_np.get_mdspan()(i, j));
        }
    }
}

TEST_CASE("Test the subtract function", "[subtract]")
{
    auto f = mdv::iota<double, 2>(0., 4, 3);
    auto g = mdv::fill<double, 2>(100., 4, 3);
    auto h = f.subtract(g);
    // f.print();
    // g.print();
    // h.print();
    std::vector<double> v_np{
        -100, -99, -98,
        -97, -96, -95,
        -94, -93, -92,
        -91, -90, -89};
    mdv::MDV<double, 2, 'R'> h_np(v_np, 4, 3);
    for (size_t i = 0; i < h.get_shape()[0]; ++i)
    {
        for (size_t j = 0; j < h.get_shape()[1]; ++j)
        {
            REQUIRE(h.get_mdspan()(i, j) == h_np.get_mdspan()(i, j));
        }
    }
}

TEST_CASE("Test the add function", "[add]")
{
    auto f = mdv::iota<double, 2>(0., 4, 3);
    auto g = mdv::fill<double, 2>(-100., 4, 3);
    auto h = f.add(g);
    // f.print();
    // g.print();
    // h.print();
    std::vector<double> v_np{
        -100, -99, -98,
        -97, -96, -95,
        -94, -93, -92,
        -91, -90, -89};
    mdv::MDV<double, 2, 'R'> h_np(v_np, 4, 3);
    for (size_t i = 0; i < h.get_shape()[0]; ++i)
    {
        for (size_t j = 0; j < h.get_shape()[1]; ++j)
        {
            REQUIRE(h.get_mdspan()(i, j) == h_np.get_mdspan()(i, j));
        }
    }
}

TEST_CASE("VectorMd", "[vectormd]")
{
    size_t depths = 2;
    size_t rows = 3;
    size_t cols = 5;

    std::vector<double> v(depths * rows * cols);
    std::iota(v.begin(), v.end(), 0);
    mdv::MDV<double, 3> b(v, depths, rows, cols);
    auto c = mdv::zeros<double, 2>(rows, cols);
    auto d = mdv::ones<double, 2>(rows, cols);
    auto e = mdv::randn<double, 3>(2, 5, 10);
    auto f = mdv::fill<std::complex<double>, 2>(std::complex<double>(0, 0), rows, cols);

    auto b_mds = b.get_mdspan();

    // REQUIRE(b.get_mdspan()(0, 0, 1) == b.get(0, 0, 1)); // ok
    // REQUIRE(b.get_mdspan()(1, 0, 0) == b.get(1, 0, 0)); // ok
    // REQUIRE(b.get_mdspan()(0, 1, 0) == b.get(0, 1, 0)); // ok
    // REQUIRE(b.get_mdspan()(1, 1, 1) == b.get(1, 1, 1)); // not ok

    auto v1 = b.get_vector();
    for (auto e : v1)
    {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    auto sp = b.get_span();
    for (auto e : sp)
    {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    auto last_cols = stdex::submdspan(b_mds, std::pair{0, depths}, std::pair{0, rows}, std::pair{cols - 1, cols});
    for (int d = 0; d < last_cols.extent(0); d++)
        for (int r = 0; r < last_cols.extent(1); r++)
            for (int c = 0; c < last_cols.extent(2); c++)
                last_cols(d, r, c) = -999;

    b.print();
    c.print();
    d.print();
    e.print();
    f.print();
}
