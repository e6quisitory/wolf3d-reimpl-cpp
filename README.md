# Wolfenstein 3D Clone
(_**Work in Progress**_)

I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning the game in it's entirety, including enemies, weapons, sound system, etc., would be quite a challenge for me at the moment. I'll see how far I can get. So far, I've been able to get basic raycasting working, with the map being loaded from a file (`map.txt`):

<img src="https://user-images.githubusercontent.com/25702188/198394205-2c1cabd8-92a7-438c-9d0d-d3798f64c975.gif" width="600" />

## Build Instructions
I'm doing this project on a 2020 M1 MacBook Air, 8 GB RAM, running MacOS Ventura.

SDL2 must be installed, which I've installed using Homebrew. To make sure that SDL2 get's picked up by the compiler, I've added the following lines in my `~/.zshrc` file:
```
export CPATH=/opt/homebrew/include
export LIBRARY_PATH=/opt/homebrew/lib
```
I'm compiling using g++ like so:
```
g++ -std=c++17 -lSDL2 main.cpp
```
And running like so:
```
./a.out
```
## Immediate Goals
- Textured walls
- Darken distant walls
- Make a "floor"

## Resources Used
- [Wolfenstein 3D's map renderer](https://www.youtube.com/watch?v=eOCQfxRQ2pY) (video)
- [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA) (video)
