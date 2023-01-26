#include "GameEngine.h"

int main() {

    GameEngine GameEngine;
    GameEngine.Init();

    while (GameEngine.inputs->quit_flag == false)
        GameEngine.GameLoop();

    GameEngine.Exit();
     
    return 0;
    
}
