cmake_minimum_required(VERSION 3.16)
project(tests LANGUAGES C)

include_directories(.. ../metalang99/include)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic -std=c99
                      -ftrack-macro-expansion=0)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # Enable a standard-confirming C99/C11 preprocessor.
  add_compile_options("/std:c11")
endif()

add_subdirectory(extern_impl)

add_executable(basic_tests basic_tests.c common.h common.c util.h)
add_executable(decl_impl decl_impl.c common.h common.c)
add_executable(forward_typedefs forward_typedefs.c)
add_executable(impl_primary impl_primary.c common.h common.c)
add_executable(metalang99_compliant metalang99_compliant.c)
add_executable(superinterfaces superinterfaces.c common.h common.c util.h)

foreach(TARGET ${BUILDSYSTEM_TARGETS})
  set_target_properties(TARGET PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
endforeach()
