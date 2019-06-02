include(Resources)

project(RCTest)

generate_binary_resources(
  ASSETS1
  NAME Assets1
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/txt/bar
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/txt/foo
)

generate_binary_resources(
  ASSETS2
  NAME Assets2
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/png/picture.png
)

add_executable(${PROJECT_NAME}
  main.cpp
  ${ASSETS1}
  ${ASSETS2}
)
target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_17
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    RC
)
