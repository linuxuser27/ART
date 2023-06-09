#ifndef _SRC_INC_SHAPES_HPP_
#define _SRC_INC_SHAPES_HPP_

#include <cmath>
#include "elements.hpp"

template<typename T>
class sphereT_t final : public hittableT_t<T>
{
    point3T_t<T> _center;
    T _radius;
    std::shared_ptr<materialT_t<T>> _material;
public:
    using hit_result_t = typename hittableT_t<T>::hit_result_t;

    sphereT_t() = default;
    sphereT_t(point3T_t<T> cen, T r, std::shared_ptr<materialT_t<T>> material)
        : _center{ cen }
        , _radius{ r }
        , _material{ std::move(material) }
    { }
    virtual ~sphereT_t() = default;

    bool hit(rayT_t<T> const& r, T t_min, T t_max, hit_result_t& result) const override
    {
        vec3T_t<T> oc = r.origin() - _center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - _radius * _radius;
        auto discriminant = half_b * half_b - a * c;

        if (discriminant < 0)
            return false;

        // Find the nearest root that lies in the acceptable range.
        auto disc_sqrt = std::sqrt(discriminant);
        auto root = (-half_b - disc_sqrt) / a;
        if (root < t_min || t_max < root)
        {
            root = (-half_b + disc_sqrt) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        result.t = root;
        result.p = r.at(result.t);
        auto outward_normal = (result.p - _center) / _radius;
        result.set_face_normal(r, outward_normal);
        result.material = _material;
        return true;
    }
};

#endif // _SRC_INC_SHAPES_HPP_