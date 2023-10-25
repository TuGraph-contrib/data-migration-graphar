#include <iostream>
#include <filesystem>
#include <arrow/api.h>
#include <gar/graph.h>
#include <gar/graph_info.h>
#include <gar/reader/arrow_chunk_reader.h>
#include <gar/writer/arrow_chunk_writer.h>

int main(int argc, char *argv[])
{
  // graph.yaml
  std::string demo_path = std::filesystem::path(__FILE__).parent_path().parent_path()
  std::string path = demo_path + "/ldbc_sample/parquet/ldbc_sample.graph.yml";
  auto graph_info = GAR_NAMESPACE::GraphInfo::Load(path).value();

  // arrow
  std::string label = "person", property_name = "firstName";
  auto group = graph_info.GetVertexPropertyGroup(label, property_name).value();
  auto maybe_reader = GAR_NAMESPACE::ConstructVertexPropertyArrowChunkReader(
      graph_info, label, group);
  const auto chunk_size = graph_info.GetVertexInfo(label)->GetChunkSize();
  auto reader = maybe_reader.value();
  auto walkReader = [&](GAR_NAMESPACE::VertexPropertyArrowChunkReader &reader)
  {
    int idx = 0, sum = 0;
    std::shared_ptr<arrow::Table> table;

    do
    {
      auto result = reader.GetChunk();
      table = result.value();
      auto [start, end] = reader.GetRange().value();
      std::cout << "Chunk: " << idx << ",\tNums: " << table->num_rows() << "/"
                << chunk_size << ",\tRange: (" << start << ", " << end << "]"
                << '\n';
      idx++;
      sum += table->num_rows();
    } while (!reader.next_chunk().IsIndexError());

    std::cout << "Total Nums: " << sum << "/"
              << reader.GetChunkNum() * chunk_size << '\n';
    std::cout << "Column Nums: " << table->num_columns() << "\n";
    std::cout << "Column Names: ";
    for (int i = 0; i < table->num_columns(); i++)
    {
      std::cout << "`" << table->ColumnNames()[i] << "` ";
    }
    std::cout << "\n\n";
  };
  walkReader(reader);

  // vertex
  auto vertices_collection = GAR_NAMESPACE::ConstructVerticesCollection(graph_info, "person").value();

  for (auto it = vertices_collection.begin(); it != vertices_collection.end(); ++it)
  {
    auto vertex = *it;
    std::cout << "id=" << vertex.id()
              << ", firstName=" << vertex.property<std::string>("firstName").value()
              << std::endl;
  }

  // edge
  auto edge_collection = GAR_NAMESPACE::ConstructEdgesCollection(
                             graph_info, "person", "knows", "person",
                             GraphArchive::AdjListType::ordered_by_source)
                             .value();
  auto edges = std::get<GraphArchive::EdgesCollection<
      GraphArchive::AdjListType::ordered_by_source>>(edge_collection);
  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    auto edge = *it;
    std::cout << "src=" << edge.source() << ", dst=" << edge.destination() << std::endl;
  }
}