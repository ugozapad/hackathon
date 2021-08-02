cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(engine)

include(cmake/common.cmake)

# add vendor dir
add_subdirectory(vendor)

# add engine dir
add_subdirectory(src)
