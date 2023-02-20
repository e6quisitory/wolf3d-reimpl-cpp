# Wolfenstein 3D Clone

<img src="https://user-images.githubusercontent.com/25702188/218297410-8b732b32-b8dc-4e6c-910e-b3732e5346de.png" width=300/>

1. [Introduction & Demo](#introduction)
2. [Pre-compiled Binaries](#binaries)
3. [Build Instructions](#build)
    - [Windows](#windows-build)
    - [macOS](#macos)
    - [Linux (Debian-based)](#linux)
4. [Controls & Map Loading](#controls)
5. [Immediate Goals](#igoals)
6. [Later Goals](#lgoals)
7. [Credits](#credits)
8. [Resources Used](#resources)

<a name="introduction"/>

## Introduction
I'm attempting to make a clone of the classic 1992 id Software game, Wolfenstein 3D. Cloning most of the functionality in the game from scratch is the goal, including rendering the game world, enemies, weapons, sound system, etc. So far, I've implemented the following functionality:
- Raycasting
- Selectively textured walls with lighting
- Sprites
- GPU accelerated texture mapping
- Multithreaded rendering
- Doors w/ automatic closing
- Map loading from a `csv` file
- Smooth movement using VSYNC
- Mouse to look around
- Minimap (dev tool)

Here is a short video of what the engine is currently capable of:

<div align="center">
  <video src="https://user-images.githubusercontent.com/25702188/213652164-ee0d2622-4c79-4e63-87d3-8f52f17b8080.mp4" width=852/>
</div>

The only dependacy of the project thus far is SDL2, used to open a window and render pixels into it, hardware-based texture scaling, reading keyboard and mouse inputs, and VSYNC.

The comments in the commits are full of detailed explanations on the implementation of parts of the engine, how I encountered and fixed bugs, etc. Worth looking at if you're building something like this yourself.

</a>

<a name="binaries"/>

## Pre-compiled Binaries
I've made pre-compiled binaries for Windows and macOS. Head on over to the [Releases](https://github.com/e6quisitory/wolf3d-clone/releases) section to obtain them, and see the below instructions on how to run them.

Linux users will have to resort to building the project themselves, instructions [here](#linux). Someday I'll get around to making a Linux binary as well, but for now, I've had enough of build systems and want to get back to coding.

### Windows
Unzip the archive, go into the `bin` folder, then double-click on `wolf3d-clone.exe`.

If you're running Windows 11, there is a chance SmartScreen might flag the executable as being potentially malicious. This is due to the code not being signed (I still gotta figure that out). So for now, either turn off SmartScreen temporarily or allow the executable to bypass it (if the option is present).

### macOS (x86 & ARM)
Unzip the archive. Inside you'll find a `run.command` script. Double click it to launch the game. It is expected that macOS will initially not allow you to run the script; there'll be a pop up saying:
```
"run.command” cannot be opened because it is from an unidentified developer
```
This is due to the code not being signed (I have yet to figure that out). So, for now, close that pop up, then go to:
```
System Settings -> Privacy & Security
```
There, at the bottom you'll see something along the lines of **_"run.command" was blocked..."_** and next to it a button that says `Open Anyway`. Hit that, after which it'll likely prompt you for your password/fingerprint, after which there'll be another dialog, hit enter on that as well, and finally the game should launch.

</a>

<a name="build"/>

## Build Instructions
If you'd like to compile/build the project yourself, below are instructions on how to do so for each major platform.

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
    cmake -G "Ninja" ../src
    ninja -j $(sysctl -n hw.physicalcpu)
    ```
4. Now a clickable `.app` executable should be present in this build directory. You can run it from the terminal with the below command, and you can also navigate to it and simply double-click it as well to launch.

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
    sudo apt install clang
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
    cmake -G "Ninja" ../src -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
    ninja
    ```
5. Now run the compiled executable like so:

    ```
    ./wolf3d-clone
    ```
**Aside**

If after running `ninja`, you get compilation errors of C++ standard library headers not being found (like `<iostream>` etc.), run the following command:

```
clang -v
```
Now check the following two lines of the output:

```
Found candidate GCC installation: ...
Selected GCC installation: ...
```
Now here, make sure that the _selected_ GCC version is one of the ones _found_. It is possible for the selected version to be one that is not even present on your system, which will cause the standard library headers to not be found. If in your case the selected version is not one of the ones found, you can simply install that version of GCC with the following command:

```
sudo apt install g++-n
```
Where `n` is the version you wish to install. So, for example, if the selected version is GCC 12 and it's not one of the versions found, you can install it like so:

```
sudo apt install g++-12
```
After the GCC installation is finished, make sure you're still in the `build` directory, then run the following set of commands to rebuild the project and run the compiled executable. Now there should be no missing STL header errors that get thrown.

```
rm -r *
cmake -G "Ninja" ../src -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
ninja
./wolf3d-clone
```

</a>

<a name="controls"/>

## Controls & Map Loading
`W`, `A`, `S`, `D` to move, mouse to look around, `spacebar` for opening doors, `Esc` to exit game.

The map file is `map.csv`, located in the `assets` folder. It is ASCII encoded, not UTF-8 encoded. I find that editing the file is easiest in Excel.

As for how to construct the map, i.e. what the values in the `map.csv` file mean, I have yet to type up a new guide; I will get to that soon. In the mean time, you can read [this](https://github.com/e6quisitory/wolf3d-clone/commit/3f9f9e308629098a29df2e3ec8103fddd3fc51c0#commitcomment-88618626) old guide and look at the values in the included `map.csv` and try to piece things together.

</a>

<a name="igoals"/>

## Immediate Goals
- Character/sprite animation
- Weapons
- Enemy AI

</a>

<a name="lgoals"/>

## Later Goals
- Linux pre-compiled binary
- Write a more sophisticated CMake script that:
    - In `Debug` config simply compiles into executable
    - In `Release` config makes a proper, signed, pre-compiled binary package for the platform (Windows, macOS, Linux) using CPack
        - Downloads a copy of SDL2, puts it inside the package, and links executable to that local copy (changed `RPATH`)
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
