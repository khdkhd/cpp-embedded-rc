cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

## ## am-conan

## ### Variables

option(SKIP_CONAN_INSTALL "Skip conan setup" OFF)

## ### Functions

macro(conan_load_buildinfo)
  if(IS_MULTI_CONFIG)
    set(_CONANBUILDINFO conanbuildinfo_multi.cmake)
  else()
    set(_CONANBUILDINFO conanbuildinfo.cmake)
  endif()

  # Checks for the existence of conanbuildinfo.cmake, and loads it important
  # that it is macro, so variables defined at parent scope
  if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${_CONANBUILDINFO}")
    message(STATUS "Conan: Loading ${_CONANBUILDINFO}")
    include(${CMAKE_CURRENT_BINARY_DIR}/${_CONANBUILDINFO})
  else()
    message(FATAL_ERROR "${_CONANBUILDINFO} doesn't exist in ${CMAKE_CURRENT_BINARY_DIR}")
  endif()

  set(_CONANPATHS "conan_paths.cmake")
  if(EXISTS "${CMAKE_BINARY_DIR}/${_CONANPATHS}")
    message(STATUS "Conan: Loading ${_CONANPATHS}")
    include(${CMAKE_BINARY_DIR}/${_CONANPATHS})
  endif()
endmacro()

## #### conan_setup
## Cette ajoute le remote spécifié si ce dernier n'est pas déjà présent dans la
## liste des remotes.
##
## ##### Synopsis:
## ```cmake
## conan_setup(
##   conanfile
##   [CMAKE_TARGETS]
##   [KEEP_RPATHS]
##   [SKIP_INSTALL]
##   [SETTINGS <settings1=value1>...]
##   [OPTIONS <options1=value1>...]
## )
## ```
##
## ##### Arguments:
## - `CONANFILE <conanfile>`, path to the conanfile, default to ${CMAKE_SOURCE_DIR}/conanfile.txt
## - `CMAKE_TARGETS`, enable CMake targets,
## - `KEEP_RPATHS`, enable keep RPATH,
## - `SKIP_INSTALL`, skip conan install,
## - `SETTINGS <settings1=value1>...`, add settings
## - `OPTIONS <options1=value1>...`, add options.
##
## ##### Usage:
## ```cmake
## conan_setup(
##   CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.txt
##   CMAKE_TARGETS
##   KEEP_RPATHS
## )
## ```
macro(conan_setup)
  cmake_parse_arguments(CONAN_SETUP "SKIP_INSTALL;CMAKE_TARGETS;KEEP_RPATHS;" "CONANFILE;PROFILE" "SETTINGS;OPTIONS" ${ARGN})

  get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

  find_program(CONAN conan)

  if(NOT CONAN_SETUP_CONANFILE)
    set(CONAN_SETUP_CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.txt)
  endif()

  if(IS_MULTI_CONFIG)
    list(APPEND CONAN_BUILD_TYPES Debug Release)
    list(APPEND CONAN_ARGS -g cmake_multi)
  elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    list(APPEND CONAN_BUILD_TYPES "Debug")
  else()
    list(APPEND CONAN_BUILD_TYPES "Release")
  endif()

  if(WIN32)
    if(CMAKE_GENERATOR_PLATFORM STREQUAL x64)
      list(APPEND CONAN_SETUP_SETTINGS arch=x86_64)
    else()
      list(APPEND CONAN_SETUP_SETTINGS arch=x86)
    endif()
  endif()

  foreach(SETTING ${CONAN_SETUP_SETTINGS})
    list(APPEND CONAN_ARGS -s ${SETTING})
  endforeach()

  foreach(OPTION ${CONAN_SETUP_OPTIONS})
    list(APPEND CONAN_ARGS -o ${OPTION})
  endforeach()

  if(CONAN_SETUP_PROFILE)
    list(APPEND CONAN_ARGS "-pr=${CONAN_SETUP_PROFILE}")
  endif()

  if(NOT (CONAN_SETUP_SKIP_INSTALL OR SKIP_CONAN_INSTALL))
    foreach(BUILD_TYPE ${CONAN_BUILD_TYPES})
      execute_process(
        COMMAND ${CONAN} install
          ${CONAN_ARGS}
          -s build_type=${BUILD_TYPE}
          --build=missing
          "${CONAN_SETUP_CONANFILE}"
        RESULT_VARIABLE CONAN_RESULT
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      )
    endforeach()
  else()
    message(STATUS "Conan: Skip install")
  endif()

  conan_load_buildinfo()

  if(CONAN_SETUP_CMAKE_TARGETS)
    list(APPEND CONAN_BASIC_SETUP_OPTIONS TARGETS)
  endif()

  if(CONAN_SETUP_KEEP_RPATHS)
    list(APPEND CONAN_BASIC_SETUP_OPTIONS KEEP_RPATHS)
  endif()

  conan_basic_setup(${CONAN_BASIC_SETUP_OPTIONS})
