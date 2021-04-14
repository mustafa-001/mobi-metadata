set(sources parse_int exth_header palm_header record0)
add_executable(mobi-metadata main ${sources})
target_link_libraries(mobi-metadata stdc++)
add_library(mobi-metadata-static STATIC ${sources} )
