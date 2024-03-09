//
// Created by cxc on 2024/3/9.
//

#pragma once
#include <memory>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_port_geometry.hpp>

namespace fe
{
enum class ConnectionMode
{
    None,
    Port2Port,
    Port2Node,
    Node2Node,
    Node2Port,
    FloatPort,
    FloatNode
};

class ConnectionItem : public QGraphicsItem
{
public:
    ConnectionItem() = delete;
    ConnectionItem(double z_value);

    //配置接口
    void updateStart(const QPoint& value);
    void updateEnd(const QPoint& value);
    void updateCache(const QPoint& start, const QPoint& end);

    //绘画接口
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

private:
    void updateCache();

private:
    DynamicHPortGeometry* out_port_ = nullptr;
    DynamicHGeometry* out_node_ = nullptr;
    unsigned int out_port_index_ = 0;
    QGraphicsItem* out_item_ = nullptr;

    DynamicHPortGeometry* in_port_ = nullptr;
    DynamicHGeometry* in_node_ = nullptr;
    unsigned int in_port_index_ = 0;
    QGraphicsItem* in_item_ = nullptr;

    ConnectionMode connection_mode_ = ConnectionMode::None;

    double z_value_ = 0.0;

    //缓存
    QPoint start_;
    QPoint end_;
    QRectF bounding_rect_;
    QPainterPath cubic_;
};

} //namespace fe
