# TuGraph数据迁移项目提案

**摘要** 项目提案根据任务细分进行拆解，生成粒度较细的解决方案和执行计划，并给出里程碑式小任务完成日期表

## 任务前提

- [x] 在系统上搭建开发环境
- [x] 安装软件，运行项目，运行demo，熟悉TuGraph基本使用
- [x] fork项目git，打通贡献流程

## 任务一 支持图数据库导入导出

### 展开GraphAr调研

**目的**：熟悉GraphAr对图数据库neo4j导入导出的工作流程，参考GraphAr的例子来设计导入导出工具
**结果**：编译GraphAr项目，运行相关例子，打通了neo4j图数据库的导入导出流程，了解了neo4j spark connector工具，以及GraphAr对neo4j数据的解析流程，总结成文档，可以参考其中的c++接口解析数据导入到TuGraph

### 其他图数据库导入导出工具调研

**目的**：了解较完善的图数据库对其他图数据库数据导入导出的相关工具，参考相关项目构思TuGraph可以采取的方案
**结果**：总结表格如下

| 图数据库    | 图数据库导入导出or转换迁移工具 | connector支持  | 备注   |
| ----------- | ------------------------------ | -------------- | ------ |
| neo4j       | 未发现                         | spark          | 已实践 |
| NebulaGraph | Exchange(Spark应用)            | spark、flink   | 未实践 |
| OrientDB    | orientdb-neo4j-importer        | 未发现         | 未实践 |
| JanusGraph  | 未发现                         | Hadoop-Gremlin | 未实践 |
| ArangoDB    | 未发现                         | spark          | 未实践 |

**思考**：

1. 对于NebulaGraph来说，Exchange工具的Reader和Writer工具对图数据库导入这一块非常方便，可以统一各种数据库的导入，同时可以更好地导入流程，可以作为一个feature
2. 大部分图数据库都支持spark的连接，有一定的工具解析成DataFrame对象，TuGraph可以考虑参考也做一个spark connector

### 实现方案调研

**目的**：目前已有方案三种，可以调研实现难度和大致情况，然后根据自己能力进行选择

1. 基于GraphAr项目，根据相关c++接口，做相关数据导入Tugraph的工具，类似lgraph_import
2. 参考其他图数据库实现方式，实现一个TuGraph的Spark Connector
3. 基于DataX项目，做相关工具支持图数据库数据通过DataX导入TuGraph

**结果**：方案调研如下

| 方案 | GraphAr                                                                           | Spark Connector                                                                           | DataX                                                        |
| ---- | --------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------------------------------------------------------ |
| 语言 | C++                                                                               | Scala/Java                                                                                | Java                                                         |
| 优点 | 已有部分相关经验,相关问题可以答疑，TuGraph可以直接利用相关工具，开发周期较快      | 丰富TuGraph工具集，可以统一到大数据平台，开发更多功能                                     | 已有TuGraph reader实现，DataX支持neo4j导出，插件开发文档详细 |
| 缺点 | CMAKE，gtest不熟，大型c++项目经验不足，需要花时间学习，开发过程可能会抛出较多问题 | connector工具较大，需要从零开始实现，开发周期可能较长，可能设计到技术选型问题，未知性较大 | DataX对图数据库支持不够完善，作为一个导入导出工具可能比较重  |

**思考**：三种方案对于我来说，最难的都是开发环境的搭建，方案选取应该能够尽快启动，搭好写代码架子，其中的语言逻辑和代码的编写应该没有问题。经过综合评估，优先选择GraphAr方案。

### GraphAr方案讨论

**目的**: 根据自己技术栈和时间, 制定GraphAr导出到TuGraph的实现方案
**前置条件**:

1. 补充学习cmake, 可以阅读CmakeLists.txt, 会复用以及编写cmake语句
2. 跑通GraphAr c++ example, 能编写自己的demo程序
3. 阅读TuGraph importer源码, 总结自己的理解和问题

**讨论**:

1. demo展示 + importer理解
2. 自己遇到的问题
3. 实现方案讨论

**结果**：阅读v3源码，参考v3的csv parser和jsonline parser制作一个graphar的parser

## TimeLine

时间线以及任务总结

| 时间        | 内容                             | 里程碑                                                 |
| ----------- | -------------------------------- | ------------------------------------------------------ |
| 6.28-7.7    | 研究数据库相关文档, 制定基础方案 | ✔️完成[任务前提](#任务前提)                             |
| 7.7 双周会  | 评估方案, 详细讲解任务           | 调整任务顺序                                           |
| 7.7-7.12    | 使用GraphAr, 总结问题            | ✔️完成[GraphAr调研](#展开graphar调研)                   |
| 7.12 交流会 | 探讨GraphAr问题                  | 建立交流群                                             |
| 7.12-7.21   | 调研相关工具                     | ✔️完成[其他图数据库调研](#其他图数据库导入导出工具调研) |
| 7.21 双周会 | 细化方案, 整合proposal           |                                                        |
| 7.21-7.25   | 调研详细方案                     | ✔️完成[细化方案调研](#实现方案调研)                     |
| 7.25-8.4    | 转战c++, 学习cmake               |                                                        |
| 8.4 双周会  | 评估进度, 请教阅读源码方式       |                                                        |
| 8.4-8.7     | 优化demo, 读懂impoter, 准备讨论  |                                                        |
| 8.7         | 讨论方案                         | ✔️完成[GraphAr方案讨论](#graphar方案讨论)               |