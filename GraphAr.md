# GraphAr集成

## 环境要求

1. 指定参数先安装arrow, 再安装GraphAr即可
2. 由于arrow编译cmake最低版本16, 所以将lgraph_import的cmake最低版本修改到16
3. arrow安装避免外部链接可以将相关文件上传到oss上

```bash
wget https://mirrors.ustc.edu.cn/apache/arrow/arrow-13.0.0/apache-arrow-13.0.0.tar.gz \
  && tar xf apache-arrow-13.0.0.tar.gz && cd apache-arrow-13.0.0/cpp && mkdir build && cd build \
  && cmake .. -DARROW_DATASET=ON -DARROW_PARQUET=ON -DARROW_ORC=ON -DARROW_CSV=ON && make -j6 \
  && make install && rm -rf /apache-arrow-*

wget https://tugraph-web.oss-cn-beijing.aliyuncs.com/tugraph/deps/graphar/GraphAr-0.8.0.tar.gz \
  && tar xf GraphAr-0.8.0.tar.gz && cd GraphAr-0.8.0/cpp && mkdir build && cd build \
  && cmake .. && make -j6 && make install
```

## 实现

在提出GraphAr依赖arrow下载问题后, 现在GraphAr可以根据系统中的arrow安装

现在GraphAr集成到tugraphdb中，需要两步

1. 声明GAR_NAMESPACE变量
2. target link包含gar

```cmake
add_definitions(-DGAR_NAMESPACE=GraphArchive)

target_link_libraries(${TARGET_LGRAPH_IMPORT}
        lgraph_server_lib
        lgraph_cypher_lib
        lgraph_server_lib
        librocksdb.a)
```
