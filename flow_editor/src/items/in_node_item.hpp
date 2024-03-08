﻿//
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
    InNodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style);
    QRectF boundingRect() const override { return dynamic_port_geometry_.geometry().bounding_rect; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

private:
    //绘画相关
    void updateCache();
    static void paintTo(QPainter* painter, const PortSubGeometry& geometry, double scale, std::shared_ptr<NodeStyle>& style);

private:
    NodeData data_;
    std::shared_ptr<NodeStyle> style_;
    DynamicHPortGeometry dynamic_port_geometry_;
    double scale_ = 1.0;
};

} //namespace fe