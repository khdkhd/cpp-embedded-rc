include_guard()

function(rc_generate OUTPUT_VAR)
  cmake_parse_arguments(GEN_BIN_RC "" "BASE;NAME" "FILES" ${ARGN})

  find_program(GEN_BIN_RC_GENERATOR rc-generate)

  if(NOT GEN_BIN_RC_GENERATOR)
    message(FATAL_ERROR "rc-generator executable not found!")
  endif()

  if(NOT GEN_BIN_RC_BASE)
    set(GEN_BIN_RC_BASE "${CMAKE_CURRENT_SOURCE_DIR}")
  endif()

  if(NOT GEN_BIN_RC_NAME)
    set(GEN_BIN_RC_NAME "binary_data")
  endif()

  set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/)
  set(OUTPUT_FILES ${OUTPUT_DIR}/${GEN_BIN_RC_NAME}.cpp)

  add_custom_command(
    OUTPUT ${OUTPUT_FILES}
    COMMAND "${GEN_BIN_RC_GENERATOR}"
      --base "${GEN_BIN_RC_BASE}"
      --name "${GEN_BIN_RC_NAME}"
      --output-dir "${OUTPUT_DIR}"
      ${GEN_BIN_RC_FILES}
    DEPENDS
      ${GEN_BIN_RC_FILES}
  )

  set(${OUTPUT_VAR} ${OUTPUT_FILES} PARENT_SCOPE)
endfunction()
