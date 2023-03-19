# Compiler configurations

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Configure CMake for platforms
#
if(WIN32)
  add_compile_definitions(BUILD_WINDOWS=1)
elseif(APPLE)
  add_compile_definitions(BUILD_MACOS=1)

  set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR ON)
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
  set(CMAKE_INSTALL_RPATH "@loader_path")
else()
  set(CMAKE_INSTALL_RPATH "$ORIGIN")
  add_compile_definitions(BUILD_UNIX=1)
endif()

#
# Configure compilers
#
if(MSVC)
  add_compile_options(/Zc:wchar_t-) # wchar_t is a built-in type.
  add_compile_options(/W4 /WX) # warning level 4 and warnings are errors.
  add_compile_options(/Zi) # enable debugging information.

  add_link_options(/DEBUG) # enable debugging information.
else()
  add_compile_options(-Wall -Werror) # All warnings and are errors.
  add_compile_options(-g) # enable debugging information.
endif()
