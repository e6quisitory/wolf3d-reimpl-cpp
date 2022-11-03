#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

#include "vec2.h"
#include "ray.h"
#include "map.h"
#include "player.h"
#include "misc.h"
#include "renderer.h"

using namespace std::chrono_literals;

int main() {
    
    renderer r("map.csv", vec2(5.1,5.1), vec2(1,0));
    r.render_game(1280, 720);

    return 0;
}
