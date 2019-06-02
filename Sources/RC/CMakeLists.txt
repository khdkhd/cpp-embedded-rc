project(rc)

if(RC_SHARED)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} rc.cpp rc.hpp)

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
set_target_properties(${PROJECT_NAME}
  PROPERTIES
    PUBLIC_HEADER rc.hpp
)

install(TARGETS ${PROJECT_NAME}
  PUBLIC_HEADER DESTINATION include
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
