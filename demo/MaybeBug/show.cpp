#include <iostream>
#include <filesystem>
#include <gar/graph.h>
#include <gar/graph_info.h>

int main(int argc, char *argv[])
{
    // construct graph information from file
    std::string path = "/root/demo/edge_test/movie.graph.yml"; // the path of the graph information file (e.g., ldbc_sample.graph.yml)
    auto graph_info = GraphArchive::GraphInfo::Load(path).value();
    auto vertices = GraphArchive::VerticesCollection::Make(graph_info, "person").value();
    for (auto it = vertices->begin(); it != vertices->end(); ++it)
    {
        // get a vertex and access its data
        auto vertex = *it;
        std::cout << "id=" << vertex.property<bool>("id").value() << std::endl;
    }
}