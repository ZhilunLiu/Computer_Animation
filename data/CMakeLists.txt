set(EX_DATA_LIST
    "${EX_DATA_ROOT}/cube.obj"
    "${EX_DATA_ROOT}/dragon.mtl"
    "${EX_DATA_ROOT}/dragon.obj"
    "${EX_DATA_ROOT}/suzanne.obj"
    "${EX_DATA_ROOT}/ball.obj"
    PARENT_SCOPE)

set_source_files_properties(${EX_DATA_LIST}
    PROPERTIES HEADER_FILE_ONLY TRUE)
