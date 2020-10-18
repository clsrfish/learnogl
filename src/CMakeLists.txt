set(SUB_DIRS advanced_ogl breakout getting_started gl lighting model_loading utils)

foreach (DIR IN LISTS SUB_DIRS)
    message("Adding sub-directory: ${DIR}")
    add_subdirectory(${DIR})
endforeach ()

aux_source_directory(. SRC)

add_library (src ${SRC})
target_link_libraries(src ${SUB_DIRS})
