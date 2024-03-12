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
    static void paint(QPainter* painter, NodeItem* item);

private:
    static void paintNormal(QPainter* painter, NodeItem* item);
    static void paintSimple(QPainter* painter, NodeItem* item);

private:
    static void paintFrame(QPainter* painter, NodeItem* item);
    static void paintNodeRect(QPainter* painter, NodeItem* item);
    static void paintRunBtn(QPainter* painter, NodeItem* item);
    static void paintConnectionPoints(QPainter* painter, NodeItem* item);
};

} //namespace fe
