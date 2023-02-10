# Wolfenstein 3D Clone

<p><i>🚧🚧  Work in Progress  🚧🚧</i></p>

I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning most of the functionality in the game from scratch is the goal, including rendering the game world, enemies, weapons, sound system, etc. So far, I've implemented the following functionality:
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

The only dependacy of the project thus far is SDL2, used to open a window and render pixels into it, hardware-based texture scaling, reading keyboard and mouse inputs, and VSYNC.

The comments in the commits are full of detailed explanations on the implementation of parts of the engine, how I encountered and fixed bugs, etc. Worth looking at if you're building something like this yourself.

## Build Instructions
The following instructions are for macOS. I will update the build procedure to accommodate other platforms soon.

Firstly, ensure `CMake`, `Ninja`, and `SDL2` are installed on your Mac through Homebrew:
```
brew install cmake
brew install ninja
brew install sdl2
```

Now, clone this repo and `cd` into it:
```
git clone https://github.com/e6quisitory/wolf3d-clone.git
cd wolf3d-clone
```

Then run the following to build:
```
mkdir build
cd build
cmake -G "Ninja" ../src
ninja
```

And run the executable like so:
```
./wolf3d-clone
```

## Controls & Map Loading
`W`, `A`, `S`, `D` to move, mouse to look around, `spacebar` for opening doors, `Esc` to exit game.

The map file is `map.csv` and is ASCII encoded, not UTF-8 encoded. I find that editing the file is easiest in Excel.

As for how to construct the map, i.e. what the values in the `csv` file mean, I have yet to type up a new guide; I will get to that soon. In the mean time, you can read [this](https://github.com/e6quisitory/wolf3d-clone/commit/3f9f9e308629098a29df2e3ec8103fddd3fc51c0#commitcomment-88618626) old guide.

## Immediate Goals
- Precompiled header
- Updating Cmake script to accommodate building on Windows and Linux
- Character/sprite animation
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
