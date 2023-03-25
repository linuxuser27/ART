#ifndef _SRC_INC_SHAPES_HPP_
#define _SRC_INC_SHAPES_HPP_

#include "shapes.hpp"

template<typename T>
class sphereT_t final : public hittableT_t<T>
{
    pointT3_t<T> _center;
    T _radius;
public:
    sphereT_t() = default;
    sphereT_t(pointT3_t<T> cen, T r)
        : _center{ cen }
        , _radius{ r }
    { }
    virtual ~sphereT_t() = default;

    bool hit(rayT_t<T> const& r, T t_min, T t_max, typename hittableT_t<T>::hit_result_t& result) const override
    {
        vecT3_t<T> oc = r.origin() - _center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - _radius * _radius;
        auto discriminant = half_b * half_b - a * c;

        if (discriminant < 0.0f)
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
        set_face_normal(result, r, outward_normal);
        return true;
    }
};

#endif // _SRC_INC_SHAPES_HPP_