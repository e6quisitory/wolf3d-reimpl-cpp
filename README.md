# Wolfenstein 3D Clone

<img src="https://user-images.githubusercontent.com/25702188/218297410-8b732b32-b8dc-4e6c-910e-b3732e5346de.png" width=300/>

1. [Introduction & Demo](#introduction)
2. [Build Instructions](#build)
    - [Windows](#windows-build)
    - [macOS](#macos)
    - [Linux (Debian-based)](#linux)
3. [Controls & Map Loading](#controls)
4. [Immediate Goals](#igoals)
5. [Later Goals](#lgoals)
6. [Credits](#credits)
7. [Resources Used](#resources)

<a name="introduction"/>

## Introduction
I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning most of the functionality in the game from scratch is the goal, including rendering the game world, enemies, weapons, sound system, etc. So far, I've implemented the following functionality:
- Raycasting
- Selectively textured walls with lighting
- Sprites
- GPU accelerated texture mapping
- Doors w/ automatic closing
- Map loading from a `csv` file
- Smooth movement using VSYNC
- Mouse to look around
- Minimap (dev tool)

Here's a couple short videos of what the engine is currently capable of:

<div align="center">
  <video src="https://user-images.githubusercontent.com/25702188/213652164-ee0d2622-4c79-4e63-87d3-8f52f17b8080.mp4" width=852/>
</div>
</br>
<div align="center">
<video src="https://user-images.githubusercontent.com/25702188/220486392-d4c782b8-bcb1-475a-883a-08ab6a0e1d46.mp4" width=500/>
</div>
</br>

The only dependacy of the project thus far is SDL2, used to open a window and render pixels into it, hardware-based texture scaling, reading keyboard and mouse inputs, and VSYNC.

The comments in the commits are full of detailed explanations on the implementation of parts of the engine, how I encountered and fixed bugs, etc. Worth looking at if you're building something like this yourself.

</a>

<a name="build"/>

## Build Instructions
</a>

<a name="windows-build"/>

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
    cmake -G "Ninja" ..
    ninja
    ```
6. Finally, run the executable like so:

    ```
    ./wolf3d-clone.exe
    ```
</a>

<a name="macos"/>

### macOS
1. Ensure `CMake` and `Ninja` are installed on your Mac through [Homebrew](https://brew.sh/):

    ```
    brew install cmake
    brew install ninja
    ```
2. Head on over to the [SDL releases page](https://github.com/libsdl-org/SDL/releases) and download the latest SDL release for macOS (with a `.dmg` extension).
    - After download, mount the `.dmg` archive (open it)
    - Inside you'll see a macOS framework bundle named `SDL2.framework`. Copy it.
    - Open a Finder window, hit `Command`+`Shift`+`G` and type in `/Library/Frameworks`
    - Now paste the copied `SDL2.framework` into this folder
    
2. Clone this repo and `cd` into it:

    ```
    git clone https://github.com/e6quisitory/wolf3d-clone.git
    cd wolf3d-clone
    ```
3. Run the following to build:

    ```
    mkdir build
    cd build
    cmake -G "Ninja" ..
    ninja
    ```
4. Now a clickable `.app` executable should be present in this build directory. You can run it from the terminal with the command below, and you can also navigate to it and simply double-click it as well to launch.

    ```
    open -n ./wolf3d-clone.app
    ```
    
</a>

<a name="linux"/>

### Linux (Debain-based)
Note that the below instructions have been tested on Ubuntu, and should work on other Debian-based distros as well (ex. Linux Mint, etc.). I cannot ensure that they'll work on non-Debian based distros, like Arch, CentOS, etc.

1. Update your package lists

    ```
    sudo apt update
    ```
2. Install the following packages

    ```
    sudo apt install build-essentials
    sudo apt install cmake
    sudo apt install ninja-build
    sudo apt install libsdl2-dev
    sudo apt install git
    ```
3. Clone this repo and `cd` into it:

    ```
    git clone https://github.com/e6quisitory/wolf3d-clone.git
    cd wolf3d-clone
    ```
4. Run the following to build:

    ```
    mkdir build
    cd build
    cmake -G "Ninja" ..
    ninja
    ```
5. Now run the compiled executable like so:

    ```
    ./wolf3d-clone
    ```
</a>

<a name="controls"/>

## Controls & Map Loading

### Game controls
- `W`, `A`, `S`, `D` to move
- Mouse to look around
- `spacebar` for opening doors
- `Esc` to exit game

### Minimap controls
- ~ (tilde) key to unlock/relock the mouse from the main window 
- Left click on tiles in the minimap to spawn to them (if possible)
- Right click anywhere on minimap to change player view direction

### Map loading
The map file is `map.csv`, located in the `assets` folder. It is ASCII encoded, not UTF-8 encoded. I find that editing the file is easiest in Excel.
As for how to construct the map, i.e. what the values in the `map.csv` file mean, I have yet to type up a new guide; I will get to that soon. In the mean time, you can read [this](https://github.com/e6quisitory/wolf3d-clone/commit/3f9f9e308629098a29df2e3ec8103fddd3fc51c0#commitcomment-88618626) old guide and look at the values in the included `map.csv` and try to piece things together.

</a>

<a name="igoals"/>

## Immediate Goals
- Enemy AI & character animation
- Weapons
- Multithreaded raycasting
  - Thread pool
  - Mutexes/locks for shared data access b/w threads

</a>

<a name="lgoals"/>

## Later Goals
- Linux pre-compiled binary
- Write a more sophisticated CMake script that:
    - In `Debug` config simply compiles into executable
    - In `Release` config makes a proper, signed, pre-compiled binary package for the platform (Windows, macOS, Linux) using CPack
        - Downloads a copy of SDL2, puts it inside the package, and links executable to that local copy (changes `RPATH`)
        - Puts `assets` folder inside binary package as well
- Darken distant walls (shadow/depth effect)
- Textured ceiling & floor
- Add joystick support
- Port to browser using Webassembly
- Networked multiplayer _(aspirational)_
- Proper map editor _(aspirational)_
- Port to iOS?

</a>

<a name="credits"/>

## Credits
All wall and sprite textures + the logo at the top of this README are from the official Wolfenstein 3D artwork that shipped in the game back in 1992. I found them [here](https://www.spriters-resource.com/pc_computer/wolfenstein3d/).

</a>

<a name="resources"/>

## Resources Used
- _Game Engine Blackbook - Wolfenstein 3D_ by Fabien Sanglard
- [Wolfenstein 3D's map renderer](https://www.youtube.com/watch?v=eOCQfxRQ2pY) (video)
- [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA) (video)
- [Make Your Own Raycaster Series](https://www.youtube.com/watch?v=gYRrGTC7GtA) (video)

</a>
