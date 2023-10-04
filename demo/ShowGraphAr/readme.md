# 说明

## 环境说明

1. 需要安装GraphAr, 同时show程序需要安装arrow
2. 数据在gar-test子模块中

## 运行说明

``` bash
$ mkdir build && cd build
$ cmake ..
$ make
$ ./show
```

1. show展示数据, 分别展示arrow chunk数据, 点, 边
2. 注意源码中的path路径为绝对路径

## 附录

### arrow安装

注意编译选项

```bash
wget https://tugraph-web.oss-cn-beijing.aliyuncs.com/tugraph/deps/graphar/apache-arrow-13.0.0.tar.gz \
  && tar xf apache-arrow-13.0.0.tar.gz && cd apache-arrow-13.0.0/cpp && mkdir build && cd build \
  && cmake .. -DARROW_DATASET=ON -DARROW_PARQUET=ON -DARROW_ORC=ON -DARROW_CSV=ON && make \
  && make install 
```

### gar安装(依赖系统安装arrow)

```bash
wget https://tugraph-web.oss-cn-beijing.aliyuncs.com/tugraph/deps/graphar/GraphAr-0.8.0.tar.gz \
  && tar xf GraphAr-0.8.0.tar.gz && cd GraphAr-0.8.0/cpp && mkdir build && cd build \
  && cmake .. && make && make install
```
