//
// Created by cxc on 2024/3/7.
//

#include "in_node_item.hpp"
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <iostream>
#include <QElapsedTimer>

namespace fe
{
InNodeItem::InNodeItem(NodeData& data, DynamicHPortGeometry& geometry, std::shared_ptr<NodeStyle> style, double z_value) :
    data_(data),
    geometry_(geometry),
    style_(style),
    z_value_(z_value)
{
    setPos(data_.position);
    setZValue(z_value_);

    geometry_.update(scale_);
    updateCache();
}
void InNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    if (scale_ != lod)
    {
        scale_ = lod;
        geometry_.update(lod);
    }
    paintTo(painter, geometry_.components(), scale_, style_);
}
void InNodeItem::updateCache()
{
}
void InNodeItem::paintTo(QPainter* painter, const PortUIComponents& components, double scale, std::shared_ptr<NodeStyle>& style)
{
    //保存状态
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    painter->drawRect(components.bounding_rect);
    //painter->drawRect(geometry.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.port_rect);

    //画主区域
    painter->setBrush(QColor(0x654321));
    painter->drawPolygon(components.node_polygon.data(), (int)components.node_polygon.size());
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
} //namespace fe
