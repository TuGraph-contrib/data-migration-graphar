# TuGraph数据迁移项目提案

## 方案调研

### 其他图数据库导入导出工具调研

**目的**：了解较完善的图数据库对其他图数据库数据导入导出的相关工具，参考相关项目构思TuGraph可以采取的方案

**结果**：总结表格如下

| 图数据库    | 图数据库导入导出or转换迁移工具 | connector支持  |
| ----------- | ------------------------------ | -------------- |
| neo4j       | 未发现                         | spark          |
| NebulaGraph | Exchange(Spark应用)            | spark、flink   |
| OrientDB    | orientdb-neo4j-importer        | 未发现         |
| JanusGraph  | 未发现                         | Hadoop-Gremlin |
| ArangoDB    | 未发现                         | spark          |

**思考**：

1. 对于NebulaGraph来说，Exchange工具的Reader和Writer工具对图数据库导入这一块非常方便，可以统一各种数据库的导入，同时具有更好的导入流程，可以作为一个feature
2. 大部分图数据库都支持spark的连接，有一定的工具解析成DataFrame对象，TuGraph可以考虑参考也做一个spark connector, 接入大数据生态

### GraphAr调研

**目的**：GraphAr目的就是作为图数据结构数据中间存储格式, 参考GraphAr的例子和接口将gar数据导入到tugraph中

**结果**：编译GraphAr项目，运行相关例子，打通了neo4j图数据库的导入导出流程，了解了neo4j spark connector工具，以及GraphAr对neo4j数据的解析流程，可以参考其中的c++接口解析数据导入到TuGraph

### 实现方案调研

**目的**：目前已有方案三种，可以调研实现难度和大致情况

1. 基于GraphAr项目，根据相关c++接口，做相关数据导入Tugraph的工具，类似lgraph_import
2. 参考其他图数据库实现方式，实现一个TuGraph的Spark Connector
3. 基于DataX项目，做相关工具支持图数据库数据通过DataX导入TuGraph

**结果**：方案调研如下, 最终选择基于GraphAr进行开发

| 方案 | GraphAr                                                                                    | Spark Connector                                                                           | DataX                                                        |
| ---- | ------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------- | ------------------------------------------------------------ |
| 语言 | C++                                                                                        | Scala/Java                                                                                | Java                                                         |
| 优点 | GraphAr已经部分支持neo4j导入, 并且提供了c++接口, TuGraph可以直接利用相关工具，开发周期较快 | 丰富TuGraph工具集，可以统一到大数据平台，开发更多功能                                     | 已有TuGraph reader实现，DataX支持neo4j导出，插件开发文档详细 |
| 缺点 | GraphAr生态还不够完善, 对gar数据的解析存在一定的overhead                                   | connector工具较大，需要从零开始实现，开发周期可能较长，可能设计到技术选型问题，未知性较大 | DataX对图数据库支持不够完善，作为一个导入导出工具可能比较重  |

## 导入工具实现设计

### 改进importer

外部图数据源统一到GraphAr数据中, 同时拓展lgraph_import工具的GraphAr parser功能, 实现lgraph_import可以直接导入gar数据, 并且实现以下功能

- 自动解析GraphAr配置生成schema
- 根据主键作为端点构建边
- 误数据和配置报错提示
