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
#include "camera.h"
#include "misc.h"

using namespace std::chrono_literals;

int main() {

    // Create world map from txt file
    map* world_map = new map("map.txt");

    // Create player camera
    camera cam(vec2(8.2,8.2), vec2(-0.6,-0.5), 72.0, world_map);

    // Create window and renderer
    int image_width = 1280;
    int image_height = 720;
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window* window = SDL_CreateWindow( "Wolfenstein 3D Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image_width, image_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC is important
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );  // use linear filtering for scaling

    // Create texture
    SDL_Texture* texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);

    // Main SDL window loop
    bool running = true;
    bool render = true;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    while (running) {
        // SDL event handling
        SDL_Event e;
        while(SDL_PollEvent(&e)) { // return 1 if there is a pending event, otherwise 0 (loop doesn't run)
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        // Check for key presses every 60ms. Delay is necessary to prevent loop from going at full speed and draining CPU when no rendering is taking place (idle).
        if (render == false) {
            std::this_thread::sleep_for(60ms);
        }

        double speed = 0.15;

        if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_D]) {
            render = cam.move_y(0.3*0.7071067*speed);
            render = cam.move_x(0.3*0.7071067*speed);
        } else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A]) {
            render = cam.move_y(0.3*0.7071067*speed);
            render = cam.move_x(-0.3*0.7071067*speed);
        } else if(state[SDL_SCANCODE_W])
            render = cam.move_y(0.3*speed);
        else if (state[SDL_SCANCODE_S])
            render = cam.move_y(-0.3*speed);
        else if (state[SDL_SCANCODE_A])
            render = cam.move_x(-0.3*speed);
        else if (state[SDL_SCANCODE_D])
            render = cam.move_x(0.3*speed);

        if (state[SDL_SCANCODE_RIGHT])
            render = cam.swivel((-pi/20)*speed);
        else if (state[SDL_SCANCODE_LEFT])
            render = cam.swivel((pi/20)*speed);

        if (render == true) {
            Uint32 *locked_pixels = nullptr;
            int pitch = image_width * 4;
            SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&locked_pixels), &pitch);

            for (int i = 0; i < image_width; ++i) {
                ray curr_ray = cam.get_ray(static_cast<double>(i) / image_width);
                hit_info ray_hit = world_map->hit(curr_ray);
                if (ray_hit.hit == true) {
                    // Draw sky
                    for (int k = 0; k < image_height/2; ++k)
                        locked_pixels[k * image_width + i] = 0xFF87CEEB;

                    // Dray floor
                    for (int k = image_height/2; k < image_height; ++k)
                        locked_pixels[k * image_width + i] = 0xFF555555;

                    // Draw boxes
                    int render_height = static_cast<int>(400 / (ray_hit.dist*std::cos(cam.fov/2)));

                    int start_height = (image_height/2 - render_height/2);
                    int end_height = (image_height/2 + render_height/2);

                    if (start_height < 0) start_height = 0;
                    if (end_height > image_height) end_height = image_height;

                    for (int j = start_height; j < end_height; ++j) {
                        switch(ray_hit.wall_type) {
                            case 'v':
                                locked_pixels[j * image_width + i] = 0xFFDCDCDC;
                                break;
                            case 'h':
                                locked_pixels[j * image_width + i] = 0xFFAAAAAA;
                                break;
                            case 'c':
                                locked_pixels[j * image_width + i] = 0xFFFFFFFF;
                                break;
                        }
                    }

                }
            }

            SDL_UnlockTexture(texture);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(sdl_renderer);

            if (!(state[SDL_SCANCODE_W] || state[SDL_SCANCODE_A] || state[SDL_SCANCODE_S] || state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_LEFT]))
                render = false;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete world_map;

    return 0;
}
