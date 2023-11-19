# force CMake to use Unix Makefiles generator
# reference: https://stackoverflow.com/questions/11269833/cmake-selecting-a-generator-within-cmakelists-txt
set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
