# Find and add the cmocka library
find_library(CMOCKA_LIBRARY NAMES cmocka)
add_library(cmocka SHARED IMPORTED)
set_property(TARGET cmocka PROPERTY IMPORTED_LOCATION "${CMOCKA_LIBRARY}")

add_executable(main main.c)
target_link_libraries(main cmocka)

add_test(main ${CMAKE_CURRENT_BINARY_DIR}/main)