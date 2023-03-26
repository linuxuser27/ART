#ifndef _SRC_INC_UTILITY_HPP_
#define _SRC_INC_UTILITY_HPP_

#include <numbers>
#include <random>
#include <concepts>

template<typename T>
    requires std::floating_point<T>
T degrees_to_radians(T degrees)
{
    return degrees * std::numbers::pi_v<T> / 180;
}

// Get a random value from [0, 1)
template<typename T>
    requires std::floating_point<T>
T random_value()
{
    static std::uniform_real_distribution<T> distribution{ 0, 1 };
    static std::mt19937 generator;
    return distribution(generator);
}

// Get a random value from [min, max)
template<typename T>
    requires std::floating_point<T>
T random_value(T min, T max)
{
    return min + (max - min) * random_value<T>();
}

#endif // _SRC_INC_UTILITY_HPP_