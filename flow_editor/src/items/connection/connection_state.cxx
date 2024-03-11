#include "connection_state.hpp"

#include <QtCore/QDebug>
#include <QtCore/QPointF>

#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/items/abs_node_item.hpp>
#include <src/items/connection/draft_connection_item.hpp>

namespace fe
{

ConnectionState::~ConnectionState()
{
    //resetLastHoveredNode();
}

PortType ConnectionState::requiredPort() const
{
    PortType t = PortType::None;
    return t;
}

bool ConnectionState::requiresPort() const
{
    //return (isInvalid(cgo_.connection_->out)) || (isInvalid(cgo_.connection_->in));
    return false;
}

bool ConnectionState::hovered() const
{
    return hovered_;
}

void ConnectionState::setHovered(bool hovered)
{
    hovered_ = hovered;
}

void ConnectionState::setLastHoveredNode(guid16 const node_id)
{
    last_hovered_node_ = node_id;
}
const guid16& ConnectionState::lastHoveredNode() const
{
    return last_hovered_node_;
}
void ConnectionState::resetLastHoveredNode()
{
    if (!isInvalid(last_hovered_node_))
    {
        auto item = cgo_.scene_.absNodeItem(last_hovered_node_);
        if (item)
        {
            item->update();
        }
    }
    last_hovered_node_ = { 0 };
}

} //namespace fe
