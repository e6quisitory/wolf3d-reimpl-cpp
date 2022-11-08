#pragma once

#include <SDL2/SDL.h>

#include "map.h"
#include "player.h"

class updater {
public:
    updater(map* const wm, player* const p):
        world_map(wm), plyr(p) {}

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
        if (keyboard_state[SDL_SCANCODE_W] && keyboard_state[SDL_SCANCODE_D]) {
            *render_flag = plyr->move_y(0.3*0.7071067*speed);
            *render_flag = plyr->move_x(0.3*0.7071067*speed);
        } else if (keyboard_state[SDL_SCANCODE_W] && keyboard_state[SDL_SCANCODE_A]) {
            *render_flag = plyr->move_y(0.3*0.7071067*speed);
            *render_flag = plyr->move_x(-0.3*0.7071067*speed);
        } else if(keyboard_state[SDL_SCANCODE_W]) {
            *render_flag = plyr->move_y(0.3*speed);
        } else if (keyboard_state[SDL_SCANCODE_S]) {
            *render_flag = plyr->move_y(-0.3*speed);
        } else if (keyboard_state[SDL_SCANCODE_A]) {
            *render_flag = plyr->move_x(-0.3 * speed);
        } else if (keyboard_state[SDL_SCANCODE_D]) {
            *render_flag = plyr->move_x(0.3 * speed);
        }

        if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            *render_flag = plyr->swivel((-pi / 20) * speed);
        } else if (keyboard_state[SDL_SCANCODE_LEFT]) {
            *render_flag = plyr->swivel((pi / 20) * speed);
        }
    }

private:
    map* world_map;
    player* plyr;
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);;
    const double speed = 0.15;
};