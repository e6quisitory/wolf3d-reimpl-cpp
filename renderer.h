/*
 * renderer.h:
 *
 * Uses game_data struct to turn the game world from a data representation into a graphical representation, to be outputted to the screen.
 *
 */

#pragma once

#include <cmath>
#include <vector>
#include <utility>

#include "ray.h"
#include "game_data.h"
#include "dda.h"
#include "global.h"

class renderer {
public:
    void init(game_data* _game_data) {
        GameData = _game_data;

        // Pre-calculate the ray angles and their cosines, as they do not change
        double proj_plane_width = 2*std::tan(degrees_to_radians(fov/2));
        double segment_len = proj_plane_width / GameData->Multimedia.screen_width;
        casting_ray_angles.reserve(GameData->Multimedia.screen_width);
        for (int i = 0; i < GameData->Multimedia.screen_width; ++i) {
            double angle = std::atan(-(i*segment_len-(proj_plane_width/2)));
            casting_ray_angles.push_back({angle, std::cos(angle)});
        }
    }

    // Renders a single frame and outputs it to the window/screen
    void render_frame() {
        vec2& view_dir = GameData->Player.view_dir;
        
        // Clear screen
        SDL_RenderClear(GameData->Multimedia.sdl_renderer);

        // Draw ceiling and floor (simple solid colors)
        draw_ceiling_floor();

        // Iterate over the width of the screen, casting out a ray for each vertical column of pixels
        for (int i = 0; i < GameData->Multimedia.screen_width; ++i) {
            // Get ray with origin of player location and direction corresponding to the current vertical column of pixels
            ray curr_ray = get_ray(i);
            intersection curr_inter(curr_ray, curr_ray.origin);

            // Go from intersection to intersection on the grid
            while (GameData->Map.within_map<ipoint2>(curr_inter.iPoint)) {
                // Keep track of previous tile type to pass into tile->ray_hit function (for door sidewall rendering)
                TILE_TYPE prev_tile_type = GameData->Map.get_tile(curr_inter.iPoint)->type();

                // Calculate next intersection of curr_ray with map grid
                curr_inter = next_intersection(curr_inter);

                // See if tile that is hit is opaque (non-empty)
                ray_tile_hit_info tile_hit = GameData->Map.get_tile(curr_inter.iPoint)->ray_tile_hit(curr_inter, prev_tile_type);

                // If non-empty, then we must render out a column of pixels on the screen, with the texture + SDL_Rect given by the tile
                if (tile_hit.hit == true) {

                    // Rect given by the tile for the texture
                    SDL_Rect texture_rect = tile_hit.texture_rect;

                    // How high we render the column of pixels on the screen depends on how far away the intersection is from the player + the cosine of the ray angle
                    int render_height = get_render_height(tile_hit.distance, casting_ray_angles[i].second);

                    // With calculated render height, we specify the section of the screen we'd like to render the column of pixels into
                    SDL_Rect screen_rect = {i, GameData->Multimedia.screen_height / 2 - render_height / 2, 1, render_height};

                    // SDL renders out the portion of the texture given by the tile into the portion of the screen specified by us.
                    // Crucially, this function handles the texture scaling for us, and it does those calculations on the GPU.
                    SDL_RenderCopy(GameData->Multimedia.sdl_renderer, tile_hit.texture, &texture_rect, &screen_rect);

                    // Since we've hit a non-empty block, we can break out of the loop
                    break;
                } else
                    // If the tile hit is empty (or ray intersects part of door that is currently open), then pass through and go on to next intersection
                    continue;
            }
        }
        // Refresh SDL renderer (so that new rendered frame shows up)
        SDL_RenderPresent(GameData->Multimedia.sdl_renderer);
    }

private:
    void draw_ceiling_floor() {
        SDL_Rect ceiling = {0, 0, GameData->Multimedia.screen_width, GameData->Multimedia.screen_height/2};
        SDL_Rect floor = {0, GameData->Multimedia.screen_height/2, GameData->Multimedia.screen_width, GameData->Multimedia.screen_height/2};

        SDL_SetRenderDrawColor( GameData->Multimedia.sdl_renderer, 50, 50, 50, 0 );
        SDL_RenderFillRect(GameData->Multimedia.sdl_renderer, &ceiling );

        SDL_SetRenderDrawColor( GameData->Multimedia.sdl_renderer, 96, 96, 96, 0 );
        SDL_RenderFillRect(GameData->Multimedia.sdl_renderer, &floor );
    }

    // Returns ray corresponding to a vertical column of pixels (0 is leftmost column of pixels)
    ray get_ray(const int& ray_num) {
        return ray(GameData->Player.location, GameData->Player.view_dir.rotate(casting_ray_angles[ray_num].first));
    }

    // Given hit distance and ray angle cosine, calculate how high the column of pixels to render should be
    int get_render_height(const double& hit_dist, const double& angle_cosine) {
        static double coeff = 1.3*GameData->Multimedia.screen_width / ((16.0/9.0) * (fov/72.0));  // must account for screen aspect ratio
        return static_cast<int>(coeff/(hit_dist*angle_cosine));
    }

private:
    game_data* GameData;

    double fov = 72.0;
    std::vector<std::pair<double, double>> casting_ray_angles;  // First element in each pair is the angle, second is the cosine of it
};
