#ifndef _SRC_INC_VEC3_HPP_
#define _SRC_INC_VEC3_HPP_

#include <cmath>

template<typename T>
class vec3T_t final
{
    T _e[3];
public:
    using elem_t = T;

    vec3T_t() : _e{}
    { }
    vec3T_t(T x, T y, T z) : _e{ x, y, z }
    { }

    T x() const { return _e[0]; }
    T y() const { return _e[1]; }
    T z() const { return _e[2]; }

    vec3T_t operator-() const { return { -_e[0], -_e[1], -_e[2] }; }

    vec3T_t& operator+=(vec3T_t const &v)
    {
        _e[0] += v._e[0];
        _e[1] += v._e[1];
        _e[2] += v._e[2];
        return *this;
    }

    vec3T_t& operator*=(T const t)
    {
        _e[0] *= t;
        _e[1] *= t;
        _e[2] *= t;
        return *this;
    }

    vec3T_t& operator/=(T const t)
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

    bool near_zero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        auto const epsilon = T(1e-8);
        return (std::abs(_e[0]) < epsilon)
            && (std::abs(_e[1]) < epsilon)
            && (std::abs(_e[2]) < epsilon);
    }
};

template<typename T>
using point3T_t = vec3T_t<T>;

template<typename T>
vec3T_t<T> operator+(vec3T_t<T> const &u, vec3T_t<T> const &v)
{
    return { u.x() + v.x(), u.y() + v.y(), u.z() + v.z() };
}

template<typename T>
vec3T_t<T> operator-(vec3T_t<T> const &u, vec3T_t<T> const &v)
{
    return { u.x() - v.x(), u.y() - v.y(), u.z() - v.z() };
}

template<typename T>
vec3T_t<T> operator*(vec3T_t<T> const &u, vec3T_t<T> const &v)
{
    return { u.x() * v.x(), u.y() * v.y(), u.z() * v.z() };
}

template<typename T>
vec3T_t<T> operator*(T t, vec3T_t<T> const &v)
{
    return { t * v.x(), t * v.y(), t * v.z() };
}

template<typename T>
vec3T_t<T> operator*(vec3T_t<T> const &v, T t)
{
    return t * v;
}

template<typename T>
vec3T_t<T> operator/(vec3T_t<T> v, T t)
{
    return (1 / t) * v;
}

template<typename T>
T dot(vec3T_t<T> const &u, vec3T_t<T> const &v)
{
    return u.x() * v.x()
         + u.y() * v.y()
         + u.z() * v.z();
}

template<typename T>
vec3T_t<T> cross(vec3T_t<T> const &u, vec3T_t<T> const &v)
{
    return { u.y() * v.z() - u.z() * v.y(),
             u.z() * v.x() - u.x() * v.z(),
             u.x() * v.y() - u.y() * v.x() };
}

template<typename T>
vec3T_t<T> unit_vector(vec3T_t<T> v)
{
    return v / v.length();
}

#endif // _SRC_INC_VEC3_HPP_
