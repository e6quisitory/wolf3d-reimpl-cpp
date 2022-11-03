# Wolfenstein 3D Clone
(_**Work in Progress**_)

I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning the game in it's entirety, including enemies, weapons, sound system, etc., would be quite a challenge for me at the moment. I'll see how far I can get. So far, I've been able to get basic raycasting working with selectively textured walls and a colored ceiling & floor. The map is being loaded from a file (`map.csv`), ASCII encoded (I edit mine in Excel):

<p align="center">
<img src="https://user-images.githubusercontent.com/25702188/199422478-515fb05b-5025-4ac9-9e2f-fd50e8593e0e.gif" width="600" />
</p>

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
## Controls
`w`, `a`, `s`, `d` for moving, `←` and `→` arrow keys for changing looking direction.

## Immediate Goals
- Sprites
- Textured ceiling & floor
- Add inertia to movement (like in modern FPS games)
- Darken distant walls (fog effect)
- Implement doors
- Use mouse to look around

## Credits
All textures used are official Wolfenstein 3D textures. I found them [here](https://www.textures-resource.com/pc_computer/wolf3d/texture/1375/).

## Resources Used
- _Game Engine Blackbook - Wolfenstein 3D_ by Fabien Sanglard
- [Wolfenstein 3D's map renderer](https://www.youtube.com/watch?v=eOCQfxRQ2pY) (video)
- [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA) (video)
- [Make Your Own Raycaster Series](https://www.youtube.com/watch?v=gYRrGTC7GtA) (video)
