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
            const auto& port_rect = iter_node->second->geometry->components().in_port_rect;
            if (port_index < port_rect.size())
            {
                position = iter_node->second->draw_item->pos() + port_rect[port_index].center();
                return true;
            }
        }
        else if (type == PortType::Out)
        {
            const auto& port_rect = iter_node->second->geometry->components().out_port_rect;
            if (port_index < port_rect.size())
            {
                position = iter_node->second->draw_item->pos() + port_rect[port_index].center();
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
                draft_connection = std::make_unique<DraftConnectionItem>(*scene, PortType::In, connection.out, connection.out_port, draft_connection_style, draw_item);
                //draft_connection->setEndPoint(PortType::In, );
                draft_connection->grabMouse();
                return;
            }
        }
    }
    draft_connection = std::make_unique<DraftConnectionItem>(*scene, required_port, id, port_index, draft_connection_style);
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
                draft_connection = std::make_unique<DraftConnectionItem>(*scene, PortType::In, connection.out, connection.out_port, draft_connection_style, draw_item);
                draft_connection->setEndPoint(PortType::In, end_point);
                draft_connection->setLastHoveredNode(last_hovered_node);
                draft_connection->grabMouse();
                draft_connection->update();
                return;
            }
        }
    }
    draft_connection = std::make_unique<DraftConnectionItem>(*scene, required_port, id, port_index, draft_connection_style);
    draft_connection->grabMouse();
}

void FlowSceneData::resetDraftConnection()
{
    if (draft_connection)
    {
        scene->removeItem(draft_connection.get());
        draft_connection = nullptr;
    }
}
void FlowSceneData::removeConnection(const guid18& id)
{
    auto iter = connection_items.find(id);
    if (iter != connection_items.end())
    {
        connection_items.erase(iter);
    }
}
void FlowSceneData::addConnection(const guid18& id, const Connection& connection)
{
    std::unique_ptr<ConnectionItemData> item_data = std::make_unique<ConnectionItemData>();
    item_data->connection = connection;
    item_data->style = connection_style;
    item_data->z_value = connection_z_value;
    connection_items.emplace(id, std::move(item_data));
    new ConnectionItem(*scene, id);
    connection_z_value++;
}

} //namespace fe