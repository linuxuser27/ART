#ifndef _SRC_INC_VEC3_HPP_
#define _SRC_INC_VEC3_HPP_

#include <cmath>

template<typename T>
class vecT3_t final
{
    T _e[3];
public:
    using elem_t = T;

    vecT3_t() : _e{}
    { }
    vecT3_t(T x, T y, T z) : _e{ x, y, z }
    { }

    T x() const { return _e[0]; }
    T y() const { return _e[1]; }
    T z() const { return _e[2]; }

    vecT3_t operator-() const { return { -_e[0], -_e[1], -_e[2] }; }
    T operator[](int32_t i) const { return _e[i]; }
    T& operator[](int32_t i) { return _e[i]; }

    vecT3_t& operator+=(vecT3_t const &v)
    {
        _e[0] += v._e[0];
        _e[1] += v._e[1];
        _e[2] += v._e[2];
        return *this;
    }

    vecT3_t& operator*=(T const t)
    {
        _e[0] *= t;
        _e[1] *= t;
        _e[2] *= t;
        return *this;
    }

    vecT3_t& operator/=(T const t)
    {
        return *this *= 1/t;
    }

    T length() const
    {
        return std::sqrt(length_squared());
    }

    T length_squared() const
    {
        return _e[0]*_e[0] + _e[1]*_e[1] + _e[2]*_e[2];
    }
};

template<typename T>
using pointT3_t = vecT3_t<T>;

template<typename T>
using colorT_t = vecT3_t<T>;

template<typename T>
vecT3_t<T> operator+(vecT3_t<T> const &u, vecT3_t<T> const &v)
{
    return { u.x() + v.x(), u.y() + v.y(), u.z() + v.z() };
}

template<typename T>
vecT3_t<T> operator-(vecT3_t<T> const &u, vecT3_t<T> const &v)
{
    return { u.x() - v.x(), u.y() - v.y(), u.z() - v.z() };
}

template<typename T>
vecT3_t<T> operator*(vecT3_t<T> const &u, vecT3_t<T> const &v)
{
    return { u.x() * v.x(), u.y() * v.y(), u.z() * v.z() };
}

template<typename T>
vecT3_t<T> operator*(T t, vecT3_t<T> const &v)
{
    return { t * v.x(), t * v.y(), t * v.z() };
}

template<typename T>
vecT3_t<T> operator*(vecT3_t<T> const &v, T t)
{
    return t * v;
}

template<typename T>
vecT3_t<T> operator/(vecT3_t<T> v, T t)
{
    return (1 / t) * v;
}

template<typename T>
T dot(vecT3_t<T> const &u, vecT3_t<T> const &v)
{
    return u.x() * v.x()
         + u.y() * v.y()
         + u.z() * v.z();
}

template<typename T>
vecT3_t<T> cross(vecT3_t<T> const &u, vecT3_t<T> const &v)
{
    return { u.y() * v.z() - u.z() * v.y(),
             u.z() * v.x() - u.x() * v.z(),
             u.x() * v.y() - u.y() * v.x() };
}

template<typename T>
vecT3_t<T> unit_vector(vecT3_t<T> v)
{
    return v / v.length();
}

#endif // _SRC_INC_VEC3_HPP_
