#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <thread>
#include <chrono>

#include "vec2.h"
#include "ray.h"
#include "map.h"
#include "player.h"
#include "misc.h"
#include "renderer.h"
#include "updater.h"

using namespace std::chrono_literals;

class game_engine {
public:
    game_engine(): world_map(nullptr), plyr(nullptr), r(nullptr), upd(nullptr) {
        render_flag = new bool(true);
    }

    ~game_engine() {
        delete world_map;
        delete plyr;
        delete r;
        delete upd;
        delete render_flag;
    }

    void create_map(std::string map_file) {
        world_map = new map(map_file);
    }

    void create_player(vec2 spawn_location, vec2 looking_dir, double FOV) {
        plyr = new player(spawn_location, looking_dir, FOV, world_map);
    }

    void create_renderer(int width, int height) {
        assert(world_map != nullptr && plyr != nullptr);
        assert(width % std::thread::hardware_concurrency() == 0);
        r = new renderer(width, height, world_map, plyr);
        plyr->calculate_ray_angles(r->screen_width);
    }

    void create_updater() {
        assert(world_map != nullptr && plyr != nullptr && r != nullptr);
        upd = new updater(world_map, plyr, r->last_fps);
    }

    bool check_quit() {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return true;
            } else
                return false;
        }
        return false;
    }

    void game_loop() {

        upd->update_player(render_flag);

        if (*render_flag) {
            r->start_fps_measure();
            r->render_to_backbuffer();
            r->swap_buffers();
            r->update_fps();

            if (!upd->key_down() && !world_map->doors_opening()) {
                *render_flag = false;
                r->stop_fps_measure();
            }
        } else
            std::this_thread::sleep_for(10ms);

    }

private:
    map* world_map;
    player* plyr;
    renderer* r;
    updater* upd;
    bool* render_flag;
};