add_executable(test test )
##target_include_directories(${})
target_link_libraries(test mobi-metadata-static gtest_main gtest pthread)