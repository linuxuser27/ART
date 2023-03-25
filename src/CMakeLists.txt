# Configure the compiler
include(../configure.cmake)

include_directories(BEFORE ./inc)

set(SOURCES
  main.cpp
)

set(HEADERS
  ./inc/art.hpp
  ./inc/camera.hpp
  ./inc/hittable.hpp
  ./inc/ray.hpp
  ./inc/shapes.hpp
  ./inc/vec3.hpp
)

add_executable(gen_ppm
  ${SOURCES}
  ${HEADERS}
)

install(TARGETS gen_ppm)