project(RC)

add_library(${PROJECT_NAME}
  rc.cpp
  rc.hpp
)
target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_17
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    CONAN_PKG::boost_format
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
