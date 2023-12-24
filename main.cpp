#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "colour.h"

#include <iostream>
#include <chrono>
#include <ctime>




vec3 ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + vec3(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*vec3(1.0, 1.0, 1.0) + a*vec3(0.5, 0.7, 1.0);
}


int main() {

    float aspect_ratio = 16.0 / 9.0;
    int image_width = 600;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    hittable_list world;
    // world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // world.add(make_shared<sphere>(point3(0, 1,-0.8), 0.3));
    // world.add(make_shared<sphere>(point3(1, -1,-0.8), 0.4));
    // world.add(make_shared<sphere>(point3(-1, 1,-0.8), 0.5));
    // world.add(make_shared<sphere>(point3(-1, -1,-0.5), 0.2));
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    auto camera_center = point3(0,0,0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    auto start = std::chrono::system_clock::now();

    for (int j = 0; j < image_height; ++j){

        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);
            vec3 pixel_colour = ray_color(r, world);

            write_colour(std::cout, pixel_colour);
        }
    }
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::clog << "\n Done! \n" 
        << "Elapsed time: " << elapsed_seconds.count() << "s";
}