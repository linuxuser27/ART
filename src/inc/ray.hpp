#ifndef _SRC_INC_RAY_HPP_
#define _SRC_INC_RAY_HPP_

#include "vec3.hpp"

template<typename T>
class rayT_t final
{
    pointT3_t<T> _orig;
    vecT3_t<T> _dir;
public:
    rayT_t() = default;
    rayT_t(pointT3_t<T> origin, vecT3_t<T> direction)
        : _orig{ origin }
        , _dir{ direction }
    { }

    pointT3_t<T> origin() const { return _orig; }
    vecT3_t<T> direction() const { return _dir; }

    vecT3_t<T> at(T t) const
    {
        return _orig + (t * _dir);
    }
};

#endif // _SRC_INC_RAY_HPP_
