//
// Created by cxc on 2024/3/11.
//

#pragma once
#include <src/items/node_item.hpp>
#include <src/items/in_node_item.hpp>
#include <src/items/out_node_item.hpp>
#include <src/items/connection/connection_item.hpp>
#include <unordered_map>

namespace fe
{
class NodeItemData
{
public:
    NodeData data;
    std::unique_ptr<DynamicHGeometry> geometry;
    std::shared_ptr<NodeStyle> node_style;
    double z_value = 0;
    NodeItem* draw_item = nullptr;
};
class InPortItemData
{
public:
    NodeData data;
    std::unique_ptr<DynamicHPortGeometry> geometry;
    std::shared_ptr<NodeStyle> node_style;
    double z_value = 0;
    InNodeItem* draw_item = nullptr;
};
class OutPortItemData
{
public:
    NodeData data;
    std::unique_ptr<DynamicHPortGeometry> geometry;
    std::shared_ptr<NodeStyle> node_style;
    double z_value = 0;
    OutNodeItem* draw_item = nullptr;
};
class ConnectionItemData
{
public:
    Connection connection;
    std::shared_ptr<ConnectionStyle> style;
    double z_value = 0;
    ConnectionItem* draw_item = nullptr;
};

class FlowSceneData
{
public:
    //原始数据
    std::map<guid16, std::unique_ptr<NodeItemData>> node_items;
    std::map<guid16, std::unique_ptr<InPortItemData>> in_node_items;
    std::map<guid16, std::unique_ptr<OutPortItemData>> out_node_items;
    std::map<guid18, std::unique_ptr<ConnectionItemData>> connection_items;

    //查询接口
    bool getNodePortPosition(const guid16 id, PortType type, unsigned int port_index, QPointF& position);

private:
};
} //namespace fe
