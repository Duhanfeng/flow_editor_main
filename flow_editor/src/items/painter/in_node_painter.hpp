//
// Created by cxc on 2024/3/12.
//

#pragma once
#include <src/items/in_node_item.hpp>

namespace fe
{
class InNodePainter
{
public:
    static void painter(QPainter* painter, InNodeItem* item);
};

} //namespace fe
