# Wolfenstein 3D Clone

<p align="center"><b><i>
🚧🚧  Work in Progress  🚧🚧
</i></b></p>

I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning most of the functionality in the game is the goal, including enemies, weapons, sound system, etc. So far, I've implemented the following functionality:
- Raycasting
- Selectively textured walls with lighting
- Sprites
- GPU accelerated texture mapping
- Doors w/ automatic closing
- Map loading from a `csv` file
- Smooth movement using VSYNC
- Mouse to look around

Here is a short video of what the engine is currently capable of:

<div align="center">
  <video src="https://user-images.githubusercontent.com/25702188/213652164-ee0d2622-4c79-4e63-87d3-8f52f17b8080.mp4" width=852/>
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
cd src; g++ -std=c++17 main.cpp Utilities/DDA/DDA.cpp Utilities/MiscMath/MiscMath.cpp Utilities/Ray/Ray.cpp Managers/DoorManager/DoorManager.cpp Managers/MapManager/MapManager.cpp Managers/MultimediaManager/MultimediaManager.cpp Managers/PlayerManager/PlayerManager.cpp State/GameData/Map/Map.cpp State/GameData/Multimedia/Multimedia.cpp -o wolf3d-clone -lSDL2 -Wno-switch; ./wolf3d-clone; rm wolf3d-clone; cd ..
```
(yes i know this is hideous; I will write a proper Cmake script soon...)

## Controls & Map Loading
`W`, `A`, `S`, `D` to move, mouse to look around, `spacebar` for opening doors, `Esc` to exit game.

The map file is `map.csv` and is ASCII encoded, not UTF-8 encoded. I find that editing the file is easiest in Excel.

As for how to construct the map, i.e. what the values in the `csv` file mean, I have yet to type up a new guide; I will get to that soon. In the mean time, you can read [this](https://github.com/e6quisitory/wolf3d-clone/commit/3f9f9e308629098a29df2e3ec8103fddd3fc51c0#commitcomment-88618626) old guide.

## Immediate Goals
- Project restructuring (proper headers + implementation files)
- Precompiled header
- Refactoring according to [idTech4 style guidelines](https://github.com/e6quisitory/wolf3d-clone/files/10593839/CodeStyleConventions.pdf)
- Shifting to Cmake instead of compiling from terminal
- Character animation
- Weapons
- Enemy AI

## Later Goals
- Darken distant walls (shadow/depth effect)
- Textured ceiling & floor
- Add joystick support
- Port to browser using Webassembly
- Networked multiplayer _(aspirational)_
- Proper map editor _(aspirational)_
- Port to iOS?

## Credits
All textures used are the official Wolfenstein 3D textures that shipped in the game back in 1992. I found them [here](https://www.spriters-resource.com/pc_computer/wolfenstein3d/).

## Resources Used
- _Game Engine Blackbook - Wolfenstein 3D_ by Fabien Sanglard
- [Wolfenstein 3D's map renderer](https://www.youtube.com/watch?v=eOCQfxRQ2pY) (video)
- [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA) (video)
- [Make Your Own Raycaster Series](https://www.youtube.com/watch?v=gYRrGTC7GtA) (video)
