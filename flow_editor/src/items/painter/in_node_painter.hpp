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
    static void paint(QPainter* painter, InNodeItem* item);

private:
    static void paintFrame(QPainter* painter, InNodeItem* item);
    static void paintNodeRect(QPainter* painter, InNodeItem* item);
    static void paintCaption(QPainter* painter, InNodeItem* item);
    static void paintConnectionPoints(QPainter* painter, InNodeItem* item);
};

} //namespace fe
