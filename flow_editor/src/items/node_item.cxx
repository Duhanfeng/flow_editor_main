//
// Created by cxc on 2024/3/7.
//

#include "node_item.hpp"
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <iostream>
#include <QElapsedTimer>
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/painter/node_painter.hpp>

namespace fe
{
NodeItem::NodeItem(FlowScene& scene, const guid16& id) :
    AbsNodeItem(scene, id)
{
    //查询配置属性
    FlowSceneData* flow_data = scene.flowSceneData();
    auto itr = flow_data->node_items.find(id);
    if (itr == flow_data->node_items.end())
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
    //setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    geometry_->update(1.0);
    updateCache(1.0);

    //添加显示对象
    itr->second->draw_item = this;
    scene.addItem(this);
}
void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    updateCache(lod);

    NodePainter::painter(painter, this);
}
void NodeItem::updateCache(double scale)
{
    if (scale_ == scale)
    {
        return;
    }
    scale_ = scale;
    if (scale_ < 0.5)
    {
        geometry_->updateSimple(scale_);
        bounding_rect_ = &geometry_->simpleComponents().bounding_rect;
        crt_model_ = 1;
    }
    else
    {
        bounding_rect_ = &geometry_->components().bounding_rect;
        crt_model_ = 0;
    }
    moveConnections();
}
void NodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}
int NodeItem::getPortIndex(PortType required_port, const QPoint& pos) const
{
    if (required_port == PortType::Out)
    {
        for (size_t i = 0; i < geometry_->components().out_port_rect.size(); ++i)
        {
            if (geometry_->components().out_port_rect[i].contains(pos))
            {
                return (int)i;
            }
        }
    }
    else if (required_port == PortType::In)
    {
        for (size_t i = 0; i < geometry_->components().in_port_rect.size(); ++i)
        {
            if (geometry_->components().in_port_rect[i].contains(pos))
            {
                return (int)i;
            }
        }
    }

    return -1;
}

} //namespace fe
