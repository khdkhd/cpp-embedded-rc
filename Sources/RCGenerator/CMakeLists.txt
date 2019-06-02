project(rc-generate)

add_executable(${PROJECT_NAME}
  generator.cpp
  generator.hpp
  io.cpp
  io.hpp
  utils.hpp
  main.cpp
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    CONAN_PKG::boost_format
    CONAN_PKG::boost_filesystem
    CONAN_PKG::boost_program_options
)
target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_17
)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)
