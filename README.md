Light
====
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Windows](https://github.com/Physicc/Light/actions/workflows/windows.yml/badge.svg)](https://github.com/Physicc/Light/actions/workflows/windows.yml) [![OSX](https://github.com/Physicc/Light/actions/workflows/osx.yml/badge.svg)](https://github.com/Physicc/Light/actions/workflows/osx.yml) [![Linux GCC](https://github.com/Physicc/Light/actions/workflows/linux-gcc.yml/badge.svg)](https://github.com/Physicc/Light/actions/workflows/linux-gcc.yml) [![Linux Clang](https://github.com/Physicc/Light/actions/workflows/linux-clang.yml/badge.svg)](https://github.com/Physicc/Light/actions/workflows/linux-clang.yml)

Lightweight Game Engine written in C++

----

Light is a lightweight game engine written in C++. It is currently in development and is not yet ready for use. We are a small team consisting of students from Indian Institute of Technology Bombay. We are working on this project to primarily learn about game engine development, and to document our process so that others can learn from our mistakes.

## Modules

* **Light**: The rendering backbone of the engine
	* Wraps around OpenGL API
* **LightFramework**: The main game engine framework
	* Input
	* Windowing
	* ECS
	* GUI
* **Physicc**: The physics engine we are building from scratch
	* Rigid Body Physics
* **Editor**: A level editor built using LightFramework

## File Structure

Each module has its own folder.

* **include** contains the header files
* **src** contains the source files
* **libs** contains the external library sources

There is a **shared** folder which contains all the common libraries and files to be shared amongst the modules.

## Supported Platforms

We aim to support as the most common desktop platforms

* Windows 10/11
* Ubuntu 18
* Ubuntu 20
* OSX 10
* OSX 11

## Prerequisites

### Windows

* mingw-w64
* CMake

#### OR

* Visual Studio 2019
* CMake

### Ubuntu

* GCC (>= 8) or Clang (>= 9)
* X11
* CMake

Install X11 via

```bash
sudo apt install xorg-dev
```

### OSX

* XCode (>= 12)
* CMake

## Build and Run

### VSCode

* Recursively clone the git module:

	```bash
	git clone --branch development --single-branch --recursive https://github.com/Physicc/Light
	```

	This will clone only the development branch of the repository. This is what you probably want.

* Get the [CMake tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension

* Select CMake source location as the repository root folder if prompted

* Select kit
	* Select `GCC` if you are using `mingw-w64` as your compiler
	* Select kit as `Visual Studio 2019` if you are using `MSVC (from VS2019)` as your compiler
	
* Select build target as `Editor`

* Press the Build button

	![image](https://imgur.com/tGXw3XQ.png)

* Now press the Run button

	![image](https://imgur.com/99V3q5C.png)

### Terminal

* Recursively clone the git module:

	```bash
	git clone --branch development --single-branch --recursive https://github.com/Physicc/Light
	```

	This will clone only the development branch of the repository. This is what you probably want.

* Change into the repository directory

	```bash
	cd Light
	```

* Execute the following command to generate the build configuration

	```bash
	cmake -Bbuild
	```

	You can also specify the build generator if you have more than one via the `-G` flag.
	The generators we support are `MinGW Makefiles`, `Unix Makefiles`, `Visual Studio 16 2019`, `Xcode`

	e.g.
	```bash
	cmake -Bbuild -G"Unix Makefiles"
	```

* Build the executable using the following command

	```bash
	cmake --build build
	```

	On Linux, you can also append `-- -j<num>` to the end of the above command, to run the build on `<num>` cores.
	
	eg. 
	```bash
	cmake --build build -- -j4	# for 4 cores
	```

* If you are on Linux and do not have a dedicated GPU, run the following command

	```bash
	export MESA_GL_VERSION_OVERRIDE=3.3
	```

* Change into build directory (This should be the current working directory)
	
	```bash
	cd build
	```
* Run the executable
	
	```bash
	./Editor
	```

### Visual Studio 2019

* You can if you wish to, use VS2019 as your IDE instead of VSCode on Windows.

* Recursively clone the git module:

	```bash
	git clone --branch development --single-branch --recursive https://github.com/Physicc/Light
	```

	This will clone only the development branch of the repository. This is what you probably want.

* Change into the repository directory

	```bash
	cd Light
	```

* Execute the following command to generate the build configuration

	```bash
	cmake -Bbuild -G"Visual Studio 16 2019"
	```

* Now you can open the solution file generated in Visual Studio 2019.

* You will need to regenerate the solution file every time you modify the source code.

* We currently do not provide any additional support if you wish to use Visual Studio as your IDE

## Documentation

[Link](https://physicc.github.io/Light/) to documentation (may be outdated)

## Contributing

If you want to contribute to this project, please drop an email to [Rwitaban](mailto:rwitaban0001@gmail.com)

You can send pull requests, but please follow the guidelines on [CONTRIBUTING.md](https://github.com/Physicc/Light/blob/main/CONTRIBUTING.md)


## Attributions

* Icon created by Chaowalit Koetchuea for thenounproject

* Project heavily inspired from [Cherno's Hazel](https://github.com/TheCherno/Hazel)




