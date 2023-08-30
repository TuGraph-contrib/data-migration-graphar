#include <iostream>
#include "gar/graph.h"
#include "gar/graph_info.h"
#include "json.hpp"
#include <map>
#include <utility>

typedef std::map<std::string, std::string> Dict;

void ParseType(const GAR_NAMESPACE::DataType data_type, std::string &type_name)
{
  switch (data_type.id())
  {
  case GAR_NAMESPACE::Type::BOOL:
    type_name = "BOOL";
    break;
  case GAR_NAMESPACE::Type::INT32:
    type_name = "INT32";
    break;
  case GAR_NAMESPACE::Type::INT64:
    type_name = "INT64";
    break;
  case GAR_NAMESPACE::Type::FLOAT:
    type_name = "FLOAT";
    break;
  case GAR_NAMESPACE::Type::DOUBLE:
    type_name = "DOUBLE";
    break;
  case GAR_NAMESPACE::Type::STRING:
    type_name = "STRING";
    break;
  default:
    break;
  }
}

void CheckAdjListType(const GAR_NAMESPACE::EdgeInfo &edge_info, GAR_NAMESPACE::AdjListType &adj_list_type)
{
  for (std::uint8_t i = 0; i <= static_cast<std::uint8_t>(GAR_NAMESPACE::AdjListType::ordered_by_dest); ++i)
  {
    GAR_NAMESPACE::AdjListType type = static_cast<GAR_NAMESPACE::AdjListType>(i);
    if (edge_info.ContainAdjList(type))
      adj_list_type = type;
  }
}

void WalkVertex(const GAR_NAMESPACE::VertexInfo &ver_info, std::string &primary, std::vector<Dict> &props, std::vector<std::string> &prop_names)
{
  auto ver_groups = ver_info.GetPropertyGroups();
  for (auto ver_props : ver_groups)
  {
    for (auto prop : ver_props.GetProperties())
    {
      if (prop.is_primary)
        primary = prop.name;
      prop_names.emplace_back(prop.name);
      std::string type_name;
      ParseType(prop.type, type_name);
      props.emplace_back(std::map<std::string, std::string>{
          {"name", prop.name},
          {"type", type_name}});
    }
  }
}

void WalkEdge(const GAR_NAMESPACE::EdgeInfo &edge_info, std::vector<Dict> &props, std::vector<std::string> &prop_names)
{
  GAR_NAMESPACE::AdjListType type;
  CheckAdjListType(edge_info, type);
  auto edge_groups = edge_info.GetPropertyGroups(type).value();
  for (auto edge_props : edge_groups)
  {
    for (auto prop : edge_props.GetProperties())
    {
      prop_names.emplace_back(prop.name);
      std::string type_name;
      ParseType(prop.type, type_name);
      props.emplace_back(std::map<std::string, std::string>{
          {"name", prop.name},
          {"type", type_name}});
    }
  }
}

int main()
{
  std::string path = "./gar-test/ldbc_sample/parquet/ldbc_sample.graph.yml";
  auto graph_info = GAR_NAMESPACE::GraphInfo::Load(path).value();

  nlohmann::json json_conf;
  json_conf["schema"] = {};
  json_conf["files"] = {};
  auto vertex_infos = graph_info.GetVertexInfos();
  for (const auto &[key, value] : vertex_infos)
  {
    nlohmann::json schema_node;
    schema_node["label"] = value.GetLabel();
    schema_node["type"] = "VERTEX";
    std::string primary;
    std::vector<Dict> properties;
    std::vector<std::string> prop_names;
    WalkVertex(value, primary, properties, prop_names);
    schema_node["primary"] = primary;
    schema_node["properties"] = properties;
    json_conf["schema"].push_back(schema_node);

    nlohmann::json file_node;
    file_node["path"] = path;
    file_node["format"] = "GraphAr";
    file_node["label"] = value.GetLabel();
    file_node["columns"] = prop_names;
    json_conf["files"].push_back(file_node);
  }

  auto edge_infos = graph_info.GetEdgeInfos();
  for (const auto &[key, value] : edge_infos)
  {
    nlohmann::json schema_node;
    schema_node["label"] = value.GetEdgeLabel();
    schema_node["type"] = "EDGE";
    std::vector<Dict> properties;
    std::vector<std::string> prop_names = {"SRC_ID", "DST_ID"};
    WalkEdge(value, properties, prop_names);
    schema_node["properties"] = properties;
    json_conf["schema"].push_back(schema_node);

    nlohmann::json file_node;
    file_node["path"] = path;
    file_node["format"] = "GraphAr";
    file_node["label"] = value.GetEdgeLabel();
    file_node["SRC_ID"] = value.GetSrcLabel();
    file_node["DST_ID"] = value.GetDstLabel();
    file_node["columns"] = prop_names;
    json_conf["files"].push_back(file_node);
  }

  std::string json_str = json_conf.dump();
  std::cout << json_str << std::endl;
}