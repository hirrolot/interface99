cmake_minimum_required(VERSION 3.16)
project(examples LANGUAGES C)

include_directories(.. ../metalang99/include)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic -std=c99
                      -ftrack-macro-expansion=0)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_compile_options("-fmacro-backtrace-limit=1")
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  # Enable a standard-conforming C99/C11 preprocessor.
  add_compile_options("/std:c11")
elseif(CMAKE_C_COMPILER_ID STREQUAL "TinyCC")
  add_compile_definitions(ML99_ALLOW_POOR_DIAGNOSTICS)
endif()

add_subdirectory(opaque_type)

add_executable(state state.c)
add_executable(trace_state trace_state.c)
add_executable(airplane airplane.c)
add_executable(read_write read_write.c)
add_executable(read_write_both read_write_both.c)
add_executable(media_stream media_stream.c)
add_executable(marker marker.c)

foreach(TARGET ${BUILDSYSTEM_TARGETS})
  set_target_properties(TARGET PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
endforeach()
