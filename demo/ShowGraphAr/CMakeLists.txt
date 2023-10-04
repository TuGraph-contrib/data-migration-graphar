cmake_minimum_required(VERSION 3.15)

project(graph-archive-demo)

find_package(Threads REQUIRED)
add_definitions(-DGAR_NAMESPACE=GraphArchive)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

set(TARGET "show")

find_package(Arrow REQUIRED)
find_package(ArrowDataset REQUIRED)
find_package(ArrowAcero REQUIRED)
find_package(Parquet REQUIRED)
find_package(gar REQUIRED)
find_package(Threads REQUIRED)

add_executable(${TARGET} ${TARGET}.cpp)
target_link_libraries(${TARGET} PRIVATE gar stdc++fs Threads::Threads 
    Arrow::arrow_static
    Parquet::parquet_static
    ArrowDataset::arrow_dataset_static
    ArrowAcero::arrow_acero_static)