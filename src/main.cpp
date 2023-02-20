#include "GameEngine/GameEngine.h"
#include "DevTools/Minimap/Minimap.h"

#if _WIN64
    #include "Windows.h"
    INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow){
#else
    int main(int argc, char** argv) {
#endif
    GameEngine gameEngine;
    
    #if __APPLE__ && __MACH__
    gameEngine.Init(argv[0]);
    #else
    gameEngine.Init();
    #endif

    Minimap minimap(gameEngine.worldState, gameEngine.multimedia, 20);

    while (gameEngine.quitGameFlag == false) {
        gameEngine.GameLoop();
        minimap.Update();
    }

    gameEngine.Exit();
     
    return 0;
}