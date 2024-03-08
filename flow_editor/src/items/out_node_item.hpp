//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <memory>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_hor_geometry.hpp>

namespace fe
{
class OutNodeItem : public QGraphicsItem
{
public:
    OutNodeItem() = delete;
    OutNodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

private:
    //绘画相关
    void updateCache();
    static void paintTo(QPainter* painter, double scale, const NodeData& data, const NodeSubGeometry& node_sub_geometry, std::shared_ptr<NodeStyle>& style);

private:
    NodeData data_;
    std::shared_ptr<NodeStyle> style_;
    DynamicHorGeometry dynamic_hor_geometry_;
    double scale_ = 1.0;
};

} //namespace fe