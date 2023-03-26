#ifndef _SRC_INC_ART_HPP_
#define _SRC_INC_ART_HPP_

#include <limits>
#include <numbers>
#include <random>
#include <concepts>

template<typename T>
    requires std::floating_point<T>
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
        return degrees * pi / 180;
    }

    // Get a random value from [0, 1)
    static T rnd_val()
    {
        static std::uniform_real_distribution<T> distribution{ 0, 1 };
        static std::mt19937 generator;
        return distribution(generator);
    }

    // Get a random value from [min, max)
    static T rnd_val(T min, T max)
    {
        return min + (max - min) * rnd_val();
    }
};

#endif // _SRC_INC_ART_HPP_