cmake_minimum_required(VERSION 3.15)

project(graph-archive-demo)

add_definitions(-DGAR_NAMESPACE=GraphArchive)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

set(TARGET "show")

find_package(gar REQUIRED)

add_executable(${TARGET} ${TARGET}.cpp)
target_link_libraries(${TARGET} PRIVATE gar stdc++fs)