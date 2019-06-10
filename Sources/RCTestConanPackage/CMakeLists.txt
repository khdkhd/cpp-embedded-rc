cmake_minimum_required(VERSION 3.0)

project(ConanPackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

conan_basic_setup()

include(rc/generate)

rc_generate(
  ASSETS
  NAME Assets
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/bar
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/foo
)

add_executable(${PROJECT_NAME}
  main.cpp
  ${ASSETS}
)
target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_17
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${CONAN_LIBS}
)

enable_testing()
add_test(
  NAME ${PROJECT_NAME}
  COMMAND $<TARGET_FILE:${PROJECT_NAME}>
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
