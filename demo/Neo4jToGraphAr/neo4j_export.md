# neo4j导出流程记录

## 利用spark将数据在neo4j和GraphAr之间导入导出

### 环境搭建

Linux 安装git curl java-11和maven, maven配置文件加入mirror镜像

### 项目安装

拉取GraphAr源代码，编译spark目录下的maven项目, 得到`graphar-0.1.0-SNAPSHOT-shaded.jar`文件

下载neo4j-4.4.x, spark-3.2.4, 添加环境变量

命令如下:

```bash
git clone https://github.com/alibaba/GraphAr.git
cd GraphAr
git submodule update --init
cd spark
mvn clean package -DskipTests  # 编译不包含测试的代码 确保maven已经有镜像

curl https://mirrors.tuna.tsinghua.edu.cn/apache/spark/spark-3.2.4/spark-3.2.4-bin-hadoop3.2.tgz | tar -xz -C ${HOME}/
export SPARK_HOME="${HOME}/spark-3.2.4-bin-hadoop3.2"  
export PATH="${SPARK_HOME}/bin":"${PATH}"

curl https://dist.neo4j.org/neo4j-community-4.4.23-unix.tar.gz | tar -xz -C ${HOME}/
export NEO4J_HOME="${HOME}/neo4j-community-4.4.23"
export PATH="${NEO4J_HOME}/bin":"${PATH}"

# 环境变量也可以加入.bashrc中
```

### 运行neo4j导出到GraphAr的示例

neo4j的连接信息已经被写在源代码中, 通过环境变量配置neo4j账号密码

加载开源数据集`movies-43.dump`文件, 启动neo4j
导入GraphAr的脚本`scripts/run-neo4j2graphar.sh`已经写好, 里面有一些配置信息, 包含**chunk_size**和**file_type**，用**spark_submit**工具运行

导入的文件存储在`/tmp/graphar/neo4j2graphar`中, 可以在脚本文件中修改导入文件地址

```bash
export NEO4J_USR="neo4j"
export NEO4J_PWD="neo4j"

curl https://raw.githubusercontent.com/neo4j-graph-examples/movies/main/data/movies-43.dump -o ${NEO4J_HOME}/movies-43.dump
neo4j-admin load --from ${NEO4J_HOME}/movies-43.dump --database=neo4j
neo4j start

scripts/run-neo4j2graphar.sh
```

### 运行GraphAr导出到neo4j的示例

用cypher_shell清空已有数据

运行导出文件脚本`scripts/run-graphar2neo4j.sh`

```bash
echo "match (a) -[r] -> () delete a, r;match (a) delete a;" | cypher-shell -u ${NEO4J_USR} -p ${NEO4J_PWD} -d neo4j --format plain

scripts/run-graphar2neo4j.sh
```

## 总结

### 拓展

可以修改和拓展的地方, 可以编写自己的Scala文件编译好给spark_submit运行, 设置class入口即可，也可以参考test进行测试

```bash
# 代码待补充
```

### Next

得到的文件有一个总的结构信息yaml文件，可以用spark和c++解析，下一步可以接着这个c++接口以及spark接口继续往下做
