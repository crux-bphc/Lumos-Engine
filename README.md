# Lumos-Engine
A 2-D game engine written in C++.

# How to run
1. Run `meson setup build` to create the build directory
2. Go to build directory with `cd build`
3. Run `meson compile && ./debug`
4. To change target file change the same in `meson.build`

# How to set up build directory type
1. In the parent dirctory calkl `meson build --buildtype debug`
2. Here `build` is the directory name. We(Lumos Team) are choosing this by convention. If you want to rename it remember to add it to `.gitignore.`
3. Furthermore, `debug` refers to build type. You can choose from `plain`, `debug`, `debugoptimized`, `release`, `minsize`, `custom`. You can read more about it [here](https://mesonbuild.com/Builtin-options.html#details-for-buildtype).
4. Then proceed as stated in [How to run](#how-to-run)

# Dependencies
1. For GLFW and GLEW: `sudo apt-get install libglfw3-dev libglew-dev`
2. For spdlog: `sudo apt-get install libspdlog-dev`
3. glm version `0.9.9.8` (pre installed as header library in `Lumos` directory)

# File structure
1. The engine code is present in `Lumos` directory
2. Testing out feature can be done by making another file in `Test` folder (change the target in `meson.build`)

# Documentation
The documentation for this project can be found [here](https://lambert-crypto.github.io/Lumos-Docs/).

# How to use spdlog
```cpp
#include "spdlog/spdlog.h"

int main() 
{
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
    
    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");
    
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");    
    
    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    
    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");
}
```
