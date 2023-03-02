#pragma once

#include "../../Multimedia/Multimedia.h"
#include "../../InputsBuffer/InputsBuffer.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

struct spriteSheetParams_t {
    std::string  fileName;
    int          pitch;
    int          numTextures;
};

typedef std::tuple<int, int, int> displayParams_t;

/*
=========================================================
    MultimediaManager
=========================================================
*/

class MultimediaManager {
private:
    Multimedia*    multimedia;
    InputsBuffer*  inputsBuffer;

public:
    void             Init                     (Multimedia* const _multimedia, InputsBuffer* const _inputsBuffer);
    void             Exit()                                                                                                       const;
    void             Update()                                                                                                     const;
    void             CreateWindowRenderer     (const int customScreenWidth, const int customScreenHeight)                         const;
    void             CreateWindowRenderer()                                                                                       const;
    void             LoadTextures             (const textureType_t textureType, const spriteSheetParams_t spriteSheetParams)      const;

private:
    void             CreateSdlWindowRenderer  (const int screenWidth, const int screenHeight, const bool fullScreen)              const;
    void             ToggleMouseLock()                                                                                            const;
    SDL_Surface*     BmpToSurface             (const char* const fileName)                                                        const;
    SDL_Texture*     ExtractTexture           (SDL_Surface* const textureSheet, const int textureSheetPitch, const int textureID) const;
};
