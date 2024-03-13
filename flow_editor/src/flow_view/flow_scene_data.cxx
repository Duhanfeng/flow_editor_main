//
// Created by cxc on 2024/3/11.
//

#include "flow_scene_data.hpp"

#include <src/items/abs_node_item.hpp>
#include <src/items/node_item.hpp>
#include <src/items/in_node_item.hpp>
#include <src/items/out_node_item.hpp>
#include <src/items/connection/connection_item.hpp>
#include <src/items/connection/draft_connection_item.hpp>

namespace fe
{
FlowSceneData::FlowSceneData(FlowScene& scene_, SceneConfig& scene_config_) :
    scene(scene_), scene_config(scene_config_)
{
}

void FlowSceneData::setFlow(std::shared_ptr<Flow> flow_)
{
    flow = flow_;

    //清空资源
    scene.clear();
    node_items.clear();
    in_node_items.clear();
    out_node_items.clear();
    connection_items.clear();
    node_z_value = 100;
    connection_z_value = 0;

    if (!flow)
    {
        return;
    }

    //构建对象
    for (const auto& node : flow->nodes)
    {
        addNode(node.first, node.second);
    }
    for (const auto& node : flow->in_nodes)
    {
        addInNode(node.first, node.second);
    }
    for (const auto& node : flow->out_nodes)
    {
        addOutNode(node.first, node.second);
    }
    for (const auto& connection : flow->connections)
    {
        addConnection(connection.first, connection.second);
    }
}

NodeData* FlowSceneData::getNodeData(const guid16& id)
{
    auto iter_node = node_items.find(id);
    if (iter_node != node_items.end())
    {
        return &iter_node->second->data;
    }
    auto iter_in = in_node_items.find(id);
    if (iter_in != in_node_items.end())
    {
        return &iter_in->second->data;
    }
    auto iter_out = out_node_items.find(id);
    if (iter_out != out_node_items.end())
    {
        return &iter_out->second->data;
    }
    return nullptr;
}

AbsNodeItem* FlowSceneData::absNodeItem(const guid16& id)
{
    auto iter_node = node_items.find(id);
    if (iter_node != node_items.end())
    {
        return iter_node->second->draw_item;
    }
    auto iter_in = in_node_items.find(id);
    if (iter_in != in_node_items.end())
    {
        return iter_in->second->draw_item;
    }
    auto iter_out = out_node_items.find(id);
    if (iter_out != out_node_items.end())
    {
        return iter_out->second->draw_item;
    }
    return nullptr;
}
ConnectionItem* FlowSceneData::connectionItem(const guid18& id)
{
    auto iter_node = connection_items.find(id);
    if (iter_node != connection_items.end())
    {
        return iter_node->second->draw_item;
    }
    return nullptr;
}
std::vector<ConnectionItem*> FlowSceneData::allConnectionItems(const guid16& id)
{
    std::vector<ConnectionItem*> items;
    for (const auto& item : connection_items)
    {
        if ((item.second->connection.in == id) || (item.second->connection.out == id))
        {
            items.emplace_back(item.second->draw_item);
        }
    }
    return items;
}
bool FlowSceneData::getNodePortPosition(const guid16& id, PortType type, unsigned int port_index, QPointF& position)
{
    auto iter_node = node_items.find(id);
    if (iter_node != node_items.end())
    {
        if (type == PortType::In)
        {
            const auto& port_rect = iter_node->second->geometry->components().in_ports;
            if (port_index < port_rect.size())
            {
                position = iter_node->second->draw_item->pos() + port_rect[port_index].port_center;
                return true;
            }
        }
        else if (type == PortType::Out)
        {
            const auto& port_rect = iter_node->second->geometry->components().out_ports;
            if (port_index < port_rect.size())
            {
                position = iter_node->second->draw_item->pos() + port_rect[port_index].port_center;
                return true;
            }
        }
        return false;
    }
    auto iter_in = in_node_items.find(id);
    if (iter_in != in_node_items.end())
    {
        const auto& port_rect = iter_in->second->geometry->components().port_rect;
        position = iter_in->second->draw_item->pos() + port_rect.center();
        return true;
    }
    auto iter_out = out_node_items.find(id);
    if (iter_out != out_node_items.end())
    {
        const auto& port_rect = iter_out->second->geometry->components().port_rect;
        position = iter_out->second->draw_item->pos() + port_rect.center();
        return true;
    }

    return false;
}
bool FlowSceneData::getNodePreviewPortPosition(const guid16& id, PortType type, unsigned int port_index, QPointF& position)
{
    auto iter_node = node_items.find(id);
    if (iter_node != node_items.end())
    {
        if (type == PortType::In)
        {
            const auto& port_rect = iter_node->second->geometry->simpleComponents().in_port_rect;
            position = iter_node->second->draw_item->pos() + port_rect.center();
            return true;
        }
        else if (type == PortType::Out)
        {
            const auto& port_rect = iter_node->second->geometry->simpleComponents().out_port_rect;
            position = iter_node->second->draw_item->pos() + port_rect.center();
            return true;
        }
        return false;
    }
    auto iter_in = in_node_items.find(id);
    if (iter_in != in_node_items.end())
    {
        const auto& port_rect = iter_in->second->geometry->components().port_rect;
        position = iter_in->second->draw_item->pos() + port_rect.center();
        return true;
    }
    auto iter_out = out_node_items.find(id);
    if (iter_out != out_node_items.end())
    {
        const auto& port_rect = iter_out->second->geometry->components().port_rect;
        position = iter_out->second->draw_item->pos() + port_rect.center();
        return true;
    }

    return false;
}
bool FlowSceneData::checkConnectionPossible(const guid16& out_id, unsigned int out_port_index, const guid16& in_id, unsigned int in_port_index)
{
    auto out_data = getNodeData(out_id);
    auto in_data = getNodeData(in_id);
    if (!out_data || !in_data)
    {
        return false;
    }
    if (out_port_index >= out_data->outputs.size())
    {
        return false;
    }
    if (in_port_index >= in_data->inputs.size())
    {
        return false;
    }
    if (out_id == in_id)
    {
        return false;
    }

    const PortData& out_port_data = out_data->outputs[out_port_index];
    const PortData& in_port_data = in_data->inputs[in_port_index];

    if (out_port_data.port_type == in_port_data.port_type || out_port_data.port_type.isEmpty() || in_port_data.port_type.isEmpty())
    {
        return true;
    }

    return false;
}

void FlowSceneData::makeDraftConnection(PortType required_port, const guid16& id, unsigned int port_index)
{
    resetDraftConnection();
    if (required_port == PortType::None)
    {
        return;
    }

    //如果需要连接到Out端口,并且已存在对应连接, 则转为: 将为当前连接查找输入端口
    if (required_port == PortType::Out)
    {
        for (const auto& connection_item : connection_items)
        {
            const auto& connection = connection_item.second->connection;
            if (connection.in == id && connection.in_port == port_index)
            {
                auto draw_item = connection_item.second->draw_item;
                draft_connection = std::make_unique<DraftConnectionItem>(scene, PortType::In, connection.out, connection.out_port, scene_config.draft_connection_style, draw_item);
                //draft_connection->setEndPoint(PortType::In, );
                draft_connection->grabMouse();
                return;
            }
        }
    }
    draft_connection = std::make_unique<DraftConnectionItem>(scene, required_port, id, port_index, scene_config.draft_connection_style);
    draft_connection->grabMouse();
}
void FlowSceneData::makeDraftConnection(PortType required_port, const guid16& id, unsigned int port_index, QPointF end_point, const guid16& last_hovered_node)
{
    resetDraftConnection();
    if (required_port == PortType::None)
    {
        return;
    }

    //如果需要连接到Out端口,并且已存在对应连接, 则转为: 将为当前连接查找输入端口
    if (required_port == PortType::Out)
    {
        for (const auto& connection_item : connection_items)
        {
            const auto& connection = connection_item.second->connection;
            if (connection.in == id && connection.in_port == port_index)
            {
                auto draw_item = connection_item.second->draw_item;
                draft_connection = std::make_unique<DraftConnectionItem>(scene, PortType::In, connection.out, connection.out_port, scene_config.draft_connection_style, draw_item);
                draft_connection->setEndPoint(PortType::In, end_point);
                draft_connection->setLastHoveredNode(last_hovered_node);
                draft_connection->grabMouse();
                draft_connection->update();
                return;
            }
        }
    }
    draft_connection = std::make_unique<DraftConnectionItem>(scene, required_port, id, port_index, scene_config.draft_connection_style);
    draft_connection->grabMouse();
}

void FlowSceneData::resetDraftConnection()
{
    if (draft_connection)
    {
        scene.removeItem(draft_connection.get());
        draft_connection = nullptr;
    }
}

void FlowSceneData::recheck()
{
    //对于1000个对象,进行一次recheck,耗时大概7-10ms
    if (flow == nullptr)
    {
        return;
    }

    //如果存在scene中存在但是flow中没有的,则移除
    std::vector<guid16> invalid_node_ids;
    for (const auto& scene_node : node_items)
    {
        const auto& node_id = scene_node.first;
        auto itr = flow->nodes.find(node_id);
        if (itr == flow->nodes.end())
        {
            invalid_node_ids.emplace_back(node_id);
        }
    }
    for (const auto& node_id : invalid_node_ids)
    {
        auto itr = node_items.find(node_id);
        scene.removeItem(itr->second->draw_item);
        node_items.erase(itr);
    }

    //如果是flow中存在,但是scene中没有的,则构建
    std::vector<guid16> valid_node_ids;
    for (const auto& flow_node : flow->nodes)
    {
        const auto& node_id = flow_node.first;
        auto itr = node_items.find(node_id);
        if (itr == node_items.end())
        {
            valid_node_ids.emplace_back(node_id);
        }
    }
    for (const auto& node_id : valid_node_ids)
    {
        auto itr = flow->nodes.find(node_id);
        addInNode(itr->first, itr->second);
    }

    //如果存在scene中存在但是flow中没有的,则移除
    std::vector<guid18> invalid_connection_ids;
    for (const auto& scene_node : connection_items)
    {
        const auto& node_id = scene_node.first;
        auto itr = flow->connections.find(node_id);
        if (itr == flow->connections.end())
        {
            invalid_connection_ids.emplace_back(node_id);
        }
    }
    for (const auto& connection_id : invalid_connection_ids)
    {
        auto itr = connection_items.find(connection_id);
        scene.removeItem(itr->second->draw_item);
        connection_items.erase(itr);
    }

    //如果是flow中存在,但是scene中没有的,则构建
    std::vector<guid18> valid_connection_ids;
    for (const auto& flow_connection : flow->connections)
    {
        const auto& connection_id = flow_connection.first;
        auto itr = connection_items.find(connection_id);
        if (itr == connection_items.end())
        {
            valid_connection_ids.emplace_back(connection_id);
        }
    }
    for (const auto& connection_id : valid_connection_ids)
    {
        auto itr = flow->connections.find(connection_id);
        addConnection(itr->first, itr->second);
    }
}
void FlowSceneData::addInNode(const guid16& id, const NodeData& data)
{
    //构造InPortItemData对象
    auto item_data = std::make_unique<InPortItemData>();
    item_data->data = data; //直接拷贝NodeData数据
    item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::InNode, item_data->data, scene_config.node_style, scene_config.type_color_map);
    item_data->node_style = scene_config.node_style;
    item_data->z_value = node_z_value;
    //保存数据
    in_node_items.emplace(id, std::move(item_data));
    new InNodeItem(scene, id);
    node_z_value++;
}
void FlowSceneData::addOutNode(const guid16& id, const NodeData& data)
{
    //构造OutPortItemData对象
    auto item_data = std::make_unique<OutPortItemData>();
    item_data->data = data; //直接拷贝NodeData数据
    item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::OutNode, item_data->data, scene_config.node_style, scene_config.type_color_map);
    item_data->node_style = scene_config.node_style;
    item_data->z_value = node_z_value;
    //保存数据
    out_node_items.emplace(id, std::move(item_data));
    new OutNodeItem(scene, id);
    node_z_value++;
}
void FlowSceneData::addNode(const guid16& id, const NodeData& data)
{
    //构造NodeItemData对象
    auto item_data = std::make_unique<NodeItemData>();
    item_data->data = data; //直接拷贝NodeData数据
    item_data->geometry = std::make_unique<DynamicGeometry>(item_data->data, scene_config.node_style, scene_config.type_color_map);
    item_data->node_style = scene_config.node_style;
    item_data->z_value = node_z_value;
    //保存数据
    node_items.emplace(id, std::move(item_data));
    new NodeItem(scene, id);
    node_z_value++;
}
void FlowSceneData::addConnection(const guid18& id, const Connection& connection)
{
    std::unique_ptr<ConnectionItemData> item_data = std::make_unique<ConnectionItemData>();
    item_data->connection = connection;
    item_data->style = scene_config.connection_style;
    item_data->z_value = connection_z_value;
    connection_items.emplace(id, std::move(item_data));
    new ConnectionItem(scene, id);
    connection_z_value++;
}
void FlowSceneData::removeConnection(const guid18& id)
{
    auto iter = connection_items.find(id);
    if (iter != connection_items.end())
    {
        scene.removeItem(iter->second->draw_item);
        connection_items.erase(iter);
    }
}
void FlowSceneData::deleteItems(const std::vector<guid16>& node_ids, const std::vector<guid18>& connection_ids)
{
    //删除nodes
    for (size_t i = 0; i < node_ids.size(); ++i)
    {
        auto iter = node_items.find(node_ids[i]);
        if (iter != node_items.end())
        {
            scene.removeItem(iter->second->draw_item);
            node_items.erase(iter);
        }
    }
    //删除connections
    for (size_t i = 0; i < connection_ids.size(); ++i)
    {
        auto iter = connection_items.find(connection_ids[i]);
        if (iter != connection_items.end())
        {
            scene.removeItem(iter->second->draw_item);
            connection_items.erase(iter);
        }
    }
}

} //namespace fe