# 说明

## 环境说明

需要安装GraphAr 

**注**: GraphAr安装过程会使用apache arrow-10 本地如果有安装apache arrow 会同步本地版本号 但是编译时还是从源代码下载使用 下载过程很漫长 可以通过log文件查看进度 如果报连接错误 需要手动下载移动到指定文件夹

GraphAr源码安装示例(ubuntu)

``` bash
    $ git clone https://github.com/alibaba/GraphAr.git
    $ cd GraphAr
    $ git submodule update --init
    $ cd cpp

    $ mkdir build-release
    $ cd build-release
    $ cmake ..
    $ make -j8

    $ sudo make install # make install
```

## 运行说明

``` bash
    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ ./show 
```

注意修改源码中的path路径，以及graph.yml中的prefix路径信息
