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

    //连接绑定
    void setConnection(ConnectionItem* item);

    //绘画接口
    QRectF boundingRect() const override { return geometry_->components().bounding_rect; }
    QPainterPath shape() const override { return shape_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    //绘画相关
    void updateCache(double scale);
    static void paintTo(QPainter* painter, const PortUIComponents& components, double scale, std::shared_ptr<NodeStyle>& style);

private:
    NodeData* data_ = nullptr;
    DynamicHPortGeometry* geometry_ = nullptr;
    std::shared_ptr<NodeStyle> style_ = nullptr;
    double z_value_ = 0.0;
    double scale_ = 0.0;
    ConnectionItem* connect_item_ = nullptr;
    QPainterPath shape_;
    //bool is_moving_ = false;
};

} //namespace fe