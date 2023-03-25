#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <vector>
#include <memory>

#include <art.hpp>
#include <vec3.hpp>
#include <ray.hpp>
#include <hittable.hpp>
#include <shapes.hpp>

namespace
{
    // Type aliases
    using art_t = artT_t<float>;
    using vec3_t = vecT3_t<art_t::elem_t>;
    using point3_t = pointT3_t<vec3_t::elem_t>;
    using color_t = colorT_t<vec3_t::elem_t>;
    using ray_t = rayT_t<vec3_t::elem_t>;
    using sphere_t = sphereT_t<vec3_t::elem_t>;
    using hittable_list_t = hittableT_list_t<vec3_t::elem_t>;

    struct pixel_t final
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    color_t const g_white{ 1.0f, 1.0f, 1.0f }; // White
    color_t const g_gradient_start = g_white;
    color_t const g_gradient_end{ 0.5f, 0.7f, 1.0f }; // Light blue

    void write_pixel(color_t pixel_color, std::vector<pixel_t>& data)
    {
        // Write the translated [0,255] value of each color component.
        auto ir = static_cast<uint8_t>(255.999 * pixel_color.x());
        auto ig = static_cast<uint8_t>(255.999 * pixel_color.y());
        auto ib = static_cast<uint8_t>(255.999 * pixel_color.z());
        data.push_back({ir, ig, ib});
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
        auto t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * g_gradient_start + t * g_gradient_end;
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

    // World
    hittable_list_t world;
    world.add(std::make_shared<sphere_t>(point3_t{ 0, 0, -1.0f }, 0.5f));
    world.add(std::make_shared<sphere_t>(point3_t{ 0, -100.5f, -1.0f }, 100.0f));

    //
    // Camera
    //
    float const viewport_height = 2.0f; // Y-axis
    float const viewport_width = aspect_ratio * viewport_height; // X-axis
    float const focal_length = 1.0f; // Z-axis

    point3_t const origin = { 0, 0, 0 };
    vec3_t const horizontal = { viewport_width, 0, 0 };
    vec3_t const vertical = { 0, viewport_height, 0 };

    // Represents the lowest left corner of the viewport.
    // Note this is the focal length away from the origin.
    vec3_t const lower_left_corner = origin - (horizontal / 2.0f) - (vertical / 2.0f) - vec3_t{ 0, 0, focal_length };

    //
    // Render
    //
    std::vector<pixel_t> image_data;
    image_data.reserve(image_width * image_height);
    for (int32_t j = image_height - 1; j >= 0; --j)
    {
        for (int32_t i = 0; i < image_width; ++i)
        {
            auto u = float(i) / (image_width - 1);
            auto v = float(j) / (image_height - 1);

            // Create a ray from the origin across the viewport
            ray_t r{ origin, lower_left_corner + u * horizontal + v * vertical - origin };

            // Given the ray compute the color of the pixel the ray intersects
            color_t pixel_color = ray_color(r, world);

            // Write the computed color
            write_pixel(pixel_color, image_data);
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