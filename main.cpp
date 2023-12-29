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


int main() {
    renderer2 myRenderer2;

    // myRenderer2.general_render();

    myRenderer2.parallel_render();

}