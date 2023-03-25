#ifndef _SRC_INC_HITTABLE_HPP_
#define _SRC_INC_HITTABLE_HPP_

#include <vector>
#include <memory>

#include "ray.hpp"

template<typename T>
struct hit_resultT_t
{
    point3T_t<T> p;
    vec3T_t<T> normal;
    T t;
    bool front_face;
};

template<typename T>
void set_face_normal(hit_resultT_t<T>& result, rayT_t<T> const r, vec3T_t<T> const outward_normal)
{
    result.front_face = dot(r.direction(), outward_normal) < 0;
    result.normal = result.front_face ? outward_normal : -outward_normal;
}

template<typename T>
class hittableT_t
{
public:
    using hit_result_t = hit_resultT_t<T>;
    virtual bool hit(rayT_t<T> const& r, T t_min, T t_max, hit_result_t& result) const = 0;
};

template<typename T>
class hittableT_list_t final : public hittableT_t<T>
{
    std::vector<std::shared_ptr<hittableT_t<T>>> _hittables;
public:
    using hittable_t = hittableT_t<T>;
    using hit_result_t = typename hittable_t::hit_result_t;

    hittableT_list_t() = default;
    virtual ~hittableT_list_t() = default;

    void add(std::shared_ptr<hittable_t>&& hittable)
    {
        _hittables.push_back(hittable);
    }

    void clear()
    {
        _hittables.clear();
    }

    bool hit(rayT_t<T> const& r, T t_min, T t_max, hit_result_t& result) const override
    {
        hit_result_t temp;
        bool hit_anything = false;
        auto closest_so_far = t_max;
        for (auto const& hittable : _hittables)
        {
            if (hittable->hit(r, t_min, closest_so_far, temp))
            {
                hit_anything = true;
                closest_so_far = temp.t;
                result = temp;
            }
        }
        return hit_anything;
    }
};

#endif // _SRC_INC_HITTABLE_HPP_