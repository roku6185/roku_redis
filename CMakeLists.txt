cmake_minimum_required(VERSION 3.2)
project(redis_client)
file(GLOB PROJECT_C_FILES *.c)
add_executable(main2 ${PROJECT_C_FILES} test/main2.c)