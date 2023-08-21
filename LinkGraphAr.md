# GraphAr链接

下载GraphAr源码，编译cpp文件之后，install会将libgar.so共享库和gar头文件安装到系统中，但是arrow的头文件和lib目录下的静态库没有安装到系统中，编写项目需要手动链接，或者手动安装，不然会报错

目前tugraph-compile的docker环境中, 完整使用GraphAr, cmake配置至少需要包含以下部分

```cmake
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
```

GraphAr编译后打包移动，build目录下cmake和make相关配置文件中的路径都是绝对路径，目前只有在相同系统和相同路径情况下可以复用
