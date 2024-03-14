//
// Created by cxc on 2024/3/7.
//

#include "in_node_item.hpp"
#include <iostream>
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <QElapsedTimer>
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/painter/in_node_painter.hpp>

namespace fe
{
InNodeItem::InNodeItem(FlowScene& scene, const guid16& id) :
    AbsNodeItem(scene, id)
{
    //查询配置属性
    FlowSceneData* flow_data = scene.flowSceneData();
    auto itr = flow_data->in_node_items.find(id);
    if (itr == flow_data->in_node_items.end())
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
    setFlags(ItemIsSelectable | ItemIsFocusable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    flow_permission_ = flow_data->scene_config.flow_permission;
    if (flow_permission_ & FlowPermission::NodeMovable)
    {
        setFlag(QGraphicsItem::ItemIsMovable);
    }

    updateCache(1.0);

    //添加显示对象
    itr->second->draw_item = this;
    scene.addItem(this);
}
void InNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    updateCache(lod);
    InNodePainter::paint(painter, this);
}
void InNodeItem::updateCache(double scale)
{
    if (scale_ != scale)
    {
        scale_ = scale;
        geometry_->update(scale);
        QPainterPath polygon_path;
        polygon_path.addPolygon(geometry_->components().node_polygon);
        QPainterPath rect_path;
        rect_path.addRect(geometry_->components().port_rect_extend);
        shape_ = polygon_path.united(rect_path);
        prepareGeometryChange();
        moveConnections();
    }
}
void InNodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    if (!isSelected() && event->modifiers() != Qt::CTRL)
    {
        scene_.clearSelection();
        setSelected(true);
    }
    if (event->button() == Qt::RightButton)
    {
        //如果有多个节点,则
        QList<QGraphicsItem*> items = scene_.selectedItems();
        if (items.size() > 1)
        {
            std::vector<guid16> node_ids;
            for (const auto& item : items)
            {
                if (AbsNodeItem* node_item = dynamic_cast<AbsNodeItem*>(item))
                {
                    node_ids.emplace_back(node_item->id());
                }
            }
            if (node_ids.size() > 1)
            {
                emit scene_.nodesContextMenu(node_ids, mapToScene(event->pos()));
            }
            else if (node_ids.size() == 1)
            {
                emit scene_.nodeContextMenu(node_ids[0], mapToScene(event->pos()));
            }
        }
        else
        {
            emit scene_.nodeContextMenu(id_, mapToScene(event->pos()));
        }
        return;
    }

    if ((flow_permission_ & FlowPermission::ConnectionEditable) && geometry_->components().port_rect_extend.contains(event->pos()))
    {
        scene_.flowSceneData()->makeDraftConnection(PortType::In, id_, 0, mapToScene(event->pos()), id_);
    }
    //else
    //{
    //    QGraphicsItem::mousePressEvent(event);
    //}
}
void InNodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);
}
void InNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
void InNodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    if (event->button() != Qt::LeftButton)
    {
        return;
    }
    emit scene_.nodeDoubleClicked(id_);
}
void InNodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    //改变鼠标样式
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}
int InNodeItem::getPortIndex(PortType required_port, const QPoint& pos) const
{
    if (required_port != PortType::Out)
    {
        return -1;
    }
    if (geometry_->components().port_rect_extend.contains(pos))
    {
        return 0;
    }
    return -1;
}
} //namespace fe
