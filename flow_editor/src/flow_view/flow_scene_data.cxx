//
// Created by cxc on 2024/3/11.
//

#include "flow_scene_data.hpp"

namespace fe
{
bool FlowSceneData::getNodePortPosition(const guid16 id, PortType type, unsigned int port_index, QPointF& position)
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
void FlowSceneData::removeConnection(const guid18& id)
{
}
void FlowSceneData::addConnection(const guid18& id, const Connection& connection)
{
}
} //namespace fe