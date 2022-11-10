#pragma once

#include <SDL2/SDL.h>

#include "map.h"
#include "player.h"

class updater {
public:
    updater(map* const wm, player* const p, double* const fps):
        world_map(wm), plyr(p), last_fps(fps) {}

    updater() {}

    bool key_down() {
        return keyboard_state[SDL_SCANCODE_W]     ||
               keyboard_state[SDL_SCANCODE_A]     ||
               keyboard_state[SDL_SCANCODE_S]     ||
               keyboard_state[SDL_SCANCODE_D]     ||
               keyboard_state[SDL_SCANCODE_RIGHT] ||
               keyboard_state[SDL_SCANCODE_LEFT];
    }
    
    void update_player(bool* const render_flag) {
        double mov_speed = 3.645/(*last_fps);
        if (keyboard_state[SDL_SCANCODE_W] && keyboard_state[SDL_SCANCODE_D]) {
            *render_flag = plyr->move_y(0.7071067*mov_speed);
            *render_flag = plyr->move_x(0.7071067*mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_W] && keyboard_state[SDL_SCANCODE_A]) {
            *render_flag = plyr->move_y(0.7071067*mov_speed);
            *render_flag = plyr->move_x(-0.7071067*mov_speed);
        } else if(keyboard_state[SDL_SCANCODE_W]) {
            *render_flag = plyr->move_y(mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_S]) {
            *render_flag = plyr->move_y(-mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_A]) {
            *render_flag = plyr->move_x(-mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_D]) {
            *render_flag = plyr->move_x(mov_speed);
        }

        double swvl_speed = 1.9089/(*last_fps);
        if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            *render_flag = plyr->swivel(-swvl_speed);
        } else if (keyboard_state[SDL_SCANCODE_LEFT]) {
            *render_flag = plyr->swivel(swvl_speed);
        }
    }

private:
    map* world_map;
    player* plyr;
    double* last_fps;
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);;
};
