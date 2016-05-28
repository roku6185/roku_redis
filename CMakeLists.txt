cmake_minimum_required(VERSION 3.2)
project(redis_client)

# Compile libredisclient.a
file(GLOB LIBREDISCLIENT_SRC_FILES *.c)
add_library(redisclient STATIC ${LIBREDISCLIENT_SRC_FILES})

# Compile programs
file(GLOB APPS_SRC_FILES apps/*.c)

foreach(APP_SRC ${APPS_SRC_FILES})
  get_filename_component(APP_BIN ${APP_SRC} NAME_WE)
  add_executable(${APP_BIN} ${APP_SRC})
  set_target_properties(${APP_BIN}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "apps/bin"
  )
  target_link_libraries(${APP_BIN} redisclient)
endforeach()

# Compile unittests
add_subdirectory(unity)
link_directories(unity)
link_directories(.)
include_directories(.)
file(GLOB UNITTEST_SRC_FILES tests/*.c)

foreach(TEST_SRC ${UNITTEST_SRC_FILES})
  get_filename_component(TEST_BIN ${TEST_SRC} NAME_WE)
  add_executable(${TEST_BIN} ${TEST_SRC})
  set_target_properties(${TEST_BIN}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "tests/bin"
  )
  target_link_libraries(${TEST_BIN} unity redisclient)
endforeach()

# Run unittests
add_custom_target(runtests)
file(GLOB TEST_BINARIES "tests/bin/*")

foreach(TEST_BIN ${TEST_BINARIES})
  add_custom_command(TARGET runtests COMMAND ${TEST_BIN})
endforeach()
