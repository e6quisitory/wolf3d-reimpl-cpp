# Wolfenstein 3D Clone

<p><i><b>🚧🚧  Work in Progress  🚧🚧</b></i></p>

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
### Windows
The following instructions assume you're running on an x86-64 Windows machine.

1. Install MSYS2 from [here](https://www.msys2.org/).
2. When the install wizard is finished, it'll launch a terminal for the **UCRT64** environment. We do not want this; instead we'd like to work in a terminal for the **CLANG64** environment. So, in the Windows start menu search, type _MSYS2 CLANG64_ and a program of that name should pop up. Click it and another terminal will pop up.
3. Inside this terminal for the CLANG64 environment, we first update all pre-installed packages by running the command below. Enter **Y** (for _yes_) when prompted to allow installation. The terminal might close, and if it does, reopen from the start menu as before.
   
    ```
    pacman -Suy
    ```
4. Now we install the packages we need to clone this repo and compile it.

    ```
    pacman -S mingw-w64-clang-x86_64-clang
    pacman -S mingw-w64-clang-x86_64-cmake
    pacman -S mingw-w64-clang-x86_64-ninja
    pacman -S mingw-w64-clang-x86_64-SDL2
    pacman -S git
    ```
4. Clone this repo and `cd` into it.

    ```
    git clone https://github.com/e6quisitory/wolf3d-clone.git
    cd wolf3d-clone
    ```
5. Run the following to build the project into a `.exe` executable:

    ```
    mkdir build
    cd build
    cmake -G "Ninja" ../src
    ninja
    ```
6. Finally, run the executable like so:

    ```
    ./wolf3d-clone.exe
    ```
**Aside**

This compiled executable will be located in `C:\msys64\home\[YOUR USERNAME]\wolf3d-clone\build` (assuming you installed MSYS2 into `C:\msys64`). You can navigate to this location in Windows file explorer and you'll see `wolf3d-clone.exe` present there. However, if you double click, it'll complain about certain `.dll`'s being missing and wont run.
Thus, if you'd like to run the executable from _outside_ of the MSYS2 CLANG64 environment terminal (as in just double click on it and have it run), you have two options:

1. Copy `libc++.dll` and `SDL2.dll` located in `C:\msys64\clang64\bin`, and paste them next to the `wolf3d-clone.exe` executable. Now when you run it, it'll be able to access both of those `.dll`'s that it needs and will run properly.
2. Add the location of the needed `.dll`'s to your system PATH. Follow the steps below to achieve this:

    - Hit `Win` + `R` and type in `systempropertiesadvanced`, then hit the `Enter` key
    - Click `Environment Variables` at the bottom of the window that opens
    - In the bottom section (System Variables) look for a variable called `Path` and double-click on it
    - In the new window that opens, click `New` on the right side
    - Enter in `C:\msys64\clang64\bin`
    - Hit the `Enter` key, then `OK` out of all open windows

Note, the executable also requires access to the `assets` folder from this project, which holds the texture/sprite sheets, map, etc. So the directory structure should still at least be the following:

```
wolf3d-clone/
├── assets/
│   ├── wall_textures.bmp
│   ├── map.csv
│   ├── objects.bmp
│   └── ...
└── build/
    ├── wolf3d-clone.exe
    ├── SDL2.dll (if not made accessible through PATH)
    └── libc++.dll (if not made accessible through PATH)
```
### macOS
1. Ensure `CMake`, `Ninja`, and `SDL2` are installed on your Mac through [Homebrew](https://brew.sh/):

    ```
    brew install cmake
    brew install ninja
    brew install sdl2
    ```
2. Clone this repo and `cd` into it:

    ```
    git clone https://github.com/e6quisitory/wolf3d-clone.git
    cd wolf3d-clone
    ```
3. Run the following to build:

    ```
    mkdir build
    cd build
    cmake -G "Ninja" ../src
    ninja -j $(sysctl -n hw.physicalcpu)
    ```
4. Finally, run the executable like so:

    ```
    ./wolf3d-clone
    ```
## Controls & Map Loading
`W`, `A`, `S`, `D` to move, mouse to look around, `spacebar` for opening doors, `Esc` to exit game.

The map file is `map.csv`, located in the `assets` folder. It is ASCII encoded, not UTF-8 encoded. I find that editing the file is easiest in Excel.

As for how to construct the map, i.e. what the values in the `map.csv` file mean, I have yet to type up a new guide; I will get to that soon. In the mean time, you can read [this](https://github.com/e6quisitory/wolf3d-clone/commit/3f9f9e308629098a29df2e3ec8103fddd3fc51c0#commitcomment-88618626) old guide and look at the values in the included `map.csv` and try to piece things together.

## Immediate Goals
- Build instructions for Linux
- Setup precompiler header
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
