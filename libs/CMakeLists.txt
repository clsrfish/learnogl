# used for source depend
add_subdirectory(glad)
add_subdirectory(stb_image)

aux_source_directory(. SRC)

# add_library(libs ${SRC})
# target_link_libraries(libs glad stb_image)