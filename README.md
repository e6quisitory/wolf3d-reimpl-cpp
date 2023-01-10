# Wolfenstein 3D Clone

<p align="center"><b><i>
🚧🚧  Work in Progress  🚧🚧
</i></b></p>

I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning most of the functionality in the game is the goal, including enemies, weapons, sound system, etc. So far, I've implemented the following functionality:
- Raycasting
- Selectively textured walls with lighting
- GPU accelerated texture mapping
- Doors w/ automatic closing
- Map loading from a `csv` file
- Colored floor and ceiling
- Smooth movement using VSYNC
- Mouse to look around

Here is a short video of what the engine is currently capable of:

<div align="center">
  <video src="https://user-images.githubusercontent.com/25702188/211391994-abf2d2df-4ea7-4be0-91c9-4591c9b027fa.mp4" width=852/>
</div>

The comments in the commits are full of detailed explanations on the implementation of parts of the engine, how I encountered and fixed bugs, etc. Worth looking at if you're building something like this yourself.

## Build Instructions
The game engine is coded completely from scratch. The only dependancy this project has is the windowing and multimedia library, SDL2, to draw pixels onto the screen as well as to do the texture scaling on the GPU. Suffice to say, coding a multimedia library from scratch is beyond my scope of knoweldge at the moment! So I'm using SDL2.

I'm doing this project on a 2020 M1 MacBook Air, and have installed SDL2 using Homebrew:
```
brew install sdl2
```
I'm compiling and running like so:
```
g++ -std=c++17 main.cpp -lSDL2; ./a.out
```
## Controls & Map Loading
`w`, `a`, `s`, `d` to move, mouse to look around, `spacebar` for opening doors, `Esc` to exit game.

The map file is `map.csv` and is ASCII encoded, not UTF-8 encoded. I find that editing the file is easiest in Excel.

As for how to construct the map, i.e. what the values in the `csv` file mean, please read [this](https://github.com/e6quisitory/wolf3d-clone/commit/3f9f9e308629098a29df2e3ec8103fddd3fc51c0#commitcomment-88618626).

## Immediate Goals
- A clean system of managing and accessing game assets 
- Sprites rendering
- Weapons
- Enemy AI & character animation

## Later Goals
- Darken distant walls (shadow/depth effect)
- Textured ceiling & floor
- Add joystick support
- Port to browser using Webassembly
- Networked multiplayer _(aspirational)_
- Proper map editor _(aspirational)_
- Port to iOS?

## Credits
All textures used are the official Wolfenstein 3D textures that shipped in the game back in 1992. I found them [here](https://www.textures-resource.com/pc_computer/wolf3d/texture/1375/).

## Resources Used
- _Game Engine Blackbook - Wolfenstein 3D_ by Fabien Sanglard
- [Wolfenstein 3D's map renderer](https://www.youtube.com/watch?v=eOCQfxRQ2pY) (video)
- [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA) (video)
- [Make Your Own Raycaster Series](https://www.youtube.com/watch?v=gYRrGTC7GtA) (video)
