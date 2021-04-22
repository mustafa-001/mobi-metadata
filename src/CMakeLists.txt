set(sources parse_int exth_header palm_header record0)
add_executable(karalama main ${sources})
target_link_libraries(karalama stdc++)
add_library(karalama-static STATIC ${sources} )
