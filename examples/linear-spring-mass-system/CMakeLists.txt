get_filename_component(EXAMPLE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(EXAMPLE_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(EXAMPLE_INCLUDE_ROOT "${EXAMPLE_ROOT}/include")
set(EXAMPLE_SOURCE_ROOT "${EXAMPLE_ROOT}/source")
set(EXAMPLE_SHADER_ROOT "${EXAMPLE_ROOT}/shaders")
set(EXAMPLE_BINARY_ROOT "${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(${EXAMPLE_INCLUDE_ROOT})
add_subdirectory(${EXAMPLE_SOURCE_ROOT})
add_subdirectory(${EXAMPLE_SHADER_ROOT})

source_group("source" FILES ${SOURCE_LIST})
source_group("include" FILES ${INCLUDE_LIST})
source_group("shaders" FILES ${SHADER_LIST})

include_directories(${EXAMPLE_INCLUDE_ROOT} ${EXAMPLE_SHADER_ROOT})

add_executable(${EXAMPLE_NAME} ${SOURCE_LIST} ${INCLUDE_LIST} ${SHADER_LIST})
target_link_libraries(${EXAMPLE_NAME} ${ATLAS_LIBRARIES})
set_target_properties(${EXAMPLE_NAME} PROPERTIES FOLDER "examples")

add_custom_command(
    TARGET ${EXAMPLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${EXAMPLE_SHADER_ROOT} 
    ${EXAMPLE_BINARY_ROOT}/shaders)

add_custom_command(
    TARGET ${EXAMPLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${EX_DATA_ROOT} 
    ${EXAMPLE_BINARY_ROOT}/data)
