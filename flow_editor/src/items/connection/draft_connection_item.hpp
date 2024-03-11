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
#include "connection_state.hpp"

namespace fe
{
//连接草稿,用于显示未有效的连接,从out->in
class DraftConnectionItem : public QGraphicsItem
{
public:
    DraftConnectionItem() = delete;
    DraftConnectionItem(FlowScene& scene, PortType required_port, const guid16& id, unsigned int port_index, std::shared_ptr<DraftConnectionStyle> style);
    DraftConnectionItem(FlowScene& scene, PortType required_port, const guid16& id, unsigned int port_index, std::shared_ptr<DraftConnectionStyle> style, ConnectionItem* original_item);

    //绘画接口
    QRectF boundingRect() const override { return bounding_rect_; }
    QPainterPath shape() const override { return shape_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void setEndPoint(PortType port_type, QPointF const& point);
    void updateCache();

private:
    FlowScene& scene_;
    PortType required_port_ = PortType::None; //所需要的端口
    const guid16& id_;
    unsigned int port_index_;
    std::shared_ptr<DraftConnectionStyle> style_;
    ConnectionItem* original_item_ = nullptr;

    friend ConnectionState;
    ConnectionState connection_state_;

    //缓存
    QPoint out_;
    QPoint in_;
    QRectF bounding_rect_;
    QPainterPath cubic_;
    QPainterPath shape_;
};

} //namespace fe
