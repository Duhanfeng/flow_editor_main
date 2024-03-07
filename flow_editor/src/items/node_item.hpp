//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>

namespace ss
{
class NodeItem final : public QGraphicsItem
{
public:
    NodeItem() = delete;
    NodeItem(const NodeData& data, const NodeSubGeometry& node_sub_geometry);
    void updateCache();

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

private:
    NodeData data_;
    NodeSubGeometry node_sub_geometry_;
    QRectF rect_cache_;
};

} //namespace ss