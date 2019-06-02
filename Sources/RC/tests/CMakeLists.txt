project(rc-tests)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    rc
    CONAN_PKG::gtest
)
add_test(
  NAME ${PROJECT_NAME}
  COMMAND $<TARGET_FILE:${PROJECT_NAME}> --gtest_output=xml:${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}.xml
)
add_dependencies(Tests ${PROJECT_NAME})
