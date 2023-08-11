cmake_minimum_required(VERSION 3.15)

set(GAR_MAJOR_VERSION 0)
set(GAR_MINOR_VERSION 1)
set(GAR_PATCH_VERSION 0)
set(GAR_VERSION ${GAR_MAJOR_VERSION}.${GAR_MINOR_VERSION}.${GAR_PATCH_VERSION})

project(graph-archive-demo LANGUAGES C CXX VERSION ${GAR_VERSION})

add_definitions(-DGAR_NAMESPACE=GraphArchive)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

add_executable(show show.cpp)
target_link_libraries(show PRIVATE gar stdc++fs)