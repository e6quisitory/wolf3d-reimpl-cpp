#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

#include "vec2.h"
#include "ray.h"
#include "map.h"
#include "camera.h"
#include "misc.h"

int main() {

    // Create world map from txt file
    map world_map("map.txt");

    // Create player camera
    camera cam(vec2(8.2,8.2), vec2(-0.6,-0.5), 72.0, 1, world_map.width-1, 1, world_map.height-1);

    // Create window and renderer
    int image_width = 1280;
    int image_height = 720;
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window* window = SDL_CreateWindow( "Wolfenstein 3D Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image_width, image_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );  // use linear filtering for scaling

    // Create texture and allocate space in memory for image
    SDL_Texture* texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);

    // Main SDL window loop
    bool running = true;
    bool render = true;
    while (running) {

        // SDL event handling
        SDL_Event e;
        while(SDL_PollEvent(&e)) { // return 1 if there is a pending event, otherwise 0 (loop doesn't run)
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            } else if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_w:
                        render = cam.move_y(0.3);
                        break;
                    case SDLK_s:
                        render = cam.move_y(-0.3);
                        break;
                    case SDLK_a:
                        render = cam.move_x(-0.3);
                        break;
                    case SDLK_d:
                        render = cam.move_x(0.3);
                        break;
                    case SDLK_RIGHT:
                        render = cam.swivel(-pi/16);
                        break;
                    case SDLK_LEFT:
                        render = cam.swivel(pi/16);
                        break;
                }
            }
        }

        if (render == true) {
            Uint32 *locked_pixels = nullptr;
            int pitch = image_width * 4;
            SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&locked_pixels), &pitch);

            for (int i = 0; i < image_width; ++i) {
                ray curr_ray = cam.get_ray(static_cast<double>(i) / image_width);
                double dist = world_map.hit(curr_ray);
                if (dist != -1) {
                    int render_height = static_cast<int>(400 / (dist * std::cos(cam.fov / 2)));
                    //int render_height = static_cast<int>(300/dist);
                    int start_height =
                            (image_height / 2 - render_height / 2) >= 0 ? (image_height / 2 - render_height / 2) : 0;
                    int end_height = (image_height / 2 + render_height / 2) < image_height ? (image_height / 2 +
                                                                                              render_height / 2)
                                                                                           : image_height;
                    for (int j = start_height; j < end_height; ++j)
                        locked_pixels[j * image_width + i] = 0xFFFFFFFF;
                }
            }

            SDL_UnlockTexture(texture);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(sdl_renderer);
            render = false;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
