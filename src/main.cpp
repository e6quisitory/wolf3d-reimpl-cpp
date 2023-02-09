#include "GameEngine.h"

int main() {

    GameEngine gameEngine;
    gameEngine.Init();

    while (gameEngine.quitGameFlag == false)
        gameEngine.GameLoop();

    gameEngine.Exit();
     
    return 0;
    
}
