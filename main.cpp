#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "colour.h"
#include "camera.h"
#include "render.h"

#include <iostream>
#include <chrono>
#include <ctime>


int main(int argc, char* argv[]) {
    
    if (argc == 1 || argc > 2) {
        std::cout << "please specify how you would like to render the image: \n"; 
        std::cout << "-m for multi-threaded render\n";
        std::cout << "-s for single-threaded render\n";
    }
    else {
        renderer2 myRenderer2;

        if (strcmp(argv[1], "-s") == 0) {
            myRenderer2.general_render();
        } else if (strcmp(argv[1], "-m") == 0) {
            myRenderer2.parallel_render();
        }
        else {
            std::cout << "Incorrect arguments.\n";
            std::cout << "-m for multi-threaded render\n";
            std::cout << "-s for single-threaded render\n";
        }
    }

    return 0;

}