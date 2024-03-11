#include "connection_state.hpp"

#include <QtCore/QDebug>
#include <QtCore/QPointF>

#include <flow_editor/flow_view/flow_scene.hpp>
#include "connection_item.hpp"

namespace fe
{

ConnectionState::~ConnectionState()
{
    //resetLastHoveredNode();
}

PortType ConnectionState::requiredPort() const
{
    PortType t = PortType::None;
    if (isInvalid(cgo_.connection_->out))
    {
        t = PortType::Out;
    }
    else if (isInvalid(cgo_.connection_->in))
    {
        t = PortType::In;
    }

    return t;
}

bool ConnectionState::requiresPort() const
{
    return (isInvalid(cgo_.connection_->out)) || (isInvalid(cgo_.connection_->in));
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
        //auto ngo = cgo_.scene_.nodeGraphicsObject(last_hovered_node_);
        //ngo->update();
    }

    last_hovered_node_ = { 0 };
}

} //namespace fe
