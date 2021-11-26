cmake_minimum_required(VERSION 3.16)
project(examples LANGUAGES C)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic -ftrack-macro-expansion=0
                      -fsanitize=address)
  add_link_options(-fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-fmacro-backtrace-limit=1 -fsanitize=address)
  add_link_options(-fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # Enable a standard-conforming C99/C11 preprocessor.
  add_compile_options("/std:c11")
elseif(CMAKE_C_COMPILER_ID STREQUAL "TinyCC")
  add_compile_definitions(ML99_ALLOW_POOR_DIAGNOSTICS)
endif()

add_subdirectory(opaque_type)

add_executable(shape shape.c)
add_executable(tracing_vehicle tracing_vehicle.c)
add_executable(airplane airplane.c)
add_executable(read_write read_write.c)
add_executable(read_write_both read_write_both.c)
add_executable(marker marker.c)
add_executable(default_impl default_impl.c)

add_subdirectory(.. build)

get_property(
  EXAMPLES
  DIRECTORY .
  PROPERTY BUILDSYSTEM_TARGETS)

get_property(
  OPAQUE_TYPE_EXAMPLE
  DIRECTORY opaque_type
  PROPERTY BUILDSYSTEM_TARGETS)

foreach(TARGET ${EXAMPLES};${OPAQUE_TYPE_EXAMPLE})
  target_link_libraries(${TARGET} interface99)
  set_target_properties(${TARGET} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED
                                                           ON)
endforeach()
