#ifndef _SRC_INC_MATERIALS_HPP_
#define _SRC_INC_MATERIALS_HPP_

#include <concepts>

#include "ray.hpp"
#include "elements.hpp"
#include "utility.hpp"

// Represents diffuse (matte) material
template<typename T, typename FORMULA>
class diffuse_baseT_t final : public materialT_t<T>
{
    vec3T_t<T> _albedo;
public:
    using hit_result_t = hit_resultT_t<T>;
    using scatter_result_t = typename materialT_t<T>::scatter_result_t;

    diffuse_baseT_t(vec3T_t<T> const& a)
        : _albedo{ a }
    { }
    virtual ~diffuse_baseT_t() = default;

    bool scatter(rayT_t<T> const& ray, hit_result_t const& hit, scatter_result_t& result) const override
    {
        point3T_t<T> scatter_direction = FORMULA{}(hit);

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = hit.normal;

        result.scattered = { hit.p, scatter_direction };
        result.attenuation = _albedo;
        return true;
    }
};

// Diffuse materials with various formula
// Requires floating point data type
template<typename T>
    requires std::floating_point<T>
class diffuseT final
{
public:
    diffuseT() = delete;

private: // static
    static vec3T_t<T> random_vec3(T min, T max)
    {
        return { random_value(min, max), random_value(min, max), random_value(min, max) };
    }

    static vec3T_t<T> random_in_unit_sphere()
    {
        while (true)
        {
            auto p = random_vec3(-1, 1);
            if (p.length_squared() >= 1)
                continue;
            return p;
        }
    }

    static vec3T_t<T> random_unit_vector()
    {
        return unit_vector(random_in_unit_sphere());
    }

    static vec3T_t<T> random_in_hemisphere(vec3T_t<T> const& normal)
    {
        vec3T_t<T> in_unit_sphere = random_in_unit_sphere();
        return (dot(in_unit_sphere, normal) > 0) // In the same hemisphere as the normal
            ? in_unit_sphere
            : -in_unit_sphere;
    }

    //
    // Diffuse formulation options found in
    // sections 8.1, 8.5 and 8.6
    //
    struct simple_formula final
    {
        point3T_t<T> operator()(hit_resultT_t<T> const& res) const
        {
            return res.normal + random_in_unit_sphere();
        }
    };

    struct lambertian_formula final
    {
        point3T_t<T> operator()(hit_resultT_t<T> const& res) const
        {
            return res.normal + random_unit_vector();
        }
    };

    struct hemisphere_scattering_formula final
    {
        point3T_t<T> operator()(hit_resultT_t<T> const& res) const
        {
            return random_in_hemisphere(res.normal);
        }
    };

public:
    using simple_t = diffuse_baseT_t<T, simple_formula>;
    using lambertian_t = diffuse_baseT_t<T, lambertian_formula>;
    using hemisphere_scattering_t = diffuse_baseT_t<T, hemisphere_scattering_formula>;
};

template<typename T>
class metalT_t final : public materialT_t<T>
{
    static vec3T_t<T> reflect(vec3T_t<T> const& v, vec3T_t<T> const& n)
    {
        return v - 2 * dot(v,n) * n;
    }

    vec3T_t<T> _albedo;
    T _fuzz;
public:
    using hit_result_t = hit_resultT_t<T>;
    using scatter_result_t = typename materialT_t<T>::scatter_result_t;

    // Fuzz is a factor [0 to 1] - no fuzz to lots
    metalT_t(vec3T_t<T> const& a, T fuzz = 0)
        : _albedo{ a }
        , _fuzz{ std::abs(fuzz) < 1 ? std::abs(fuzz) : 1 }
    { }
    virtual ~metalT_t() = default;

    bool scatter(rayT_t<T> const& ray, hit_result_t const& hit, scatter_result_t& result) const override
    {
        auto reflected = reflect(unit_vector(ray.direction()), hit.normal);
        result.scattered = { hit.p, reflected };
        result.attenuation = _albedo;
        return (dot(result.scattered.direction(), hit.normal) > 0);
    }
};

#endif // _SRC_INC_MATERIALS_HPP_
