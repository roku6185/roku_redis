cmake_minimum_required(VERSION 3.2)
project(redis_client)

# Compile libredisclient.a
file(GLOB LIBREDISCLIENT_SRC_FILES *.c)
add_library(redisclient STATIC ${LIBREDISCLIENT_SRC_FILES})

# Compile test programs
set(EXECUTABLE_OUTPUT_PATH bin)
link_directories(.)
add_executable(main apps/main.c)
target_link_libraries(main redisclient)

# Compile unittests
set(EXECUTABLE_OUTPUT_PATH tests/bin)
add_subdirectory(unity)
link_directories(unity)
link_directories(.)
include_directories(.)
file(GLOB UNITTEST_SRC_FILES tests/*.c)

foreach(TEST_SRC ${UNITTEST_SRC_FILES})
  get_filename_component(TEST_BIN ${TEST_SRC} NAME_WE)
  add_executable(${TEST_BIN} ${TEST_SRC})
  target_link_libraries(${TEST_BIN} unity redisclient)
endforeach()

# Run unittests
add_custom_target(runtests)
file(GLOB TEST_BINARIES "tests/bin/*")

foreach(TEST_BIN ${TEST_BINARIES})
  add_custom_command(TARGET runtests COMMAND ${TEST_BIN})
endforeach()
