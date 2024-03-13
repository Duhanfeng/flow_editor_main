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
    static void paint(QPainter* painter, OutNodeItem* item);

private:
    static void paintFrame(QPainter* painter, OutNodeItem* item);
    static void paintNodeRect(QPainter* painter, OutNodeItem* item);
    static void paintCaption(QPainter* painter, OutNodeItem* item);
    static void paintConnectionPoints(QPainter* painter, OutNodeItem* item);
};

} //namespace fe
