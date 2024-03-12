//
// Created by cxc on 2024/3/11.
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

namespace fe
{
//连接草稿,用于显示未有效的连接,从out->in
class DraftConnectionItem : public QGraphicsItem
{
public:
    DraftConnectionItem() = delete;
    DraftConnectionItem(FlowScene& scene, PortType required_port, const guid16& id, unsigned int port_index, std::shared_ptr<DraftConnectionStyle> style);
    DraftConnectionItem(FlowScene& scene, PortType required_port, const guid16& id, unsigned int port_index, std::shared_ptr<DraftConnectionStyle> style, ConnectionItem* original_item);

    //查询接口
    PortType requiredPort() const { return required_port_; }
    const QPoint& out() const { return out_; }
    const QPoint& in() const { return in_; }
    const QPoint& endPoint(PortType port_type) const { return (port_type == PortType::Out ? out_ : in_); }
    void setEndPoint(PortType port_type, QPointF const& point);
    void setLastHoveredNode(const guid16& last_hovered_node) { last_hovered_node_ = last_hovered_node; }

    //绘画接口
    QRectF boundingRect() const override { return bounding_rect_; }
    QPainterPath shape() const override { return shape_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void updateCache();

private:
    FlowScene& scene_;
    PortType required_port_ = PortType::None; //所需要连接的端口
    guid16 id_;
    unsigned int port_index_;
    std::shared_ptr<DraftConnectionStyle> style_;
    ConnectionItem* original_item_ = nullptr;

    //缓存
    QPoint out_;
    QPoint in_;
    QRectF bounding_rect_;
    QPainterPath cubic_;
    QPainterPath shape_;
    guid16 last_hovered_node_ = { 0 }; //最后的悬停节点
};

} //namespace fe
