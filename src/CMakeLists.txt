# Configure the compiler
include(../configure.cmake)

include_directories(BEFORE ./inc)

set(SOURCES
  main.cpp
)

set(HEADERS
  ./inc/camera.hpp
  ./inc/elements.hpp
  ./inc/materials.hpp
  ./inc/ray.hpp
  ./inc/shapes.hpp
  ./inc/vec3.hpp
  ./inc/utility.hpp
)

add_executable(gen_ppm
  ${SOURCES}
  ${HEADERS}
)

install(TARGETS gen_ppm)