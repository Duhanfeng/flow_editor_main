//
// Created by cxc on 2024/3/12.
//

#pragma once
#include <src/items/node_item.hpp>

namespace fe
{
class NodePainter
{
public:
    static void painter(QPainter* painter, NodeItem* item);

private:
    static void painterNormal(QPainter* painter, NodeItem* item);
    static void painterSimple(QPainter* painter, NodeItem* item);
};

} //namespace fe
