add_library(fake_printf STATIC fake_printf.c)
target_include_directories(fake_printf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fake_printf PRIVATE unity)
