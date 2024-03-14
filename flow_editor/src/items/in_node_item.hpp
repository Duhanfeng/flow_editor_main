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
#include <src/items/abs_node_item.hpp>

namespace fe
{
class ConnectionItem;
class InNodeItem final : public AbsNodeItem
{
public:
    InNodeItem() = delete;
    InNodeItem(FlowScene& scene, const guid16& id);

    int getPortIndex(PortType required_port, const QPoint& pos) const override;

    //绘画接口
    QRectF boundingRect() const override { return geometry_->components().bounding_rect; }
    QPainterPath shape() const override { return shape_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    //事件重载
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

    //更新缓存
    void updateCache(double scale);

private:
    friend class InNodePainter;
    NodeData* data_ = nullptr;
    DynamicHPortGeometry* geometry_ = nullptr;
    std::shared_ptr<NodeStyle> style_ = nullptr;
    double z_value_ = 0.0;
    double scale_ = 0.0;
    QPainterPath shape_;
};

} //namespace fe