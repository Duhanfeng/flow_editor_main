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
#include "connection_item.hpp"

namespace fe
{
OutNodeItem::OutNodeItem(NodeData& data, DynamicHPortGeometry& geometry, std::shared_ptr<NodeStyle> style, double z_value) :
    data_(data),
    geometry_(geometry),
    style_(style),
    z_value_(z_value)
{
    setPos(data_.position);
    setZValue(z_value_);

    updateCache(1.0);
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);
}
void OutNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    updateCache(lod);
    paintTo(painter, geometry_.components(), scale_, style_);
}
void OutNodeItem::updateCache(double scale)
{
    if (scale_ != scale)
    {
        scale_ = scale;
        geometry_.update(scale);
        if (connect_item_)
        {
            QPointF port_position = pos() + geometry_.components().port_rect.center();
            connect_item_->updateEnd(port_position.toPoint());
        }
        shape_.clear();
        shape_.addPolygon(geometry_.components().node_polygon);
        shape_.addRect(geometry_.components().port_rect);
        prepareGeometryChange();
    }
}
void OutNodeItem::paintTo(QPainter* painter, const PortUIComponents& components, double scale, std::shared_ptr<NodeStyle>& style)
{
    //保存状态
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    //painter->drawRect(components.bounding_rect);
    //painter->drawRect(geometry.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.port_rect);

    //画主区域
    painter->setBrush(QColor(0x123456));
    painter->drawPolygon(components.node_polygon);
    painter->setBrush(brush);

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(components.caption_rect.topLeft() * scale, components.port_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(components.caption_rect.topLeft(), components.port_name);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    painter->setBrush(QColor(255, 0, 0));
    painter->drawEllipse(components.port_rect);
    painter->setBrush(brush);
}
void OutNodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mousePressEvent(event);
}
void OutNodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);
    if (connect_item_)
    {
        QPointF port_position = pos() + geometry_.components().port_rect.center();
        connect_item_->updateEnd(port_position.toPoint());
    }
}
void OutNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
void OutNodeItem::setConnection(ConnectionItem* item)
{
    connect_item_ = item;
    if (connect_item_)
    {
        QPointF port_position = pos() + geometry_.components().port_rect.center();
        connect_item_->updateEnd(port_position.toPoint());
    }
}
void OutNodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    //改变鼠标样式
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}
} //namespace fe
