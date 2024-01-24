# 默认示例运行说明

GraphAr提供了一些shell脚本, 可以安装neo4j, spark, 以及运行导入程序

下面是一个运行示例

```bash
cd GraphAr/spark
scripts/get-spark-to-home.sh
export SPARK_HOME="${HOME}/spark-3.2.2-bin-hadoop3.2"
export PATH="${SPARK_HOME}/bin":"${PATH}"

scripts/get-neo4j-to-home.sh
export NEO4J_HOME="${HOME}/neo4j-community-4.4.23"
export PATH="${NEO4J_HOME}/bin":"${PATH}"
neo4j-admin set-initial-password neo4j

scripts/deploy-neo4j-movie-data.sh

scripts/build.sh

export NEO4J_USR="neo4j"
export NEO4J_PWD="neo4j"

cd import
./neo4j.sh neo4j.json
```
