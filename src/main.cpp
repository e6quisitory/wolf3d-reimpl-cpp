#include "GameEngine/GameEngine.h"

#if _WIN64

    #include "Windows.h"

    INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow){

#elif __APPLE__ && __MACH__

    int main() {
        
#else
    #error "Unknown platform"
#endif

    GameEngine gameEngine;
    gameEngine.Init();

    while (gameEngine.quitGameFlag == false)
        gameEngine.GameLoop();

    gameEngine.Exit();
     
    return 0;
    
}