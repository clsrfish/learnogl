add_subdirectory(getting_started)
add_subdirectory(gl)
add_subdirectory(utils)

aux_source_directory(. DIR_LIB_SRC)

add_library (src ${DIR_LIB_SRC})
target_link_libraries(src getting_started gl utils)
