﻿//
// Created by cxc on 2024/3/11.
//

#pragma once
#include <map>
#include <unordered_map>
#include <flow_editor/data_def.hpp>
#include <flow_editor/flow_model.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_port_geometry.hpp>
#include <src/flow_view/scene_config.hpp>

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
    std::unique_ptr<DynamicGeometry> geometry;
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
    //场景数据
    FlowScene& scene;
    SceneConfig& scene_config;
    std::shared_ptr<FlowModel> flow_model;

    //原始数据
    std::map<guid16, std::unique_ptr<NodeItemData>> node_items;
    std::map<guid16, std::unique_ptr<InPortItemData>> in_node_items;
    std::map<guid16, std::unique_ptr<OutPortItemData>> out_node_items;
    std::map<guid18, std::unique_ptr<ConnectionItemData>> connection_items;
    std::unique_ptr<DraftConnectionItem> draft_connection;
    double node_z_value = 100;                 //当前节点深度,递增
    double connection_z_value = 0;             //当前连接深度,递增
    const double top_node_z_value = 500;       //node置顶时的Z轴高度
    const double top_connection_z_value = 495; //node置顶时,其connections的Z轴高度

public:
    //构造
    FlowSceneData(FlowScene& scene, SceneConfig& scene_config);
    void setFlow(std::shared_ptr<FlowModel> flow_model);

    //查询接口
    NodeData* getNodeData(const guid16& id);
    AbsNodeItem* absNodeItem(const guid16& id);                                                                                       //返回当前id对应的AbsNodeItem
    ConnectionItem* connectionItem(const guid18& id);                                                                                 //返回当前id对应的ConnectionItem
    std::vector<ConnectionItem*> allConnectionItems(const guid16& id);                                                                //返回当前node id所涉及的所有ConnectionItem
    bool getNodePortPosition(const guid16& id, PortType type, unsigned int port_index, QPointF& position);                            //查询对应节点的对应端口
    bool getNodePreviewPortPosition(const guid16& id, PortType type, unsigned int port_index, QPointF& position);                     //查询对应节点的对应端口
    bool checkConnectionPossible(const guid16& out_id, unsigned int out_port_index, const guid16& in_id, unsigned int in_port_index); //查询是否能够连接

    //流程内对象管理
    void recheck();                                                                                   //重新校验flow和本地资源的差异性,适用于flow内部数据发生较大调整难以同步的情况
    void addInNode(const guid16& id, const NodeData& data);                                           //添加输入节点
    void addOutNode(const guid16& id, const NodeData& data);                                          //添加输出节点
    void addNode(const guid16& id, const NodeData& data);                                             //添加节点
    void addConnection(const guid18& id, const Connection& connection);                               //添加连接
    void removeConnection(const guid18& id);                                                          //移除连接
    void deleteItems(const std::vector<guid16>& node_ids, const std::vector<guid18>& connection_ids); //删除对应对象
    void makeDraftConnection(PortType required_port, const guid16& id, unsigned int port_index);      //添加草稿连接
    void makeDraftConnection(PortType required_port, const guid16& id, unsigned int port_index,
        QPointF end_point, const guid16& last_hovered_node); //添加草稿连接
    void resetDraftConnection();                             //复位草稿连接

private:
};
} //namespace fe
