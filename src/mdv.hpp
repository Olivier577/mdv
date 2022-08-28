#pragma once

#include <ctgmath>
#include <array>
#include <vector>
#include <numeric>    // std::accumulate()
#include <algorithm>  // std::equal()
#include <functional> // std::multiplies()
#include <cassert>
#include <iostream>

template <typename T, size_t D, size_t L = 1>
class MDV
{

    std::array<size_t, D> m_shape;
    std::vector<T> m_data;
    size_t m_size;
    T m_tol = 1e-9;

public:
    // Constructors
    MDV();
    MDV(std::array<size_t, D> &&shape);
    MDV(std::array<size_t, D> &&shape, std::vector<T> &&data);

    MDV(const MDV<T, D, L> &input);

    // Destructor
    ~MDV();

    // Configuration methods.
    void resize(std::array<size_t, D> &&shape);
    void reshape(std::array<size_t, D> &&shape);

    // Element access methods
    T get(std::array<size_t, D> &&coords);
    void set(std::array<size_t, D> &&coords, T val);

    /*

    // Member access methods
    std::array<T, D> get_shape() const;
    std::array<T, D> get_size() const;
    std::array<T, D> get_data() const;

    // Template parameter access methods
    size_t get_dim() const;
    size_t get_layout() const;

    // Overload == operator
    bool compare(const MDV<T, D, L> &other, T tol);
    bool operator==(const MDV<T, D, L> &rhs, T tol = m_tol);

    // Overload + - * / operators
    template <class U>
    friend MDV<U> operator+(const MDV<U> &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator+(const U &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator+(const MDV<U> &lhs, const U &rhs);
    template <class U>
    friend MDV<U> operator-(const MDV<U> &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator-(const U &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator-(const MDV<U> &lhs, const U &rhs);
    template <class U>
    friend MDV<U> operator*(const MDV<U> &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator*(const U &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator*(const MDV<U> &lhs, const U &rhs);
    template <class U>
    friend MDV<U> operator/(const MDV<U> &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator/(const U &lhs, const MDV<U> &rhs);
    template <class U>
    friend MDV<U> operator/(const MDV<U> &lhs, const U &rhs);

*/

public:
    // Element access methods
    size_t get_coeff(size_t i);
    size_t get_coeff1(size_t i);
    size_t get_id(std::array<size_t, D> coords);
    std::array<size_t, D> get_coords(const size_t &index);
    auto traverse_indices();
    bool close_enough(const T &a, const T &b, T m_tol);
};

////////////////////CONSTRUCTOR / DESTRUCTOR FUNCTIONS///////////////////////////////////////////////

// The default constructor.
template <class T, size_t D, size_t L>
MDV<T, D, L>::MDV()
{
    static_assert(L == 0 || L == 1);
    m_shape = {};
    m_size = 0;
    m_data = {};
}

// Construct empty matrix (all elements 0)
template <class T, size_t D, size_t L>
MDV<T, D, L>::MDV(std::array<size_t, D> &&shape)
    : m_shape(shape)
{
    static_assert(L == 0 || L == 1);
    m_size = std::accumulate(m_shape.begin(), m_shape.end(),
                             1, std::multiplies<size_t>());
    m_data = std::vector<T>(m_size, 0);
}

// Construct with std::vector.
template <class T, size_t D, size_t L>
MDV<T, D, L>::MDV(std::array<size_t, D> &&shape, std::vector<T> &&data)
    : m_shape(shape), m_data(data), m_size(data.size())
{
    static_assert(L == 0 || L == 1);
}

// The copy constructor.
template <class T, size_t D, size_t L>
MDV<T, D, L>::MDV(const MDV<T, D, L> &input)
    : m_shape(input.m_shape), m_data(input.m_data), m_size(input.m_size) {}

// The destructor.
template <class T, size_t D, size_t L>
MDV<T, D, L>::~MDV() {}

// Configuration methods.
template <class T, size_t D, size_t L>
void MDV<T, D, L>::resize(std::array<size_t, D> &&shape)
{
    m_shape = shape;
    m_size = std::accumulate(m_shape.begin(), m_shape.end(),
                             1, std::multiplies<size_t>());
    m_data.resize(m_size);
}

// Element functions

// Get value at given coordinates
template <class T, size_t D, size_t L>
T MDV<T, D, L>::get(std::array<size_t, D> &&coords)
{
    return m_data[get_id(coords)];
}

// Set value at given coordinates
template <class T, size_t D, size_t L>
void MDV<T, D, L>::set(std::array<size_t, D> &&coords, T val)
{
    size_t ndx = get_id(coords);
    m_data[get_id(coords)] = val;
}

/*

// Return the shape of the MDV object
template <class T, size_t D, size_t L>
std::array<T, D> MDV<T, D, L>::get_shape() const
{
    return m_shape;
}

// Return the vector size of the MDV object
template <class T, size_t D, size_t L>
std::array<T, D> MDV<T, D, L>::get_size() const
{
    return m_size;
}

// Return the vector of the MDV object
template <class T, size_t D, size_t L>
std::array<T, D> MDV<T, D, L>::get_data() const
{
    return m_data;
}

// Return the dimension
template <class T, size_t D, size_t L>
size_t MDV<T, D, L>::get_dim() const
{
    return D;
}

// Return the layout number
template <class T, size_t D, size_t L>
size_t MDV<T, D, L>::get_layout() const
{
    return L;
}

// Compare with another MDV object
template <class T, size_t D, size_t L>
bool MDV<T, D, L>::compare(const MDV<T, D, L> &other, T tol = m_tol)
{
    if (D != other.get_dim())
    {
        return false;
    }
    if (m_size != other.get_size())
    {
        return false;
    }
    if (!std::equal(m_shape.begin(), m_shape.end(), other.get_shape().begin()))
    {
        return false;
    }
    bool same_elements;
    if (L == other.get_layout())
    {
        same_elements = std::equal(m_data.begin(), m_data.end(), other.get_data(),
                                   [](const T &a, const T &b) { return close_enough(a, b); })
    }
    else
    {
        same_elements = true;

        // Replace this shit by traverse_right!
        for (size_t i = 0; i < m_size; ++i)
        {
            auto coords = MDV<T, D, L>::get_coords(i);
            same_elements *= close_enough(get(coords), other.get(coords));
        }
    }
    return same_elements;
}

// The == operator
template <class T, size_t D, size_t L>
bool MDV<T, D, L>::operator==(const MDV<T, D, L> &rhs, T tol = m_tol)
{
    return MDV<T, D, L>::compare(rhs, tol);
}


*/

