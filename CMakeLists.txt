cmake_minimum_required(VERSION 3.0)

project(CPPEmbeddedRC)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Scripts/CMake")

include(Conan)

conan_check(VERSION 1.14)
conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)
conan_install(
  CMAKE_TARGETS
  KEEP_RPATHS
)

add_subdirectory(Sources)
