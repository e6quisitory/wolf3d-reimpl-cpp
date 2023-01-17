/*
 * multimedia_manager.h:
 *
 * Manages SDL2 (init, quit, window, renderer) as well as loads game assets (texture/sprite BMP sheets) into GameData->Multimedia.
 *
 */

#pragma once

#include <string>
#include <cassert>
#include <SDL2/SDL.h>

#include "global.h"
#include "game_data.h"

class multimedia_manager {
public:
    void init(game_data* _game_data) {
        GameData = _game_data;

        // Initialize SDL globally
        SDL_Init(SDL_INIT_EVERYTHING);

        // Extract refresh rate of display
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        GameData->Multimedia.refresh_rate = dm.refresh_rate;
    }

    void exit() const {
        // Free all textures from memory
        for (int i = 0; i < 110; ++i)
            SDL_DestroyTexture(GameData->Multimedia.textures[i]);

        // Free all SDL-related memory
        SDL_DestroyRenderer(GameData->Multimedia.sdl_renderer);
        SDL_DestroyWindow(GameData->Multimedia.sdl_window);

        // Exit SDL globally
        SDL_Quit();
    }

    void create_window_and_renderer(int screen_width, int screen_height) const {
        assert(screen_width % 2 == 0 && screen_height % 2 == 0);
        GameData->Multimedia.screen_width = screen_width;
        GameData->Multimedia.screen_height = screen_height;
        GameData->Multimedia.sdl_window = SDL_CreateWindow("Wolfenstein 3D Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        GameData->Multimedia.sdl_renderer = SDL_CreateRenderer(GameData->Multimedia.sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC is important
        SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );
        SDL_SetWindowMouseGrab(GameData->Multimedia.sdl_window, SDL_TRUE);
        SDL_ShowCursor(SDL_DISABLE);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    // Loads wall textures (pair of lit + unlit texture) into GameData->Multimedia
    void load_wall_texture_pairs(const char* filename, int texture_sheet_pitch) const {
        // First load texture sheet BMP file into an SDL_Surface
        SDL_Surface* texture_sheet = bmp_to_surface(filename);

        // Then extract all textures from texture sheet and store in array
        for (int texture_id = 1; texture_id <= 110; ++texture_id)
            GameData->Multimedia.set_texture(texture_id, extract_texture(texture_sheet, texture_sheet_pitch, texture_id));

        // Free texture sheet SDL_Surface, as all textures from it have been extracted and stored as SDL_Textures
        SDL_FreeSurface(texture_sheet);
    }

private:
    // Converts a BMP file into an SDL_Surface*
    SDL_Surface* bmp_to_surface(const char* filename) const {
        SDL_Surface* temp = SDL_LoadBMP(filename);
        SDL_Surface* final = SDL_ConvertSurface(temp, SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 0);
        SDL_FreeSurface(temp);
        return final;
    }

    // Extracts a single texture from a texture sheet passed in as an SDL_Surface*
    SDL_Texture* extract_texture(SDL_Surface* texture_sheet, const int& texture_sheet_pitch, int texture_id) const {
        int topleft_x = ( (texture_id-1) % texture_sheet_pitch ) * TEXTURE_PITCH;
        int topleft_y = ( (texture_id-1) / texture_sheet_pitch ) * TEXTURE_PITCH;

        SDL_Rect src_rect = {topleft_x, topleft_y, TEXTURE_PITCH, TEXTURE_PITCH};

        // Allocate temporary SDL_Surface to store extracted texture in
        SDL_Surface* temp = SDL_CreateRGBSurfaceWithFormat(0, TEXTURE_PITCH, TEXTURE_PITCH, 32, SDL_PIXELFORMAT_ARGB8888);
        SDL_BlitSurface(texture_sheet, &src_rect, temp, nullptr);

        // Convert temporary SDL_Surface into an SDL_Texture
        SDL_Texture* extracted_texture = SDL_CreateTextureFromSurface(GameData->Multimedia.sdl_renderer, temp);

        // Free temporary SDL_Surface
        SDL_FreeSurface(temp);

        // Return extracted texture as SDL_Texture object pointer
        return extracted_texture;
    }

private:
    game_data* GameData;
};