endmacro()

## #### conan_check
## Cette fonction vérifie la présence de conan sur le système et la version
## installée.
##
## ##### Synopsis:
## ```cmake
## conan_check(
##   [VERSION <major.minor[.patch]>]
## )
## ```
##
## ##### Arguments:
## - `VERSION <major.minor[.patch]>`, la version requise de conan
##
## ##### Usage:
## ```cmake
## conan_check(VERSION 1.14)
## ```
function(conan_check)
  cmake_parse_arguments(CONAN_CHECK "" "VERSION" "" ${ARGN})

  find_program(CONAN conan)

  if(NOT CONAN)
    message(FATAL_ERROR "conan command not found.")
  endif()

  if(CONAN_CHECK_VERSION)
    execute_process(
      COMMAND ${CONAN} --version
      OUTPUT_VARIABLE CONAN_VERSION_RAW
      ERROR_VARIABLE CONAN_ERROR
      RESULT_VARIABLE CONAN_RESULT
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    if(NOT CONAN_RESULT EQUAL 0)
      message(FATAL_ERROR "${CONAN_ERROR}")
    endif()

    separate_arguments(CONAN_VERSION UNIX_COMMAND ${CONAN_VERSION_RAW})
    list(GET CONAN_VERSION 2 CONAN_VERSION)

    message(STATUS "Conan version is ${CONAN_VERSION}")

    if(CONAN_VERSION VERSION_LESS CONAN_CHECK_VERSION)
      message(FATAL_ERROR "Conan version required is ${CONAN_CHECK_VERSION}")
    endif()
  endif()
endfunction()

## #### conan_add_remote
## Cette ajoute le remote spécifié si ce dernier n'est pas déjà présent dans la
## liste des remotes.
##
## ##### Synopsis:
## ```cmake
## conan_add_remote(
##   NAME foobar
##   URL http://foo.bar.com
## )
## ```
##
## ##### Arguments:
## - `NAME <name>`, le nom du remote à enregistrer,
## - `URL <url>`, l'url du remote.
##
## ##### Usage:
## ```cmake
## conan_add_remote(
##   NAME foobar
##   URL http://foo.bar.com
## )
## ```
function(conan_add_remote)
  cmake_parse_arguments(CONAN_REMOTE "" "NAME;URL" "" ${ARGN})

  if(NOT CONAN_REMOTE_NAME)
    message(FATAL_ERROR "NAME parameter is required")
  endif()

  if(NOT CONAN_REMOTE_URL)
    message(FATAL_ERROR "URL parameter is required")
  endif()

  find_program(CONAN conan)

  if(NOT CONAN_REMOTE_URL)
    message(FATAL_ERROR "conan command not found")
  endif()

  execute_process(
    COMMAND ${CONAN} remote list --raw
    OUTPUT_VARIABLE CONAN_REMOTE_LIST_RAW
    ERROR_VARIABLE CONAN_ERROR
    RESULT_VARIABLE CONAN_RESULT
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  )

  if(NOT CONAN_RESULT EQUAL 0)
    message(FATAL_ERROR "${CONAN_ERROR}")
  endif()

  separate_arguments(CONAN_REMOTE_LIST UNIX_COMMAND ${CONAN_REMOTE_LIST_RAW})

  while(TRUE)
    list(GET CONAN_REMOTE_LIST 0 NAME)
    list(GET CONAN_REMOTE_LIST 1 URL)

    if("${URL}" STREQUAL "${CONAN_REMOTE_URL}")
      message(STATUS "Conan: Remote ${CONAN_REMOTE_NAME}=\"${CONAN_REMOTE_URL}\" already exists.")
      return()
    endif()

    list(LENGTH CONAN_REMOTE_LIST CONAN_REMOTE_LIST_SIZE)
    if(CONAN_REMOTE_LIST_SIZE GREATER 3)
      list(SUBLIST CONAN_REMOTE_LIST 3 -1 CONAN_REMOTE_LIST)
    else()
      break()
    endif()
  endwhile()

  message(STATUS "Conan: Remote ${CONAN_REMOTE_NAME}=\"${CONAN_REMOTE_URL}\" added.")
  execute_process(
    COMMAND ${CONAN} remote add ${CONAN_REMOTE_NAME} ${CONAN_REMOTE_URL}
    ERROR_VARIABLE CONAN_ERROR
    RESULT_VARIABLE CONAN_RESULT
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  )

  if(NOT CONAN_RESULT EQUAL 0)
    message(FATAL_ERROR "${CONAN_ERROR}")
  endif()
endfunction()
