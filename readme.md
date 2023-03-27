# ART

Following https://github.com/RayTracing/raytracing.github.io

## Requirements (minimum)

- [CMake](https://cmake.org/download/) 3.20

- C++20 compliant compiler

## Build

Using "Release" by default. Set to "Debug" in order to debug the application.
> `cmake -S . -B artifacts -DCMAKE_BUILD_TYPE=Release`

> `cmake --build artifacts --target install`