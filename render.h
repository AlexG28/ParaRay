#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <ctime>


class renderer2{

    public: 
        renderer2() {
            initialize_camera();
            initialize_world();
        }

        void general_render() {
            auto start = std::chrono::system_clock::now();

            cam.render(world);

            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;

            std::clog << "\n Done! \n" << "Elapsed time: " << elapsed_seconds.count() << "s";
        }

        void specific_parallel_render(int threadID) {
            cam.parallel_render(world, threadID);
        }

        void parallel_render() {
            int numberOFThreads = 16;
            
            std::vector<std::thread> threads;

            for (int i = 0; i < numberOFThreads; i++) {
                threads.emplace_back(&renderer2::specific_parallel_render, this, i);
            }

            auto start = std::chrono::system_clock::now();

            for (auto& thread : threads) {
                thread.join();
            }

            std::cout << "All threads have completed.\n Printing to file\n" << std::endl;
            cam.printBuffer();

            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;

            std::clog << "\n Done! \n" << "Elapsed time: " << elapsed_seconds.count() << "s";
        }
   
    private: 
        hittable_list world;
        camera cam;

        void initialize_camera() {
            cam.aspect_ratio = 16.0 / 9.0;
            cam.image_width = 1200;
            cam.samples_per_pixel = 10;
            cam.max_depth = 50;

            cam.vfov = 20;
            cam.lookfrom = point3(13, 2, 3);
            cam.lookat = point3(0, 0, 0);
            cam.vup = vec3(0, 1, 0);

            cam.defocus_angle = 0.6;
            cam.focus_dist = 10.0;
        }

        void initialize_world() {
            auto ground_material = make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
            world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

            for (int a = -11; a < 11; a++) {
                for (int b = -11; b < 11; b++) {
                    auto choose_mat = random_double();
                    point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

                    if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                        shared_ptr<material> sphere_material;

                        if (choose_mat < 0.8) {
                            // diffuse
                            auto albedo = vec3::random() * vec3::random();
                            sphere_material = make_shared<lambertian>(albedo);
                            world.add(make_shared<sphere>(center, 0.2, sphere_material));
                        }
                        else if (choose_mat < 0.95) {
                            // metal
                            auto albedo = vec3::random(0.5, 1);
                            auto fuzz = random_double(0, 0.5);
                            sphere_material = make_shared<metal>(albedo, fuzz);
                            world.add(make_shared<sphere>(center, 0.2, sphere_material));
                        }
                        else {
                            // glass
                            sphere_material = make_shared<dielectric>(1.5);
                            world.add(make_shared<sphere>(center, 0.2, sphere_material));
                        }
                    }
                }
            }

            auto material1 = make_shared<dielectric>(1.5);
            world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

            auto material2 = make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
            world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

            auto material3 = make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
            world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
        }
};

#endif