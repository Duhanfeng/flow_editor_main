//
// Created by cxc on 2024/3/9.
//

#pragma once
#include <memory>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_port_geometry.hpp>
#include "connection_state.hpp"

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
    ConnectionItem(FlowScene& scene, const guid18& id);

    //配置接口
    void updateStart(const QPoint& value);
    void updateEnd(const QPoint& value);
    void updateCache(const QPoint& start, const QPoint& end);
    void move();

    //绘画接口
    QRectF boundingRect() const override { return bounding_rect_; }
    QPainterPath shape() const override { return shape_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    void updateCache();

private:
    FlowScene& scene_;
    Connection* connection_ = nullptr;
    std::shared_ptr<ConnectionStyle> style_ = nullptr;
    double z_value_ = 0.0;

    friend ConnectionState;
    ConnectionState connection_state_;

    //配置参数
    //DynamicHPortGeometry* out_port_ = nullptr;
    //DynamicHGeometry* out_node_ = nullptr;
    //unsigned int out_port_index_ = 0;
    //QGraphicsItem* out_item_ = nullptr;
    //DynamicHPortGeometry* in_port_ = nullptr;
    //DynamicHGeometry* in_node_ = nullptr;
    //unsigned int in_port_index_ = 0;
    //QGraphicsItem* in_item_ = nullptr;
    //ConnectionMode connection_mode_ = ConnectionMode::None;

    //缓存
    QPoint start_;
    QPoint end_;
    QRectF bounding_rect_;
    QPainterPath cubic_;
    QPainterPath shape_;
};

} //namespace fe
