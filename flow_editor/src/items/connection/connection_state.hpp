#pragma once

#include <QtCore/QUuid>
#include <flow_editor/flow_view/flow_scene.hpp>
#include <flow_editor/data_def.hpp>

class QPointF;
namespace fe
{

class ConnectionItem;

///Stores currently draggind end.
///Remembers last hovered Node.
class ConnectionState
{
public:
    ///Defines whether we construct a new connection
    ///or it is already binding two nodes.
    enum LooseEnd
    {
        Pending = 0,
        Connected = 1
    };

public:
    ConnectionState(ConnectionItem& cgo) :
        cgo_(cgo), hovered_(false)
    {
    }

    ConnectionState(ConnectionState const&) = delete;
    ConnectionState(ConnectionState&&) = delete;

    ConnectionState& operator=(ConnectionState const&) = delete;
    ConnectionState& operator=(ConnectionState&&) = delete;

    ~ConnectionState();

public:
    PortType requiredPort() const;
    bool requiresPort() const;
    bool hovered() const;
    void setHovered(bool hovered);

public:
    ///Caches guid16 for further interaction.
    void setLastHoveredNode(guid16 const node_id);
    const guid16& lastHoveredNode() const;
    void resetLastHoveredNode();

private:
    ConnectionItem& cgo_;

    bool hovered_; //悬停标志
    guid16 last_hovered_node_ = { 0 };//最后的悬停节点
};
} //namespace fe
