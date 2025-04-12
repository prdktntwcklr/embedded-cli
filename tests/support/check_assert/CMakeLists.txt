add_library(check_assert STATIC ${cexception_SOURCE_DIR}/lib/CException.c)
target_include_directories(check_assert PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${cexception_SOURCE_DIR}/lib)
target_link_libraries(check_assert PRIVATE unity)
