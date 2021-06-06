# Light [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Build Status](https://travis-ci.com/dropTableUsers42/Light.svg?branch=main)](https://travis-ci.com/dropTableUsers42/Light) [![Windows](https://github.com/dropTableUsers42/Light/actions/workflows/cmake.yml/badge.svg)](https://github.com/dropTableUsers42/Light/actions/workflows/cmake.yml)
Lightweight Renderer, purposed to augment the development of Physicc

## Supported Platforms

* Windows 10
* Ubuntu 18
* Ubuntu 20
* OSX 10

## Prerequisites

### Common

* CMake

### Windows

* mingw-w64-gcc
* mingw-w64-make

#### OR

* Visual Studio 2019

### Ubuntu

* GCC (>= 8)
* X11

If you get library headers not found error while building, ```apt-get``` the dev version of all packages it requests you to install

```sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev```

### OSX

* XCode (>= 12)

## Build Instructions

* Recursively clone the git module:

	```git clone --recursive https://github.com/dropTableUsers42/Light```

### VSCode

* If you have the [CMake tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension, then select location as `Editor` folder, select kit as GCC/VS 2019, and build target as `Editor`

* Hit Build

### On Terminal

* Change into directory

	`cd Light`

* Execute the following command

	`cmake --no-warn-unused-cli -HEditor -Bbuild -G"[Generator]"`

	Replace `[Generator]` with the compiler name you want to compile with, 
  `MinGW Makefiles`/`Unix Makefiles`/`Visual Studio 16 2019`. The corresponding Makefile/solution file is generated in `Light/build`

* If you want to build on terminal, execute

	`cmake --build build`


* If you want to build using Visual Studio 2019, open the Solution File generated

## Run

### On Terminal

* If you do not have a dedicated GPU (or drivers for the dedicated GPU e.g. on Linux), you may need to run the following command on the terminal to set up the environment variables before running our program

	`export MESA_GL_VERSION_OVERRIDE=3.3`

* Change into build directory (This should be the current working directory)
	
	`cd build`
	
* Run the executable
	
	`./Editor`

### In IDE

* Use the default IDE command `run`, make sure the current working directory is `Light/build`




