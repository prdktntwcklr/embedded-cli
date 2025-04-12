add_library(check_assert STATIC ${cexception_dir}/CException.c)
target_include_directories(check_assert PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${cexception_dir})
target_link_libraries(check_assert PRIVATE unity)
