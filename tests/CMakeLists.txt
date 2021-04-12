add_executable(test test )
##target_include_directories(${})
target_link_libraries(test karalama-static gtest_main gtest pthread)