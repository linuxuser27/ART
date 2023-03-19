#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <vector>

#include <vec3.hpp>
#include <ray.hpp>

namespace
{
    // Type aliases
    using vec3_t = vecT3_t<float>;
    using point3_t = vec3_t;
    using color_t = vec3_t;
    using ray_t = rayT_t<vec3_t::elem_t>;

    struct pixel_t final
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    color_t const g_gradient_start{ 1.0f, 1.0f, 1.0f }; // White
    color_t const g_gradient_end{ 0.5f, 0.7f, 1.0f }; // Light blue

    void write_pixel(color_t pixel_color, std::vector<pixel_t>& data)
    {
        // Write the translated [0,255] value of each color component.
        auto ir = static_cast<uint8_t>(255.999 * pixel_color.x());
        auto ig = static_cast<uint8_t>(255.999 * pixel_color.y());
        auto ib = static_cast<uint8_t>(255.999 * pixel_color.z());
        data.push_back({ir, ig, ib});
    }

    bool hit_sphere(point3_t const& center, float radius, ray_t const& r)
    {
        vec3_t oc = r.origin() - center;
        auto a = dot(r.direction(), r.direction());
        auto b = 2.0f * dot(oc, r.direction());
        auto c = dot(oc, oc) - radius*radius;
        auto discriminant = b*b - 4*a*c;
        return (discriminant > 0);
    }

    color_t ray_color(ray_t const& r)
    {
        if (hit_sphere(point3_t{ 0, 0, -1.0f }, 0.5f, r))
            return color_t{ 1.0f, 0, 0 };

        vec3_t unit_direction = unit_vector(r.direction());
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

    //
    // Camera
    //
    float const viewport_height = 2.0f;
    float const viewport_width = aspect_ratio * viewport_height;
    float const focal_length = 1.0f;

    point3_t const origin = { 0, 0, 0 };
    vec3_t const horizontal = { viewport_width, 0, 0 };
    vec3_t const vertical = { 0, viewport_height, 0 };
    vec3_t const lower_left_corner = origin - (horizontal / 2.0f) - (vertical / 2.0f) - vec3_t{ 0, 0, focal_length };

    //
    // Render
    //
    std::vector<pixel_t> image_data;
    image_data.reserve(image_width * image_height);
    for (int32_t j = image_height - 1; j >= 0; --j)
    {
        //std::fprintf(stderr, "Scanlines remaining: %3d\n", j);
        for (int32_t i = 0; i < image_width; ++i)
        {
            auto u = float(i) / (image_width - 1);
            auto v = float(j) / (image_height - 1);
            ray_t r{ origin, lower_left_corner + u * horizontal + v * vertical - origin };
            color_t pixel_color = ray_color(r);
            write_pixel(pixel_color, image_data);
        }
    }

    //
    // Write out PPM - https://en.wikipedia.org/wiki/Netpbm
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