cmake_minimum_required(VERSION 3.0)

project(CPPEmbeddedRC)

option(RC_SHARED "Enable/Disable dynamic library" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Scripts/CMake")

include(Conan)
include(Tests)

conan_check(VERSION 1.14)
conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)
conan_setup(
  CONANFILE "${CMAKE_SOURCE_DIR}/conanfile.py"
  CMAKE_TARGETS
  KEEP_RPATHS
)

add_subdirectory(Sources)
