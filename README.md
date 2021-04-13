# Light [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
Lightweight Renderer, purposed to augment the development of Physicc

## Platforms

Platforms currently it compiles on:
* Windows 10
* Ubuntu 18

## Prerequisites

* cmake

### Windows

* MinGW / VS 2019

### Ubuntu

* gcc
* X11

### Mac

* XCode
* Cocoa
* IOKit
* CoreVideo

## Build Instructions

Recursive clone the git module

```git clone --recursive https://github.com/dropTableUsers42/Light```

### VSCode

If you have the [CMake tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension, then simply select location as `Editor` folder, select kit as GCC/VS 2019, and build target as `Editor`

Hit Build

### On Terminal

* Change into directory

	`cd Light`

* Execute the following command

	`cmake --no-warn-unused-cli -HEditor -Bbuild -G"[Generator]"`

	Replace `[Generator]` with the compiler name you want to compile with, `MinGW Makefiles`/`Unix Makefiles`/`Visual Studio 16 2019`. The corresponding makefile/solution file is generated into `Light/build`

* If you want to build on terminal, execute

	`cmake --build build`

* If you want to build using Visual Studio 2019, just open the solution file generated




