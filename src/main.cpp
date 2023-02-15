#include "GameEngine/GameEngine.h"

#if _WIN64

    #include "Windows.h"

    INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow){

#elif __APPLE__ && __MACH__

    int main(int argc, char** argv) {
        
#else
    #error "Unknown platform"
#endif

    GameEngine gameEngine;
    gameEngine.Init(argv[0]);

    while (gameEngine.quitGameFlag == false)
        gameEngine.GameLoop();

    gameEngine.Exit();
     
    return 0;
    
}