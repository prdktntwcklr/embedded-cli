function(add_unit_test module_name)
  add_executable(test_${module_name} test_${module_name}.c ${CMAKE_SOURCE_DIR}/src/${module_name}.c)
  target_compile_definitions(test_${module_name} PUBLIC TEST)
  target_include_directories(test_${module_name} PRIVATE ${CMAKE_SOURCE_DIR}/src ${fff_dir})
  target_link_libraries(test_${module_name} PUBLIC unity fake_printf)
  add_test(test_${module_name} test_${module_name})
endfunction()

add_unit_test(cli)