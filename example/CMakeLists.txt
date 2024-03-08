cmake_minimum_required(VERSION 3.8)

project("example")

include_directories(${CMAKE_SOURCE_DIR}/flow_editor/include)
include_directories(${3rdparty})

add_subdirectory(ex_101_show_view)
add_subdirectory(ex_102_random_view)
add_subdirectory(ex_103_dynamic_node)
