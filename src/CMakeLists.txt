set(SUB_DIRS getting_started gl lighting utils)

foreach (DIR IN LISTS SUB_DIRS)
    message("Adding sub-directory: ${DIR}")
    add_subdirectory(${DIR})
endforeach ()

aux_source_directory(. SRC)

add_library (src ${SRC})
target_link_libraries(src ${SUB_DIRS})