///////////PRIVATE FUNCTIONS/////////////////////////////////////////////////////////////////////////

// Get coefficients to map coordinates to index (Layout=0)
template <class T, size_t D, size_t L>
size_t MDV<T, D, L>::get_coeff(size_t i)
{
    size_t p = 1;
    while (i < D - 1)
    {
        p *= m_shape[++i];
    }
    return p;
}

// Get coefficients to map coordinates to index (Layout=1)
template <class T, size_t D, size_t L>
size_t MDV<T, D, L>::get_coeff1(size_t i)
{
    size_t p = 1;
    while (i > 0)
    {
        p *= m_shape[--i];
    }
    return p;
}

// Map coordinates to vector index
template <class T, size_t D, size_t L>
size_t MDV<T, D, L>::get_id(std::array<size_t, D> coords)
{
    // no better solution at the time
    auto c = [&](const size_t &a)
    {
        return (L == 0) ? get_coeff(a) : get_coeff1(a);
    };

    size_t id = 0;
    for (size_t i = 0; i < D; ++i)
    {
        id += coords[i] * c(i);
    }
    return id;
}

/*


// Map index to coordinates in function of shape
template <class T, size_t D, size_t L>
std::array<size_t, D> MDV<T, D, L>::get_coords(const size_t &index)
{
    size_t n = D - 1;
    size_t product = m_size;
    size_t incr = 1;
    std::array<size_t, D> coords();

    while (incr != 0)
    {
        if (index - (product - 1) <= 0)
        {
            if (n == D - 1)
            {
                coords[n] = index;
                incr = 0;
            }
            else
            {
                product /= shape[n];
                coords[n] = index / product;
                size_t rest = index %= product;
                if (rest == 0)
                {
                    break;
                }
                else
                {
                    index = rest;
                    incr = 1;
                }
            }
        }
        else
        {
            product *= shape[n - 1];
            incr = -1;
        }
        n += incr;
    }
    return coords;
}

// template <class T, size_t D, size_t L>
// auto MDV<T, D, L>::traverse_indices(size_t loop_level)
#include <array>
#include <vector>
#include <iostream>
#include <numeric>
template <size_t D>
void traverse_left(std::array<size_t, D> &shape, std::array<size_t, D> &coords)
{
    for (size_t i = 0; i < D; i++)
    {
        if (coords[i] < shape[i] - 1)
        {
            coords[i]++;
            std::fill(coords.begin(), coords.end() - (D - i), 0);
            for (auto e : coords)
            {
                std::cout << e << " ";
            }
            std::cout << std::endl;
            traverse_left(shape, coords);
        }
    }
}

template <size_t D>
void traverse_right(std::array<size_t, D> &shape, std::array<size_t, D> &coords)
{
    for (long i = D - 1; i > -1; i--)
    {
        if (coords[i] < shape[i] - 1)
        {
            coords[i]++;
            std::fill(coords.begin() + i + 1, coords.end(), 0);
            for (auto e : coords)
            {
                std::cout << e << " ";
            }
            std::cout << std::endl;
            traverse_right(shape, coords);
        }
    }
}

template <size_t D>
void traverse_right2(std::array<size_t, D> &shape, std::array<size_t, D> &coords)
{
    bool flag = true;
    while (flag)
    {
        for (long i = D - 1; i > -1; i--)
        {
            if (coords[i] < shape[i] - 1)
            {
                coords[i]++;
                std::fill(coords.begin() + i + 1, coords.end(), 0);
                for (auto e : coords)
                {
                    std::cout << e << " ";
                }
                std::cout << std::endl;
                flag = true;
                break;
            }
            flag = false;
        }
    }
}

template <size_t D>
void traverse_left2(std::array<size_t, D> &shape, std::array<size_t, D> &coords)
{
    bool flag = true;
    while (flag)
    {
        for (size_t i = 0; i < D; i++)
        {
            if (coords[i] < shape[i] - 1)
            {
                coords[i]++;
                std::fill(coords.begin(), coords.end() - (D - i), 0);
                for (auto e : coords)
                {
                    std::cout << e << " ";
                }
                std::cout << std::endl;
                flag = true;
                break;
            }
            flag = false;
        }
    }
}

std::array<size_t, 9> shape{3, 4, 5, 6, 7, 8, 9, 10, 11};
std::array<size_t, 9> coords1{0, 0, 0, 0, 0, 0, 0, 0, 0};
traverse_right2<9>(shape, coords1);

std::array<size_t, 9> coords{0, 0, 0, 0, 0, 0, 0, 0, 0};
traverse_left2<9>(shape, coords);

template <class T, size_t D, size_t L>
bool MDV<T, D, L>::close_enough(const T &a, const T &b, T m_tol)
{
    return abs(abs(a) - abs(b)) < m_tol;
}

*/