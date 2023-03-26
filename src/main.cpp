#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

#include <art.hpp>
#include <vec3.hpp>
#include <ray.hpp>
#include <camera.hpp>
#include <hittable.hpp>
#include <shapes.hpp>

namespace
{
    // Type aliases
    using art_t = artT_t<float>;
    using vec3_t = vec3T_t<art_t::elem_t>;
    using point3_t = point3T_t<vec3_t::elem_t>;
    using color_t = vec3_t;
    using ray_t = rayT_t<vec3_t::elem_t>;
    using camera_t = cameraT_t<art_t::elem_t>;
    using sphere_t = sphereT_t<vec3_t::elem_t>;
    using hittable_list_t = hittableT_list_t<vec3_t::elem_t>;

    struct pixel_t final
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    vec3_t rnd_vec3(art_t::elem_t min, art_t::elem_t max)
    {
        return { art_t::rnd_val(min, max), art_t::rnd_val(min, max), art_t::rnd_val(min, max) };
    }

    vec3_t random_in_unit_sphere()
    {
        while (true)
        {
            auto p = rnd_vec3(-1, 1);
            if (p.length_squared() >= 1)
                continue;
            return p;
        }
    }

    vec3_t random_unit_vector()
    {
        return unit_vector(random_in_unit_sphere());
    }

    vec3_t random_in_hemisphere(vec3_t const& normal)
    {
        vec3_t in_unit_sphere = random_in_unit_sphere();
        return (dot(in_unit_sphere, normal) > 0) // In the same hemisphere as the normal
            ? in_unit_sphere
            : -in_unit_sphere;
    }

    //
    // Diffuse formulation options found in
    // sections 8.1, 8.5 and 8.6
    //
    struct diffuse_simple_t final
    {
        point3_t operator()(hittable_list_t::hit_result_t const& res) const
        {
            return res.p + res.normal + random_in_unit_sphere();
        }
    };

    struct diffuse_lambertian_t final
    {
        point3_t operator()(hittable_list_t::hit_result_t const& res) const
        {
            return res.p + res.normal + random_unit_vector();
        }
    };

    struct diffuse_hemisphere_scattering_t final
    {
        point3_t operator()(hittable_list_t::hit_result_t const& res) const
        {
            return res.p + random_in_hemisphere(res.normal);
        }
    };

    color_t const g_black{ 0, 0, 0 }; // RGB:  0,  0,  0
    color_t const g_white{ 1, 1, 1 }; // RGB:255,255,255
    color_t const g_gradient_start = g_white;
    color_t const g_gradient_end{ 0.5f, 0.7f, 1 }; // Light blue

    pixel_t create_pixel(color_t pixel_color, int32_t samples)
    {
        // Divide the color by the number of samples.
        auto scale = art_t::elem_t(1) / samples;

        // Gamma-correct for gamma=2.0 (that is, raise color to power 1/gamma)
        auto r = std::sqrt(pixel_color.x() * scale);
        auto g = std::sqrt(pixel_color.y() * scale);
        auto b = std::sqrt(pixel_color.z() * scale);

        // Write the translated [0,255] value of each color component.
        auto min = art_t::elem_t(0);
        auto max = art_t::elem_t(.999);
        auto ir = static_cast<uint8_t>(256 * std::clamp(r, min, max));
        auto ig = static_cast<uint8_t>(256 * std::clamp(g, min, max));
        auto ib = static_cast<uint8_t>(256 * std::clamp(b, min, max));
        return { ir, ig, ib };
    }

    template<typename DIFFUSE>
    color_t ray_color(ray_t r, hittable_list_t const& world, int32_t max_ray_bounce = 50)
    {
        // Accumlation factor for ray bounce.
        auto acc_factor = art_t::elem_t(1);

        // Instead of recursion, iterate.
        for (int32_t i = 0; i < max_ray_bounce; ++i)
        {
            hittable_list_t::hit_result_t result;

            // Check if an object was hit.
            // Use 0.001 to address "shadow acne".
            if (world.hit(r, 0.001, art_t::infinity, result))
            {
                point3_t target = DIFFUSE{}(result);
                r = { result.p, target - result.p };
                acc_factor *= art_t::elem_t(0.5);
                continue;
            }

            // No objects hit, produce a gradient background.
            vec3_t unit_direction = unit_vector(r.direction());

            // The color is gradient along the Y-axis.
            auto t = art_t::elem_t(0.5) * (unit_direction.y() + 1);
            return acc_factor * ((1 - t) * g_gradient_start + t * g_gradient_end);
        }

        // If we've exceeded the ray bounce limit, no more light is gathered.
        return g_black;
    }
}

int main()
{
    //
    // Image
    //
    float const aspect_ratio = 16.0f / 9.0f;
    int32_t const image_width = 400;
    int32_t const image_height = static_cast<int32_t>(image_width / aspect_ratio);
    int32_t const samples_per_pixel = 10; // Antialiasing sampling rate

    //
    // World
    //
    hittable_list_t world;
    world.add(std::make_shared<sphere_t>(point3_t{ 0, 0, -1 }, 0.5f));
    world.add(std::make_shared<sphere_t>(point3_t{ 0, -100.5f, -1 }, 100));

    //
    // Camera
    //
    camera_t camera{ aspect_ratio };

    //
    // Render
    //
    std::vector<pixel_t> image_data;
    image_data.reserve(image_width * image_height);
    for (int32_t j = image_height - 1; j >= 0; --j)
    {
        for (int32_t i = 0; i < image_width; ++i)
        {
            // Using sampling, apply antialiasing to compute the pixel color.
            color_t pixel_color{};
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + art_t::rnd_val()) / (image_width - 1);
                auto v = (j + art_t::rnd_val()) / (image_height - 1);

                // Create a ray from the camera to a point on the viewport.
                ray_t r = camera.get_ray(u, v);

                // Given the ray compute the color of the pixel the ray intersects.
                pixel_color += ray_color<diffuse_hemisphere_scattering_t>(r, world);
            }

            // Create the pixel.
            image_data.push_back(create_pixel(pixel_color, samples_per_pixel));
        }
    }

    //
    // Write out PPM - https://wikipedia.org/wiki/Netpbm
    //
    std::printf("P3\n"
                "%d %d\n"
                "255\n", image_width, image_height);
    for (auto const& p : image_data)
    {
        std::printf("%u %u %u\n", p.r, p.g, p.b);
    }

    return EXIT_SUCCESS;
}