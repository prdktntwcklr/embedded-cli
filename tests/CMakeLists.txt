include(FetchContent)

set(unity_version "v2.6.1")
set(cexception_version "v1.3.4")
set(fff_version "v1.1")
set(fff_hash MD5=10a2d739289c1054f6784fcc7203c8fd)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG ${unity_version}
)

FetchContent_MakeAvailable(unity)

FetchContent_Declare(
  cexception
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/CException.git
  GIT_TAG ${cexception_version}
)

FetchContent_MakeAvailable(cexception)

set(cexception_dir "${CMAKE_BINARY_DIR}/_deps/cexception-src/lib")

# download fff.h directly, since meekrosoft/fff does not work with FetchContent
set(fff_dir "${CMAKE_BINARY_DIR}/_deps/fff")

file(DOWNLOAD
  "https://github.com/meekrosoft/fff/releases/download/${fff_version}/fff.h"
  "${fff_dir}/fff.h"
  EXPECTED_HASH ${fff_hash}
)

add_subdirectory(support)
add_subdirectory(test_cli)
