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

namespace fe
{
OutNodeItem::OutNodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style) :
    data_(data),
    style_(style),
    dynamic_port_geometry_(data_, style_)
{
    setPos(data_.position);
    dynamic_port_geometry_.update(scale_);
    updateCache();
}
void OutNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    if (scale_ != lod)
    {
        scale_ = lod;
        dynamic_port_geometry_.update(lod);
    }
    paintTo(painter, dynamic_port_geometry_.geometry(), scale_, style_);
}
void OutNodeItem::updateCache()
{
}
void OutNodeItem::paintTo(QPainter* painter, const PortSubGeometry& geometry, double scale, std::shared_ptr<NodeStyle>& style)
{
    //保存状态
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    painter->drawRect(geometry.bounding_rect);
    //painter->drawRect(geometry.icon_rect);
    painter->drawRect(geometry.caption_rect);
    painter->drawRect(geometry.port_rect);

    //画主区域
    painter->setBrush(QColor(0x123456));
    painter->drawPolygon(geometry.node_polygon.data(), (int)geometry.node_polygon.size());
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
        painter->drawStaticText(geometry.caption_rect.topLeft() * scale, geometry.port_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(geometry.caption_rect.topLeft(), geometry.port_name);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    painter->setBrush(QColor(255, 0, 0));
    painter->drawEllipse(geometry.port_rect);
    painter->setBrush(brush);
}
} //namespace fe
