//
// Created by cxc on 2024/3/12.
//

#pragma once
#include <src/items/out_node_item.hpp>

namespace fe
{
class OutNodePainter
{
public:
    static void painter(QPainter* painter, OutNodeItem* item);
};

} //namespace fe
