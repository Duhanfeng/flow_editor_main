//
// Created by cxc on 2024/3/11.
//

#pragma once
#include <unordered_map>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_port_geometry.hpp>

namespace fe
{
class AbsNodeItem;
class NodeItem;
class InNodeItem;
class OutNodeItem;
class ConnectionItem;
class DraftConnectionItem;
class FlowScene;

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
    //样式管理
    NodeLayoutStyle node_layout_style = NodeLayoutStyle::Horizontal;
    std::shared_ptr<NodeStyle> node_style;
    std::shared_ptr<ConnectionStyle> connection_style;
    std::shared_ptr<DraftConnectionStyle> draft_connection_style;

    //深度管理
    double node_z_value = 100;
    double connection_z_value = 0;

    //子组件管理
    std::shared_ptr<Flow> flow;

    //原始数据
    std::map<guid16, std::unique_ptr<NodeItemData>> node_items;
    std::map<guid16, std::unique_ptr<InPortItemData>> in_node_items;
    std::map<guid16, std::unique_ptr<OutPortItemData>> out_node_items;
    std::map<guid18, std::unique_ptr<ConnectionItemData>> connection_items;
    std::unique_ptr<DraftConnectionItem> draft_connection;
    FlowScene* scene = nullptr;

    //查询接口
    AbsNodeItem* absNodeItem(const guid16& id);                                                            //返回当前id对应的AbsNodeItem
    ConnectionItem* connectionItem(const guid18& id);                                                      //返回当前id对应的ConnectionItem
    std::vector<ConnectionItem*> allConnectionItems(const guid16& id);                                     //返回当前node id所涉及的所有ConnectionItem
    bool getNodePortPosition(const guid16& id, PortType type, unsigned int port_index, QPointF& position); //查询对应节点的对应端口

    //控制接口
    void makeDraftConnection(PortType required_port, const guid16& id, unsigned int port_index);                    //添加草稿连接
    void makeDraftConnection(PortType required_port, const guid16& id, unsigned int port_index, QPointF end_point, const guid16& last_hovered_node); //添加草稿连接
    void resetDraftConnection();                                                                                    //复位草稿连接
    void removeConnection(const guid18& id);                                                                        //移除连接
    void addConnection(const guid18& id, const Connection& connection);                                             //添加连接

private:
};
} //namespace fe
