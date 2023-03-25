#ifndef _SRC_INC_ART_HPP_
#define _SRC_INC_ART_HPP_

#include <limits>
#include <numbers>

template<typename T>
class artT_t final
{
public:
    using elem_t = T;

    //
    // Constants
    //
    static T constexpr infinity = std::numeric_limits<T>::infinity();
    static T constexpr pi = std::numbers::pi_v<T>;

    //
    // Utility Functions
    //
    static T degrees_to_radians(T degrees)
    {
        return degrees * pi / 180.0;
    }
};

#endif // _SRC_INC_ART_HPP_