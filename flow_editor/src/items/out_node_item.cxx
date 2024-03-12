//
// Created by cxc on 2024/3/7.
//

#include "out_node_item.hpp"
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <iostream>
#include <QElapsedTimer>
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/painter/out_node_painter.hpp>

namespace fe
{
OutNodeItem::OutNodeItem(FlowScene& scene, const guid16& id) :
    AbsNodeItem(scene, id)
{
    //查询配置属性
    FlowSceneData* flow_data = scene.flowSceneData();
    auto itr = flow_data->out_node_items.find(id);
    if (itr == flow_data->out_node_items.end())
    {
        setEnabled(false);
        return;
    }
    data_ = &itr->second->data;
    geometry_ = itr->second->geometry.get();
    style_ = itr->second->node_style;
    z_value_ = itr->second->z_value;

    //初始化
    setPos(data_->position);
    setZValue(z_value_);
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    updateCache(1.0);

    //添加显示对象
    itr->second->draw_item = this;
    scene.addItem(this);
}
void OutNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    updateCache(lod);
    OutNodePainter::painter(painter, this);
}
void OutNodeItem::updateCache(double scale)
{
    if (scale_ != scale)
    {
        scale_ = scale;
        geometry_->update(scale);
        QPainterPath polygon_path;
        polygon_path.addPolygon(geometry_->components().node_polygon);
        QPainterPath rect_path;
        rect_path.addRect(geometry_->components().port_rect2);
        shape_ = polygon_path.united(rect_path);
        prepareGeometryChange();
        moveConnections();
    }
}
void OutNodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (geometry_->components().port_rect.contains(event->pos()))
    {
        scene_.flowSceneData()->makeDraftConnection(PortType::Out, id_, 0, mapToScene(event->pos()), id_);
        draft_connection_item_ = scene_.flowSceneData()->draft_connection.get();
        update();
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}
void OutNodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);
}
void OutNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
void OutNodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    //改变鼠标样式
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}
int OutNodeItem::getPortIndex(PortType required_port, const QPoint& pos) const
{
    if (required_port != PortType::In)
    {
        return -1;
    }
    if (geometry_->components().port_rect.contains(pos))
    {
        return 0;
    }
    return -1;
}
} //namespace fe
