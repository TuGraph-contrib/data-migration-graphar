cmake_minimum_required(VERSION 3.15)

project(graph-archive-demo)

find_package(Threads REQUIRED)
add_definitions(-DGAR_NAMESPACE=GraphArchive)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

set(TARGET "config")

add_executable(${TARGET} ${TARGET}.cpp)
target_link_libraries(${TARGET} PRIVATE gar stdc++fs Threads::Threads)