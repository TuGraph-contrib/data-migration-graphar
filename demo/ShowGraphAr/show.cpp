#include <iostream>
#include <filesystem>
#include <gar/graph.h>
#include <gar/graph_info.h>

int main(int argc, char *argv[])
{
    // construct graph information from file
    std::string demo_path = std::filesystem::current_path().parent_path().parent_path();
    std::string path = demo_path + "/ldbc_sample/parquet/ldbc_sample.graph.yml";
    auto graph_info = GraphArchive::GraphInfo::Load(path).value();

    // get vertex information
    auto vertices = GraphArchive::VerticesCollection::Make(graph_info, "person").value();

    for (auto it = vertices->begin(); it != vertices->end(); ++it)
    {
        // get a vertex and access its data
        auto vertex = *it;
        std::cout << "id=" << vertex.property<int64_t>("id").value() << ", firstName=" << vertex.property<std::string>("firstName").value() << std::endl;
    }

    // get edge information
    auto edge_info = graph_info->GetEdgeInfo("person", "knows",
                                             "person");
    auto expect = GraphArchive::EdgesCollection::Make(graph_info, "person", "knows",
                                                      "person",
                                                      GraphArchive::AdjListType::ordered_by_source);
    auto edges = expect.value();

    for (auto it = edges->begin(); it != edges->end(); ++it)
    {
        // get an edge and access its data
        auto edge = *it;
        std::cout << "src=" << edge.source() << ", dst=" << edge.destination() << std::endl;
    }
}