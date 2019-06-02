include_guard()

function(generate_binary_resources OUTPUT_VAR)
  cmake_parse_arguments(GEN_BIN_RC "" "BASE;NAME" "FILES" ${ARGN})

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
    COMMAND $<TARGET_FILE:RCGenerator>
      --base "${GEN_BIN_RC_BASE}"
      --name "${GEN_BIN_RC_NAME}"
      --output-dir "${OUTPUT_DIR}"
      ${GEN_BIN_RC_FILES}
    DEPENDS
      RCGenerator
      ${GEN_BIN_RC_FILES}
  )

  set(${OUTPUT_VAR} ${OUTPUT_FILES} PARENT_SCOPE)
endfunction()
