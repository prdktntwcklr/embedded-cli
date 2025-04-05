include(FetchContent)

set(unity_version "v2.6.1")
set(fff_version "v1.1")
set(fff_hash MD5=10a2d739289c1054f6784fcc7203c8fd)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG ${unity_version}
)

FetchContent_MakeAvailable(unity)

# download fff.h directly, since meekrosoft/fff does not work with FetchContent
set(fff_dir "${CMAKE_BINARY_DIR}/_deps/fff")

file(DOWNLOAD
  "https://github.com/meekrosoft/fff/releases/download/${fff_version}/fff.h"
  "${fff_dir}/fff.h"
  EXPECTED_HASH ${fff_hash}
)

add_subdirectory(support)

# define function to create unit test executables
function(add_unit_test module_name)
  add_executable(test_${module_name} test_${module_name}.c ${CMAKE_SOURCE_DIR}/src/${module_name}.c)
  target_compile_definitions(test_${module_name} PUBLIC TEST)
  target_include_directories(test_${module_name} PRIVATE ${CMAKE_SOURCE_DIR}/src ${fff_dir})
  target_link_libraries(test_${module_name} PUBLIC unity fake_printf)
  add_test(test_${module_name} test_${module_name})
endfunction()

add_unit_test(cli)
