#ifndef _SRC_INC_CAMERA_HPP_
#define _SRC_INC_CAMERA_HPP_

#include "vec3.hpp"
#include "ray.hpp"

template<typename T>
class cameraT_t final
{
    point3T_t<T> _origin;
    vec3T_t<T> _horizontal;
    vec3T_t<T> _vertical;
    point3T_t<T> _lower_left_corner;
public:
    cameraT_t(T aspect_ratio)
    {
        T viewport_height = 2; // Y-axis
        T viewport_width = aspect_ratio * viewport_height; // X-axis
        T focal_length = 1; // Z-axis

        _origin = { 0, 0, 0 };
        _horizontal = { viewport_width, 0, 0 };
        _vertical = { 0, viewport_height, 0 };

        // Represents the lowest left corner of the viewport.
        // Note this is the focal length away from the camera origin.
        _lower_left_corner = _origin - (_horizontal / T(2)) - (_vertical / T(2)) - vec3T_t<T>{ 0, 0, focal_length };
    }

    rayT_t<T> get_ray(T u, T v) const
    {
        return { _origin, _lower_left_corner + u * _horizontal + v * _vertical - _origin };
    }
};

#endif // _SRC_INC_CAMERA_HPP_