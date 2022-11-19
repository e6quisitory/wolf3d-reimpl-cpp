#pragma once

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

class renderer {
public:
    renderer(int width, int height, map* const wm, player* const p):
        screen_width(width),
        screen_height(height),
        world_map(wm),
        plyr(p) {

        // Allocate backbuffer
        backbuffer = new Uint32[screen_width*screen_height];

        // Allocate frame with just ceiling & floor
        ceiling_floor = new Uint32[screen_width*screen_height];
        generate_ceiling_floor(ceiling_floor);

        // Create window and renderer
        SDL_Init( SDL_INIT_EVERYTHING );
        window = SDL_CreateWindow( "Wolfenstein 3D Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC is important
        SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );

        // Create main screen texture
        texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);

        // Create surface out of bitmap texture sheet
        bmp = bmp_to_surface("texture_sheet.bmp");
        num_textures_x = 6;
        num_textures_y = 19;
        texture_width = 64;
        texture_height = 64;
        bmp_pixels = static_cast<Uint32*>(bmp->pixels);

        // FPS measurement related vars
        fps_in_progress = false;
        last_fps = new double(60.0);

        // Mulithreading stuff
        cores = std::thread::hardware_concurrency();
        threads = new std::thread[cores];

        width_per_thread = screen_width / cores;
        starting_indices = new int[cores];
        ending_indices = new int[cores];

        for (int i = 0; i < cores; ++i) {
            starting_indices[i] = i*width_per_thread;
            ending_indices[i] = starting_indices[i] + width_per_thread;
        }
    }

    renderer() {}

    ~renderer() {
        delete[] backbuffer;
        delete[] ceiling_floor;
        delete last_fps;
        delete[] threads;
        delete[] starting_indices;
        delete[] ending_indices;

        SDL_FreeSurface(bmp);
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void render_to_backbuffer() {

        // Draw ceiling and floor
        copy_frame(ceiling_floor, backbuffer);

        // Launch rendering threads to render partial sections of frame
        for (int i = 0; i < cores; ++i)
            threads[i] = std::thread(&renderer::render_partial_frame, this, starting_indices[i], ending_indices[i]);

        // Wait for all threads to finish rendering
        for (int i = 0; i < cores; ++i)
            threads[i].join();

    }

    void swap_buffers() {
        Uint32 *locked_pixels = nullptr;
        int pitch = screen_width * 4;
        SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&locked_pixels), &pitch);

        std::copy_n(backbuffer, screen_width*screen_height, locked_pixels);

        SDL_UnlockTexture(texture);
        SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer);
    }

    void start_fps_measure() {
        if (fps_in_progress == false) {
            fps_in_progress = true;
            num_frames = 0;
            frame_start_time = SDL_GetTicks();
        }
    }

    void stop_fps_measure() {
        fps_in_progress = false;
    }

    void update_fps() {
        if (num_frames < 30) {
            ++num_frames;
        }
        else {
            Uint32 ms_diff = SDL_GetTicks() - frame_start_time; if (ms_diff == 0) return;
            *last_fps = num_frames/(ms_diff/1000.0);
            SDL_SetWindowTitle(window, (std::string("Wolfenstein 3D Clone - FPS ") + double_to_string(*last_fps, 2)).c_str());
            stop_fps_measure();
        }
    }

private:
    /* Loads bitmap file into an SDL surface with pixel format ARGB8888, and returns pointer to said surface */
    SDL_Surface* bmp_to_surface(const char* filename) {
        SDL_Surface* temp = SDL_LoadBMP(filename);
        SDL_Surface* final = SDL_ConvertSurface(temp, SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 0);
        SDL_FreeSurface(temp);
        return final;
    }

    void generate_ceiling_floor(Uint32* const cf) {
        for (int i = 0; i < screen_width; ++i) {
            for (int k = 0; k < screen_height/2; ++k)
                cf[k * screen_width + i] = 0xFF323232;

            for (int k = screen_height/2; k < screen_height; ++k)
                cf[k * screen_width + i] = 0xFF606060;
        }
    }

    void copy_frame(Uint32* const src, Uint32* const dest) {
        std::copy_n(src, screen_width*screen_height, dest);
    }

    void render_partial_frame(int start, int end) {
        for (int i = start; i < end; ++i) {
            ray curr_ray = plyr->get_ray(i);
            hit_info ray_hit = world_map->hit(curr_ray);

            if (ray_hit.hit == true) {
                int render_height = static_cast<int>(1.3*screen_height / (ray_hit.dist * curr_ray.cosine_of_angle));

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

                    if (ray_hit.texture_id == 99) {
                        texture_start_x -= (world_map->doors_amount_open[ray_hit.arr_index]/100.0)*texture_width;
                        if (world_map->door_currently_opening(ray_hit.arr_index))
                            ++texture_start_x;
                    }

                    get_pixel_at(backbuffer, screen_width, i, j) = get_pixel_at(bmp_pixels, bmp->w, texture_start_x+u_i, texture_start_y+v_i);
                }
            }
        }
    }

    Uint32& get_pixel_at(Uint32* const pixels_array, int width, int x, int y) {
        return pixels_array[y*width + x];
    }

public:
    int screen_width;
    int screen_height;
    double* last_fps;

private:
    map* world_map;
    player* plyr;
    Uint32* ceiling_floor;
    Uint32* backbuffer;

    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* texture;
    SDL_Surface* bmp;
    Uint32* bmp_pixels;
    int num_textures_x;
    int num_textures_y;
    int texture_width;
    int texture_height;

    bool fps_in_progress;
    Uint32 frame_start_time;
    int num_frames;

    int cores;
    std::thread* threads;
    int width_per_thread;
    int* starting_indices;
    int* ending_indices;
};