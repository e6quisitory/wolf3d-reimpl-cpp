#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <chrono>

#include "vec2.h"
#include "ray.h"
#include "map.h"
#include "player.h"
#include "misc.h"

using namespace std::chrono_literals;

class renderer {
public:
    renderer(int width, int height, std::string map_file, vec2 spawn_location, vec2 looking_dir): screen_width(width), screen_height(height) {
        world_map = new map(map_file);
        plyr = new player(spawn_location, looking_dir, 72.0, screen_width, world_map);
    }

    ~renderer() {
        delete plyr;
        delete world_map;
    }
    
    void render_game();

private:
    map* world_map;
    player* plyr;
    int screen_width;
    int screen_height;
    Uint8* screen_pixels;
};

void renderer::render_game() {
   
    // Create window and renderer
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window* window = SDL_CreateWindow( "Wolfenstein 3D Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC is important
    SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );

    // Create texture
    SDL_Texture* texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);

    // Create surface out of bitmap texture sheet
    SDL_Surface* bmp = bmp_to_surface("texture_sheet.bmp");
    int num_textures_x = 6;
    int num_textures_y = 19;
    int texture_width = 64;
    int texture_height = 64;
    Uint32* bmp_pixels = static_cast<Uint32*>(bmp->pixels);

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
            render = plyr->move_y(0.3*0.7071067*speed);
            render = plyr->move_x(0.3*0.7071067*speed);
        } else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A]) {
            render = plyr->move_y(0.3*0.7071067*speed);
            render = plyr->move_x(-0.3*0.7071067*speed);
        } else if(state[SDL_SCANCODE_W]) {
            render = plyr->move_y(0.3*speed);
        }
        else if (state[SDL_SCANCODE_S]) {
            render = plyr->move_y(-0.3*speed);
        }
        else if (state[SDL_SCANCODE_A])
            render = plyr->move_x(-0.3*speed);
        else if (state[SDL_SCANCODE_D])
            render = plyr->move_x(0.3*speed);

        if (state[SDL_SCANCODE_RIGHT])
            render = plyr->swivel((-pi/20)*speed);
        else if (state[SDL_SCANCODE_LEFT])
            render = plyr->swivel((pi/20)*speed);

        if (render == true) {
            Uint32 *locked_pixels = nullptr;
            int pitch = screen_width * 4;
            SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&locked_pixels), &pitch);

            for (int i = 0; i < screen_width; ++i) {
                ray curr_ray = plyr->get_ray(i);
                hit_info ray_hit = world_map->hit(curr_ray);

                // Draw ceiling
                for (int k = 0; k < screen_height/2; ++k)
                    locked_pixels[k * screen_width + i] = 0xFF323232;

                // Draw floor
                for (int k = screen_height/2; k < screen_height; ++k)
                    locked_pixels[k * screen_width + i] = 0xFF606060;

                // Draw boxes/walls
                if (ray_hit.hit == true) {
                    int render_height = static_cast<int>(930 / (ray_hit.dist * curr_ray.cosine_of_angle));

                    // Window start and end pixels for walls to be rendered into
                    int start_height = (screen_height / 2 - render_height / 2);
                    int end_height = (screen_height / 2 + render_height / 2);

                    // If very close to wall (wall takes up entire screen), reset start and end pixels to be 0 and window size, respectively
                    if (start_height < 0) start_height = 0;
                    if (end_height > screen_height) end_height = screen_height;

                    // Going vertically down, write pixels from bitmap texture onto screen.
                    for (int j = start_height; j < end_height; ++j) {
                        // Current horizontal and vertical point on each box, represented as a percent
                        double u = ray_hit.width_percent;
                        double v;

                        if (render_height < screen_height)
                            v = (j-start_height)/(double)render_height;
                            // If very close to wall, below formula must be used to avoid texture getting stretched
                        else
                            v = (((render_height-screen_height)/2)+j)/(double)render_height;

                        // Calculate bitmap texture file pixel indices
                        int u_i = static_cast<int>(u*(double)texture_width);
                        int v_i = static_cast<int>(v*(double)texture_height);

                        int texture_id = ray_hit.texture_id;
                        int texture_num_x = (texture_id % num_textures_x)-1;
                        int texture_num_y = (texture_id / num_textures_x);
                        int texture_start_y = texture_num_y*texture_height;
                        int texture_start_x = texture_num_x*texture_width;

                        get_pixel_at(locked_pixels, screen_width, i, j) = get_pixel_at(bmp_pixels, bmp->w, texture_start_x+u_i, texture_start_y+v_i);

                        /*switch (ray_hit.wall_type) {
                            case 'v':
                                locked_pixels[j * screen_width + i] = 0xFFDCDCDC;
                                break;
                            case 'h':
                                locked_pixels[j * screen_width + i] = 0xFFAAAAAA;
                                break;
                            case 'c':
                                locked_pixels[j * screen_width + i] = 0xFFAAAAAA;
                                break;
                        }*/

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

    SDL_FreeSurface(bmp);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}