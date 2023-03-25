#ifndef _SRC_INC_RAY_HPP_
#define _SRC_INC_RAY_HPP_

#include "vec3.hpp"

template<typename T>
class rayT_t final
{
    point3T_t<T> _orig;
    vec3T_t<T> _dir;
public:
    rayT_t() = default;
    rayT_t(point3T_t<T> origin, vec3T_t<T> direction)
        : _orig{ origin }
        , _dir{ direction }
    { }

    point3T_t<T> origin() const { return _orig; }
    vec3T_t<T> direction() const { return _dir; }

    vec3T_t<T> at(T t) const
    {
        return _orig + (t * _dir);
    }
};

#endif // _SRC_INC_RAY_HPP_
