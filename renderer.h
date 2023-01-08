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
    void init(game_data* gm_dat) {
        GameData = gm_dat;

        // Pre-calculate the ray angles and their cosines, as they do not change
        double proj_plane_width = 2*std::tan(degrees_to_radians(fov/2));
        double segment_len = proj_plane_width / GameData->Multimedia.screen_width;

        casting_ray_angles.reserve(GameData->Multimedia.screen_width);
        for (int i = 0; i < GameData->Multimedia.screen_width; ++i) {
            double angle = std::atan(-(i*segment_len-(proj_plane_width/2)));
            casting_ray_angles.push_back({angle, std::cos(angle)});
        }
    }

    void render_frame() {
        SDL_RenderClear(GameData->Multimedia.sdl_renderer);
        draw_ceiling_floor();

        for (int i = 0; i < GameData->Multimedia.screen_width; ++i) {
            ray curr_ray = ray(GameData->Player.location, GameData->Player.view_dir.rotate(casting_ray_angles[i].first));
            intersection curr_inter(curr_ray, curr_ray.origin);

            while (GameData->Map.within_map<ipoint2>(curr_inter.iPoint)) {
                TILE_TYPE prev_tile_type = GameData->Map(curr_inter.iPoint)->type();
                curr_inter = next_intersection(curr_inter);
                texture_hit_info hit = GameData->Map(curr_inter.iPoint)->hit(curr_inter, prev_tile_type);

                if (hit.hit == true) {
                    SDL_Rect src_rect = hit.rect;
                    int render_height = get_render_height(hit.distance, casting_ray_angles[i].second);
                    SDL_Rect draw_rect = {i, GameData->Multimedia.screen_height/2 - render_height/2, 1,
                                          render_height};
                    SDL_RenderCopy(GameData->Multimedia.sdl_renderer, hit.texture, &src_rect, &draw_rect);
                    break;
                } else
                    continue;
            }
        }
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

    int get_render_height(const double& hit_dist, const double& angle_cosine) {
        static double coeff = 1.3*GameData->Multimedia.screen_width / (16.0/9.0);  // must account for screen aspect ratio
        return static_cast<int>(coeff/(hit_dist*angle_cosine));
    }

private:
    game_data* GameData;

    std::vector<std::pair<double, double>> casting_ray_angles;  // First element in each pair is the angle, second is the cosine of it

    double fov = 72.0;
};