#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>

#include <vec3.hpp>
#include <ray.hpp>
#include <camera.hpp>
#include <elements.hpp>
#include <materials.hpp>
#include <shapes.hpp>
#include <utility.hpp>

namespace
{
    // Type aliases
    using elem_t = float;
    using vec3_t = vec3T_t<elem_t>;
    using point3_t = point3T_t<vec3_t::elem_t>;
    using color_t = vec3_t;
    using ray_t = rayT_t<vec3_t::elem_t>;
    using camera_t = cameraT_t<elem_t>;
    using sphere_t = sphereT_t<vec3_t::elem_t>;
    using hittable_list_t = hittableT_list_t<vec3_t::elem_t>;
    using material_t = materialT_t<vec3_t::elem_t>;
    using metal_t = metalT_t<vec3_t::elem_t>;
    using diffuse = diffuseT<vec3_t::elem_t>;

    struct pixel_t final
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    color_t const g_black{ 0, 0, 0 }; // RGB:  0,  0,  0
    color_t const g_red{ 255, 0, 0 }; // RGB:255,  0,  0
    color_t const g_white{ 1, 1, 1 }; // RGB:255,255,255
    color_t const g_gradient_start = g_white;
    color_t const g_gradient_end{ 0.5f, 0.7f, 1 }; // Light blue

    pixel_t create_pixel(color_t pixel_color, int32_t samples)
    {
        // Divide the color by the number of samples.
        auto scale = elem_t(1) / samples;

        // Gamma-correct for gamma=2.0 (that is, raise color to power 1/gamma)
        auto r = std::sqrt(pixel_color.x() * scale);
        auto g = std::sqrt(pixel_color.y() * scale);
        auto b = std::sqrt(pixel_color.z() * scale);

        // Write the translated [0,255] value of each color component.
        auto min = elem_t(0);
        auto max = elem_t(.999);
        auto ir = static_cast<uint8_t>(256 * std::clamp(r, min, max));
        auto ig = static_cast<uint8_t>(256 * std::clamp(g, min, max));
        auto ib = static_cast<uint8_t>(256 * std::clamp(b, min, max));
        return { ir, ig, ib };
    }

    color_t ray_color(ray_t r, hittable_list_t const& world, int32_t max_ray_bounce = 50)
    {
        // Accumlation factor for ray bounce.
        auto acc_factor = color_t{1, 1, 1};

        // Instead of recursion, iterate.
        for (int32_t i = 0; i < max_ray_bounce; ++i)
        {
            hittable_list_t::hit_result_t hit;

            // Check if an object was hit.
            // Use 0.001 to address "shadow acne".
            if (world.hit(r, 0.001, std::numeric_limits<elem_t>::infinity(), hit))
            {
                material_t::scatter_result_t scatter;
                if (!hit.material->scatter(r, hit, scatter))
                    return g_black;

                r = scatter.scattered;
                acc_factor = acc_factor * scatter.attenuation;
                continue;
            }

            // No objects hit, produce a gradient background.
            vec3_t unit_direction = unit_vector(r.direction());

            // The color is gradient along the Y-axis.
            auto t = elem_t(0.5) * (unit_direction.y() + 1);
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

    auto material_ground = std::make_shared<diffuse::lambertian_t>(color_t{ 0.8, 0.8, 0 });
    auto material_center = std::make_shared<diffuse::lambertian_t>(color_t{ 0.7, 0.3, 0.3 });
    auto material_left   = std::make_shared<metal_t>(color_t{ 0.8, 0.8, 0.8 }, 0.3);
    auto material_right  = std::make_shared<metal_t>(color_t{ 0.8, 0.6, 0.2 }, 1.0);

    world.add(std::make_shared<sphere_t>(point3_t{  0, -100.5f, -1 }, 100.0f, material_ground));
    world.add(std::make_shared<sphere_t>(point3_t{  0,    0.0f, -1 },   0.5f, material_center));
    world.add(std::make_shared<sphere_t>(point3_t{ -1,    0.0f, -1 },   0.5f, material_left));
    world.add(std::make_shared<sphere_t>(point3_t{  1,    0.0f, -1 },   0.5f, material_right));

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
                auto u = (i + random_value<elem_t>()) / (image_width - 1);
                auto v = (j + random_value<elem_t>()) / (image_height - 1);

                // Create a ray from the camera to a point on the viewport.
                ray_t r = camera.get_ray(u, v);

                // Given the ray compute the color of the pixel the ray intersects.
                pixel_color += ray_color(r, world);
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