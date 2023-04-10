# Wave Engine

A repository for experimenting on opengl (3.3) and getting a better grasp on how to communicate and manipulate
information passed on to the GPU for future app development.

## How to install libraries

- Install glfw (3.3) parse_shader package library from https://www.glfw.org/download
- Once extracted, create a build directory using ``mkdir -p build/`` at the root directory of the glfw folder.
- At the root directory of the glfw folder, run the following command ``cmake -S . -B build/``.
- Upon success, run the next command ``make`` to build the files.
- Upon success, run the last command ``sudo make install`` to write the local glfw libraries to
  the ``/usr/local/include`` path.

## How to load libraries in a c/c++ file

- Add the line ``#include <GLFW/glfw3.h>`` to the main file that will be run.

## How to link glfw libraries in Cmake

- add the following lines to your ``CMakeLists.txt`` :

```
  find_package(glfw3 REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(X11 REQUIRED)
  
  target_link_libraries(opengl-dev PUBLIC glfw cunit OpenGL::GL ${CMAKE_DL_LIBS})
```

### Finally, build project and cross your fingers that CMake does not give you any errors!