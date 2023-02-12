#include "MultimediaManager.h"

#include <cassert>

#include "../../Utilities/Conventions.h"

/*
================================
Public Methods
================================
*/

void MultimediaManager::Init(Multimedia* const _multimedia) {
    multimedia = _multimedia;
    SDL_Init(SDL_INIT_EVERYTHING);
}

void MultimediaManager::Exit() const {
    // Destroy all stored SDL_Textures out of memory
    for (const auto& [textureType, textureVec] : multimedia->textures) {
        for (SDL_Texture* const t : textureVec)
            SDL_DestroyTexture(t);
    }

    // Free all SDL-related memory
    SDL_DestroyRenderer(multimedia->sdlRenderer);
    SDL_DestroyWindow(multimedia->sdlWindow);

    // Exit SDL globally
    SDL_Quit();
}

void MultimediaManager::CreateWindowRenderer(const int screenWidth, const int screenHeight) const {
    // Make sure screen_width and screen_height are even numbers
    assert(screenWidth % 2 == 0 && screenHeight % 2 == 0);

    multimedia->sdlWindow   = SDL_CreateWindow("Wolfenstein 3D Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    multimedia->sdlRenderer = SDL_CreateRenderer(multimedia->sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC is important
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    // Set WindowParams
    multimedia->windowParams.screenHeight = screenHeight;
    multimedia->windowParams.screenWidth = screenWidth;
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    multimedia->windowParams.refreshRate = displayMode.refresh_rate;

    // Lock mouse to within window and hide pointer
    SDL_SetWindowMouseGrab(multimedia->sdlWindow, SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void MultimediaManager::LoadTextures(const textureType_t textureType, const spriteSheetParams_t spriteSheetParams) const {
    SDL_Surface* textureSheet = BmpToSurface(spriteSheetParams.fileName.c_str());

    // Must take out transparency pixels from sprite sheets
    if (textureType != textureType_t::WALLS)
        SDL_SetColorKey(textureSheet, SDL_TRUE, TRANSPARENCY_COLOR);

    // Then extract all textures from texture sheet and store in array
    for (int textureID = 1; textureID <= spriteSheetParams.numTextures; ++textureID) {
        SDL_Texture* extracted = ExtractTexture(textureSheet, spriteSheetParams.pitch, textureID);
        multimedia->AddTexture(textureType, extracted);
    }

    // Free texture sheet SDL_Surface, as all textures from it have been extracted and stored as SDL_Textures
    SDL_FreeSurface(textureSheet);
}

/*
================================
    Private Methods
================================
*/

SDL_Surface* MultimediaManager::BmpToSurface(const char* const fileName) const {
    SDL_Surface* bmpSurface          = SDL_LoadBMP(fileName);
    SDL_Surface* bmpSurface_ARGB8888 = SDL_ConvertSurface(bmpSurface, SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 0);
    SDL_FreeSurface(bmpSurface);
    return bmpSurface_ARGB8888;
}

SDL_Texture* MultimediaManager::ExtractTexture(SDL_Surface* const textureSheet, const int textureSheetPitch, const int textureID) const {
    int topleftPixel_x = ((textureID - 1) % textureSheetPitch ) * TEXTURE_PITCH;
    int topleftPixel_y = ((textureID - 1) / textureSheetPitch ) * TEXTURE_PITCH;
    SDL_Rect textureRect = {topleftPixel_x, topleftPixel_y, TEXTURE_PITCH, TEXTURE_PITCH};

    // Allocate temporary SDL_Surface and store extracted texture in it
    SDL_Surface* extractedTextureSurface = SDL_CreateRGBSurfaceWithFormat(0, TEXTURE_PITCH, TEXTURE_PITCH, 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_BlitSurface(textureSheet, &textureRect, extractedTextureSurface, nullptr);

    // Convert temporary SDL_Surface into an SDL_Texture
    SDL_Texture* extractedTexture = SDL_CreateTextureFromSurface(multimedia->sdlRenderer, extractedTextureSurface);

    // Free temporary SDL_Surface
    SDL_FreeSurface(extractedTextureSurface);

    // Return extracted texture as SDL_Texture object pointer
    return extractedTexture;
}
