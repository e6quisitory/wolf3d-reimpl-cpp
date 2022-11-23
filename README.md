# Wolfenstein 3D Clone

<p align="center"><b><i>
🚧🚧  Work in Progress  🚧🚧
</i></b></p>

I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning most of the functionality in the game is the goal, including enemies, weapons, sound system, etc. Let's see how far I get. So far, I've implemented the following functionality:
- Raycasting
- Selectively textured walls
- Doors w/ automatic closing
- Map loading from a `csv` file
- Colored floor and ceiling
- Smooth movement using VSYNC
- Multi-core rendering (for getting high fps on older processors)

Here is a short video of what the engine is currently capable of:

<div align="center">
  <video src="https://user-images.githubusercontent.com/25702188/202889868-aaf94b6c-2f61-49ab-953b-47d9a08fc103.mp4" width=852/>
</div>

The comments in the commits are full of detailed explanations on the implementation of parts of the engine, how I encountered and fixed bugs, etc. Worth looking at if you're building something like this yourself.

# Update - Nov. 23, 2022
It's been a few days since my last commit. The latest milestone I've achieved is getting doors to work properly, which I was quite proud of. However, after moving on to thinking of how I'd implement further new features, like enemies, weapons, shooting, health, etc. (featues to turn this thing from just a renderer into an actual game lol), it quickly became apparent that, if I don't want to turn this project into a huge mess of spagetti code with if statements everywhere, I'm going to need to take a break from writing code and actually plan out my game engine architecture.

It didn't become apparent to me until now that there is a reason there are entire books written on this subject (currently I'm reading _Game Engine Architecture_ by Jason Gregory). Implementing features is rather straightforward if you're fine with blasting if statements everywhere and filling your code with random flags and timers that you have to somehow keep track of, but if you don't want to do that and instead actually want to come back to your code after a few weeks and still know how it works and how you could add new stuff to it, then it requires some high level planning on a whiteboard, in an effort to give your complex system a rigid structure.

And so, in these last few days and currently, that's what I've been doing. Once I have planned out my engine, I'll start rewriting it according to the plan. I'll also post the flowchart/block diagram of it in this repo when I'm done. Of course, a lot of the current code will stay, it's just that (hopefully) it'll be structured better. Also, I learned yesterday that the SDL2 renderer can actually do hardware-accelerated texture scaling/mapping (currently I'm doing it manually in software), so I also plan on changing my code to use that capability.

## Build Instructions
The game engine is coded completely from scratch. The only dependancy this project has is the windowing and multimedia library, SDL2, to draw pixels onto the screen. Suffice to say, coding a windowing library from scratch is beyond my scope of knoweldge at the moment!

I'm doing this project on a 2020 M1 MacBook Air, and have installed SDL2 using Homebrew:
```
brew install sdl2
```
I'm compiling and running like so:
```
g++ -std=c++17 -lSDL2 main.cpp; ./a.out
```
## Controls & Map Loading
`w`, `a`, `s`, `d` for moving, `←` and `→` arrow keys for changing looking direction, `spacebar` for opening doors.

The map file is `map.csv` and is ASCII encoded, not UTF-8 encoded. I find that editing the file is easiest in Excel.

As for how to construct the map, i.e. what the values in the `csv` file mean, please read [this](https://github.com/e6quisitory/wolf3d-clone/commit/3f9f9e308629098a29df2e3ec8103fddd3fc51c0#commitcomment-88618626).

## Immediate Goals
- Sprites
- Darken distant walls (depth effect)
- Textured ceiling & floor
- Add inertia to movement (like in modern FPS games)
- Use mouse to look around
- If vertical wall (y-axis), use lit texture, if horizontal, use unlit.

## Credits
All textures used are the official Wolfenstein 3D textures that shipped in the game back in 1992. I found them [here](https://www.textures-resource.com/pc_computer/wolf3d/texture/1375/).

## Resources Used
- _Game Engine Blackbook - Wolfenstein 3D_ by Fabien Sanglard
- [Wolfenstein 3D's map renderer](https://www.youtube.com/watch?v=eOCQfxRQ2pY) (video)
- [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA) (video)
- [Make Your Own Raycaster Series](https://www.youtube.com/watch?v=gYRrGTC7GtA) (video)
