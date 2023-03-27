#ifndef _SRC_INC_CAMERA_HPP_
#define _SRC_INC_CAMERA_HPP_

#include <cmath>

#include "vec3.hpp"
#include "ray.hpp"
#include "utility.hpp"

template<typename T>
class cameraT_t final
{
    vec3T_t<T> _u;
    vec3T_t<T> _v;
    vec3T_t<T> _w;
    point3T_t<T> _origin;
    point3T_t<T> _lower_left_corner;
    vec3T_t<T> _horizontal;
    vec3T_t<T> _vertical;
    T _lens_radius;
public:
    cameraT_t(
        point3T_t<T> lookfrom,
        point3T_t<T> lookat,
        vec3T_t<T> vup,
        T vfov, // vertical field-of-view in degrees
        T aspect_ratio,
        T aperture,
        T focus_dist)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        T viewport_height = h * 2; // Y-axis
        T viewport_width = aspect_ratio * viewport_height; // X-axis

        _w = unit_vector(lookfrom - lookat);
        _u = unit_vector(cross(vup, _w));
        _v = cross(_w, _u);

        _origin = lookfrom;
        _horizontal = focus_dist * viewport_width * _u;
        _vertical = focus_dist * viewport_height * _v;

        // Represents the lowest left corner of the viewport.
        // Note this is the 'w' away from the camera origin.
        _lower_left_corner = _origin - (_horizontal / T(2)) - (_vertical / T(2)) - (focus_dist * _w);
        _lens_radius = aperture / 2;
    }

    rayT_t<T> get_ray(T s, T t) const
    {
        vec3T_t<T> rd = _lens_radius * random_in_unit_disk();
        vec3T_t<T> offset = _u * rd.x() + _v * rd.y();
        return {
            _origin + offset,
            _lower_left_corner + s * _horizontal + t * _vertical - _origin - offset
        };
    }

private: // static
    // Generate random rays for defocus blur effect (depth of field)
    static vec3T_t<T> random_in_unit_disk()
    {
        while (true)
        {
            auto p = vec3T_t<T>{ random_value<T>(-1, 1), random_value<T>(-1, 1), 0 };
            if (p.length_squared() >= 1)
                continue;
            return p;
        }
    }
};

#endif // _SRC_INC_CAMERA_HPP_