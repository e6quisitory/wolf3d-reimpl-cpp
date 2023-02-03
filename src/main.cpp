#include "GameEngine.h"

int main() {

    GameEngine gameEngine;
    gameEngine.Init();

    while (gameEngine.inputs->quit_flag == false)
        gameEngine.GameLoop();

    gameEngine.Exit();
     
    return 0;
    
}
