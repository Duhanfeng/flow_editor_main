//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QColor>
#include <QIcon>

namespace ss
{
class GraphicsViewStyle
{
public:
    QColor background_color;
    QColor fine_grid_color;
    QColor coarse_grid_color;
};

enum class NodeType
{
    Node,
    InNode,
    OutNode
};
class PortData
{
public:
    QString port_name;
    QColor port_color;
};
class NodeData
{
public:
    NodeType node_type = NodeType::Node;
    QString node_name;
    QIcon node_icon;
    QPointF position;
    std::vector<PortData> in_port;
    std::vector<PortData> out_port;
};

} //namespace ss