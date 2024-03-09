//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <memory>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_port_geometry.hpp>

namespace fe
{
class InNodeItem : public QGraphicsItem
{
public:
    InNodeItem() = delete;
    InNodeItem(NodeData& data, DynamicHPortGeometry& geometry, std::shared_ptr<NodeStyle> style, double z_value);
    QRectF boundingRect() const override { return geometry_.components().bounding_rect; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

private:
    //绘画相关
    void updateCache();
    static void paintTo(QPainter* painter, const PortUIComponents& components, double scale, std::shared_ptr<NodeStyle>& style);

private:
    NodeData& data_;
    DynamicHPortGeometry& geometry_;
    std::shared_ptr<NodeStyle> style_;
    double z_value_ = 0.0;
    double scale_ = 1.0;
};

} //namespace fe