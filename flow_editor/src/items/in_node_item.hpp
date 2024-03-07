//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>

namespace fe
{
class InNodeItem final : public QGraphicsItem
{
public:
    InNodeItem() = delete;
    InNodeItem(const NodeData& data, const NodeSubGeometry& node_sub_geometry);
    void updateCache();
    void initializePixmap(double scale);
    static void paintTo(QPainter* painter, const NodeData& data, const NodeSubGeometry& node_sub_geometry);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

private:
    NodeData data_;
    NodeSubGeometry node_sub_geometry_;
    NodeSubGeometry geometry_scale_;
    QPixmap pixmap_cache_;
    double scale_ = 1.0;
};

} //namespace fe