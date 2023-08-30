#include <iostream>
#include <filesystem>
#include <chrono>
#include "gar/graph.h"
#include "gar/graph_info.h"

struct GarData
{
    union
    {
        bool boolean;
        int8_t int8;
        int16_t int16;
        int32_t int32;
        int64_t int64;
        float sp;
        double dp;
        std::string *buf;
    } data;
    GAR_NAMESPACE::Type type;

    std::string value;

    GarData() {}

    GarData(GAR_NAMESPACE::Vertex &vertex, const std::string &prop,
            const GAR_NAMESPACE::DataType &data_type)
    {
        std::string buf;
        type = data_type.id();
        switch (data_type.id())
        {
        case GAR_NAMESPACE::Type::STRING:
            buf = vertex.property<std::string>(prop).value();
            data.buf = new std::string(buf);
            value = buf;
            break;
        case GAR_NAMESPACE::Type::BOOL:
            data.boolean = vertex.property<bool>(prop).value();
            value = std::to_string(vertex.property<bool>(prop).value());
            break;
        case GAR_NAMESPACE::Type::INT32:
            data.int32 = vertex.property<int32_t>(prop).value();
            value = std::to_string(vertex.property<int32_t>(prop).value());
            break;
        case GAR_NAMESPACE::Type::INT64:
            data.int64 = vertex.property<int64_t>(prop).value();
            value = std::to_string(vertex.property<int64_t>(prop).value());
            break;
        case GAR_NAMESPACE::Type::FLOAT:
            data.sp = vertex.property<float>(prop).value();
            value = std::to_string(vertex.property<float>(prop).value());
            break;
        case GAR_NAMESPACE::Type::DOUBLE:
            data.dp = vertex.property<double>(prop).value();
            value = std::to_string(vertex.property<double>(prop).value());
            break;
        case GAR_NAMESPACE::Type::USER_DEFINED:
            // todo
            break;
        default:
            break;
        }
    }

    GarData(GAR_NAMESPACE::Edge &vertex, const std::string &prop,
            const GAR_NAMESPACE::DataType &data_type)
    {
        std::string buf;
        type = data_type.id();
        switch (data_type.id())
        {
        case GAR_NAMESPACE::Type::STRING:
            buf = vertex.property<std::string>(prop).value();
            data.buf = new std::string(buf);
            value = buf;
            break;
        case GAR_NAMESPACE::Type::BOOL:
            data.boolean = vertex.property<bool>(prop).value();
            value = std::to_string(vertex.property<bool>(prop).value());
            break;
        case GAR_NAMESPACE::Type::INT32:
            data.int32 = vertex.property<int32_t>(prop).value();
            value = std::to_string(vertex.property<int32_t>(prop).value());
            break;
        case GAR_NAMESPACE::Type::INT64:
            data.int64 = vertex.property<int64_t>(prop).value();
            value = std::to_string(vertex.property<int64_t>(prop).value());
            break;
        case GAR_NAMESPACE::Type::FLOAT:
            data.sp = vertex.property<float>(prop).value();
            value = std::to_string(vertex.property<float>(prop).value());
            break;
        case GAR_NAMESPACE::Type::DOUBLE:
            data.dp = vertex.property<double>(prop).value();
            value = std::to_string(vertex.property<double>(prop).value());
            break;
        case GAR_NAMESPACE::Type::USER_DEFINED:
            // todo
            break;
        default:
            break;
        }
    }
};

void get_primary_key(const GAR_NAMESPACE::VertexInfo &ver_info, GAR_NAMESPACE::Property &ver_prop)
{
    auto ver_groups = ver_info.GetPropertyGroups();
    for (auto ver_props : ver_groups)
        for (auto prop : ver_props.GetProperties())
            if (prop.is_primary) {
                ver_prop = prop;
                return;
            }

}

void MapIdPrimary(std::unordered_map<GAR_NAMESPACE::IdType, GarData> &primary_map,
                  const GAR_NAMESPACE::GraphInfo& graph_info, const std::string& ver_label)
{
    auto ver_info = graph_info.GetVertexInfo(ver_label).value();
    GAR_NAMESPACE::Property ver_prop;
    auto ver_prop = get_primary_key(ver_info, ver_prop);
    auto vertices = 
        GAR_NAMESPACE::ConstructVerticesCollection(graph_info, ver_label).value();
    for (auto vertex : vertices)
    {
        primary_map[vertex.id()] = GarData(vertex, ver_prop.name, ver_prop.type);
    }
}

int main()
{
    std::string path = "./gar-test/ldbc_sample/parquet/ldbc_sample.graph.yml";
    auto graph_info = GAR_NAMESPACE::GraphInfo::Load(path).value();

    std::string src_label = "person";
    std::string edge_label = "knows";
    std::string dst_label = "person";
    std::vector<std::string> edge_props = {"creationDate"};

    auto edge_collection = GAR_NAMESPACE::ConstructEdgesCollection(
                               graph_info, src_label, edge_label, dst_label,
                               GAR_NAMESPACE::AdjListType::ordered_by_source)
                               .value();

    auto edges = std::get<GAR_NAMESPACE::EdgesCollection<
        GAR_NAMESPACE::AdjListType::ordered_by_source>>(edge_collection);
    auto edge_info =
        graph_info.GetEdgeInfo(src_label, edge_label, dst_label).value();
    size_t edge_num = edges.size();

    std::unordered_map<GAR_NAMESPACE::IdType, GarData> src_id_primary;
    std::unordered_map<GAR_NAMESPACE::IdType, GarData> dst_id_primary;
    MapIdPrimary(src_id_primary, graph_info, src_label);
    MapIdPrimary(dst_id_primary, graph_info, dst_label);

    std::vector<std::vector<GarData>> block;
    block.reserve(edge_num);
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
        auto edge = *it;

        auto src_data = src_id_primary[edge.source()];
        auto dst_data = dst_id_primary[edge.destination()];
        std::vector<GarData> temp_vf = {src_data, dst_data};

        for (auto prop : edge_props)
        {
            auto data_type = edge_info.GetPropertyType(prop).value();
            auto edge_data = GarData(edge, prop, data_type);
            temp_vf.emplace_back(edge_data);
        }
        block.emplace_back(temp_vf);
    }
    for (auto vf : block) {
        for (auto gd : vf) {
            std::cout << gd.value << std::endl;
        }
    }
}