# 说明

## 环境说明

1. 需要安装GraphAr, 其中show程序需要安装arrow
2. 下载数据

```shell
git clone https://github.com/GraphScope/gar-test.git
```

## 运行说明

cmake指定target参数, 默认config

``` bash
    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ ./show
```

1. config展示从Gar中生成json对象用于解析schema
2. show展示数据, 分别展示arrow chunk, 点, 边
3. primary展示获取主键和数据转化构造

注意修改源码中的path路径，以及graph.yml中的prefix路径信息
