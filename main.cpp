#include "colour.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>
#include <chrono>
#include <ctime>

double hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    

    if (discriminant < 0){
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) / 2.0 * a);
    }
}


vec3 ray_colour(const ray& r){
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);

    if (t > 0){
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-a)*vec3(1.0, 1.0, 1.0) + a*vec3(0.5, 0.3, 0.9);
}


int main() {

    float aspect_ratio = 16.0 / 9.0;
    int image_width = 600;
    int image_height = static_cast<int>(image_width / aspect_ratio);

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
            vec3 pixel_colour = ray_colour(r);

            write_colour(std::cout, pixel_colour);
        }
    }
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::clog << "\n Done! \n" 
        << "Elapsed time: " << elapsed_seconds.count() << "s";
}