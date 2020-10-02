add_subdirectory(utils)
add_subdirectory(getting_started)

aux_source_directory(. DIR_LIB_SRC)

add_library (src ${DIR_LIB_SRC})
target_link_libraries(src utils getting_started)
