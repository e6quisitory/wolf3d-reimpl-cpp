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
#include "inputs.h"
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
        
        // Destroy all SDL_Textures out of memory
        for (int texture_type = 0; texture_type < NUM_TEXTURE_TYPES; ++texture_type) {
            auto texture_vec = GameData->Multimedia.textures[static_cast<TEXTURE_TYPE>(texture_type)];
            if (!texture_vec.empty()) {
                for (SDL_Texture* texture : texture_vec)
                    SDL_DestroyTexture(texture);
            }
        }

        // Free all SDL-related memory
        SDL_DestroyRenderer(GameData->Multimedia.sdl_renderer);
        SDL_DestroyWindow(GameData->Multimedia.sdl_window);

        // Exit SDL globally
        SDL_Quit();
        
    }

    void create_window_and_renderer(int screen_width, int screen_height) const {
        // Make sure screen_width and screen_height are even numbers
        assert(screen_width % 2 == 0 && screen_height % 2 == 0);
        
        GameData->Multimedia.screen_width = screen_width;
        GameData->Multimedia.screen_height = screen_height;
        
        GameData->Multimedia.sdl_window = SDL_CreateWindow("Wolfenstein 3D Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        GameData->Multimedia.sdl_renderer = SDL_CreateRenderer(GameData->Multimedia.sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC is important
        
        SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );
        
        // Lock mouse to within window and hike pointer
        SDL_SetWindowMouseGrab(GameData->Multimedia.sdl_window, SDL_TRUE);
        SDL_ShowCursor(SDL_DISABLE);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    // Loads wall textures (pair of lit + unlit texture) into GameData->Multimedia
    void load_textures(const TEXTURE_TYPE& _texture_type, const char* filename, const int& texture_sheet_pitch, const int& num_textures) const {
        // First load texture sheet BMP file into an SDL_Surface
        SDL_Surface* texture_sheet = bmp_to_surface(filename);
                
        // Must take out transparency pixels from sprite sheets
        if (_texture_type != WALLS)
            SDL_SetColorKey(texture_sheet, SDL_TRUE, 0xFF980088);
        
        // Then extract all textures from texture sheet and store in array
        for (int texture_id = 1; texture_id <= num_textures; ++texture_id) {
            SDL_Texture* t = extract_texture(texture_sheet, texture_sheet_pitch, texture_id);
            GameData->Multimedia.add_texture(_texture_type, t);
        }
            
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
