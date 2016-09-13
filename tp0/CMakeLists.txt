cmake_minimum_required(VERSION 3.5)
project(tp0)

set(ROOT_PATH "${${PROJECT_NAME}_SOURCE_DIR}")
set(TEST_PATH "${ROOT_PATH}/test/")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

set(SOURCE_FILES src/main.c)
add_executable(tp0 ${SOURCE_FILES})
set_property(TARGET tp0 PROPERTY C_STANDARD 99)


target_link_libraries(${PROJECT_NAME})
target_link_libraries(tp0 m)

