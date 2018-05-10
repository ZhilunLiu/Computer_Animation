set(SHADER_LIST
    "${EXAMPLE_SHADER_ROOT}/Model.vs.glsl"
    "${EXAMPLE_SHADER_ROOT}/Model.fs.glsl"
    PARENT_SCOPE)

set_source_files_properties(${SHADER_LIST} PROPERTIES HEADER_FILE_ONLY TRUE)
