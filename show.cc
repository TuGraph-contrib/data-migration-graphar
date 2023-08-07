#include <iostream>

#include "arrow/api.h"

#include "./config.h"
#include "gar/graph.h"
#include "gar/graph_info.h"
#include "gar/reader/arrow_chunk_reader.h"
#include "gar/writer/arrow_chunk_writer.h"

int main(int argc, char* argv[]) {
  std::string path = "/home/jasin/neo4j2graphar/MovieGraph.graph.yml";

  auto graph_info = GAR_NAMESPACE::GraphInfo::Load(path).value();

  // 打印节点信息
  auto vertices_collection = GAR_NAMESPACE::ConstructVerticesCollection(graph_info, "Person");
  auto vertices = vertices_collection.value();

  for (auto it = vertices.begin(); it != vertices.end(); ++it) {
    auto vertex = *it;
    std::cout << "id=" << vertex.id()
              << ", name=" << vertex.property<std::string>("name").value()
              << std::endl;
  }

  // 打印边信息
  auto expect = GAR_NAMESPACE::ConstructEdgesCollection(
                    graph_info, "Person", "WROTE", "Movie",
                    GraphArchive::AdjListType::ordered_by_source)
                    .value();
  auto edges = std::get<GraphArchive::EdgesCollection<
      GraphArchive::AdjListType::ordered_by_source>>(expect);
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    auto edge = *it;                     
    std::cout << "src=" << edge.source() << ", dst=" << edge.destination() << std::endl;
  }
}