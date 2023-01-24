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

        sprite_backups.reserve(GameData->Multimedia.screen_width);
            // One sprite encountered per casted ray is a good initial assumption to allocate memory according to.
            // However, it's entirely possible to have a single casted ray encounter more than one sprite, in which case
            // sprite_backups vector may need to be extended.
    }

    // Renders a single frame and outputs it to the window/screen
    void render_frame() {

        SDL_RenderClear(GameData->Multimedia.sdl_renderer);

        draw_ceiling_floor();
        draw_walls();
        draw_sprites();
        
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

    void draw_walls() {
        for (int i = 0; i < GameData->Multimedia.screen_width; ++i) {
            ray curr_ray = get_ray(i);
            intersection curr_inter(curr_ray, curr_ray.origin);

            while (GameData->Map.within_map<ipoint2>(curr_inter.iPoint)) {
                TILE_TYPE prev_tile_type = GameData->Map.get_tile(curr_inter.iPoint)->type;
                curr_inter = next_intersection(curr_inter);
                ray_tile_hit_info tile_hit = GameData->Map.get_tile(curr_inter.iPoint)->ray_tile_hit(curr_inter,
                                                                                                     prev_tile_type);

                if (tile_hit.hit == true) {
                    SDL_Rect texture_rect = tile_hit.texture_rect;
                    int render_height = get_render_height(tile_hit.distance, casting_ray_angles[i].second);
                    SDL_Rect screen_rect = {i, GameData->Multimedia.screen_height / 2 - render_height / 2, 1,
                                            render_height};

                    // If sprite is hit, store the texture slice and rendering info, as we first need to render the walls + doors behind the sprites
                    if (GameData->Map.get_tile(curr_inter.iPoint)->type == SPRITE) {
                        sprite_backups.push_back(
                                texture_slice_render_info(tile_hit.texture, texture_rect, screen_rect));
                        continue;
                    } else {
                        //int color_mod = (tile_hit.distance*casting_ray_angles[i].second) < 2 ? 255 : 255*2/((tile_hit.distance*casting_ray_angles[i].second));
                        //SDL_SetTextureColorMod(tile_hit.texture, color_mod, color_mod, color_mod);
                        SDL_RenderCopy(GameData->Multimedia.sdl_renderer, tile_hit.texture, &texture_rect,
                                       &screen_rect);
                        //SDL_SetTextureColorMod(tile_hit.texture, 255, 255, 255);
                        break;
                    }
                } else
                    continue;
            }
        }
    }

    void draw_sprites() {
        if (!sprite_backups.empty()) {
            // Must render sprites in reverse of the order in which they were encountered
            for (auto s = sprite_backups.rbegin(); s != sprite_backups.rend(); ++s)
                SDL_RenderCopy(GameData->Multimedia.sdl_renderer, s->texture, &(s->texture_rect), &(s->screen_rect));
            sprite_backups.clear();
        }
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
    
    struct texture_slice_render_info {
        texture_slice_render_info(SDL_Texture* const _texture, const SDL_Rect& _texture_rect, const SDL_Rect& _screen_rect):
            texture(_texture), texture_rect(_texture_rect), screen_rect(_screen_rect) {}

        texture_slice_render_info() {};

        SDL_Texture* texture;
        SDL_Rect texture_rect;
        SDL_Rect screen_rect;
    };
    
    std::vector<texture_slice_render_info> sprite_backups;
};
