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

    color_t const g_white{ 1, 1, 1 }; // White
    color_t const g_gradient_start = g_white;
    color_t const g_gradient_end{ 0.5f, 0.7f, 1 }; // Light blue

    pixel_t create_pixel(color_t pixel_color, int32_t samples)
    {
        // Divide the color by the number of samples.
        auto scale = art_t::elem_t(1) / samples;
        auto r = pixel_color.x() * scale;
        auto g = pixel_color.y() * scale;
        auto b = pixel_color.z() * scale;

        // Write the translated [0,255] value of each color component.
        auto min = art_t::elem_t(0);
        auto max = art_t::elem_t(.999);
        auto ir = static_cast<uint8_t>(256 * std::clamp(r, min, max));
        auto ig = static_cast<uint8_t>(256 * std::clamp(g, min, max));
        auto ib = static_cast<uint8_t>(256 * std::clamp(b, min, max));
        return { ir, ig, ib };
    }

    color_t ray_color(ray_t const& r, hittable_list_t const& world)
    {
        // Check if an object was hit.
        hittable_list_t::hit_result_t result;
        if (world.hit(r, 0, art_t::infinity, result))
            return 0.5f * (result.normal + g_white);

        // No objects hit, produce a gradient background
        vec3_t unit_direction = unit_vector(r.direction());

        // The color is gradient along the Y-axis
        auto t = 0.5f * (unit_direction.y() + 1);
        return (1 - t) * g_gradient_start + t * g_gradient_end;
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

                // Create a ray from the camera to a point on the viewport
                ray_t r = camera.get_ray(u, v);

                // Given the ray compute the color of the pixel the ray intersects
                pixel_color += ray_color(r, world);
            }

            // Create the pixel
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