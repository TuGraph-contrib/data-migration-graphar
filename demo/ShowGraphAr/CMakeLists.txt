cmake_minimum_required(VERSION 3.15)

project(graph-archive-demo)

find_package(Threads REQUIRED)
add_definitions(-DGAR_NAMESPACE=GraphArchive)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

add_executable(show show.cpp)
target_link_libraries(show PRIVATE gar stdc++fs Threads::Threads
    "/root/GraphAr/cpp/build/arrow_ep-prefix/lib/libarrow.a"
    "/root/GraphAr/cpp/build/arrow_ep-prefix/lib/libarrow_dataset.a"
    "/root/GraphAr/cpp/build/arrow_ep-prefix/lib/libarrow_bundled_dependencies.a"
    "/root/GraphAr/cpp/build/arrow_ep-prefix/lib/libparquet.a"    
)
target_include_directories(show SYSTEM BEFORE PRIVATE /root/GraphAr/cpp/build/arrow_ep-prefix/include)