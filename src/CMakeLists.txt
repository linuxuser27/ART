# Configure the compiler
include(../configure.cmake)

include_directories(BEFORE ./inc)

set(SOURCES
  main.cpp
)

set(HEADERS
  ./inc/vec3.hpp
  ./inc/ray.hpp
)

add_executable(gen_ppm
  ${SOURCES}
  ${HEADERS}
)

install(TARGETS gen_ppm)