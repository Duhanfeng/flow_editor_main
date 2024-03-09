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

namespace fe
{
NodeItem::NodeItem(NodeData& data, DynamicHGeometry& geometry, std::shared_ptr<NodeStyle> style, double z_value) :
    data_(data),
    geometry_(geometry),
    style_(style),
    z_value_(z_value)
{
    setPos(data_.position);
    setZValue(z_value_);

    geometry_.update(scale_);
    updateCache();
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);
}
void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    if (scale_ != lod)
    {
        scale_ = lod;
        updateCache();
    }

    if (crt_model_ == 0)
    {
        paintTo(painter, geometry_.components(), 1.0, style_);
    }
    else
    {
        paintSimpleTo(painter, geometry_.simpleComponents(), scale_, style_);
    }
}
void NodeItem::updateCache()
{
    if (scale_ < 0.5)
    {
        geometry_.updateSimple(scale_);
        bounding_rect_ = &geometry_.simpleComponents().bounding_rect;
        crt_model_ = 1;
    }
    else
    {
        bounding_rect_ = &geometry_.components().bounding_rect;
        crt_model_ = 0;
    }
}
void NodeItem::paintTo(QPainter* painter, const NodeUIComponents& components, double scale, std::shared_ptr<NodeStyle>& style)
{
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    //painter->drawRect(geometry.bounding_rect);
    painter->drawRect(components.node_rect);
    painter->drawRect(components.title_rect);
    painter->drawRect(components.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.run_btn_rect);
    painter->drawRect(components.port_rect);
    for (size_t i = 0; i < components.in_port_rect.size(); ++i)
    {
        painter->drawRect(components.in_port_rect[i]);
    }
    for (size_t i = 0; i < components.out_port_rect.size(); ++i)
    {
        painter->drawRect(components.out_port_rect[i]);
    }
    for (size_t i = 0; i < components.in_port_text_rect.size(); ++i)
    {
        painter->drawRect(components.in_port_text_rect[i]);
    }
    for (size_t i = 0; i < components.out_port_text_rect.size(); ++i)
    {
        painter->drawRect(components.out_port_text_rect[i]);
    }

    //画主体
    //QBrush b = painter->brush();
    //painter->setBrush(QColor(0x606060));
    const double radius = 3.0;
    painter->drawRoundedRect(components.node_rect, radius, radius);
    //painter->setBrush(b);

    //画图标
    painter->fillRect(components.icon_rect, 0xC0C0C0);

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(components.caption_rect.topLeft() * scale, components.node_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(components.caption_rect.topLeft(), components.node_name);
    }

    //绘画输入输出名
    f.setBold(false);
    painter->setFont(f);
    for (size_t i = 0; i < components.in_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(components.in_port_text_rect[i].topLeft(), components.in_port_text[i]);
    }
    painter->setFont(font);
    for (size_t i = 0; i < components.out_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(components.out_port_text_rect[i].topLeft(), components.out_port_text[i]);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    painter->setBrush(QColor(255, 0, 0));
    for (size_t i = 0; i < components.in_port_rect.size(); ++i)
    {
        painter->drawEllipse(components.in_port_rect[i]);
    }
    for (size_t i = 0; i < components.out_port_rect.size(); ++i)
    {
        painter->drawEllipse(components.out_port_rect[i]);
    }
    painter->setBrush(brush);

    //绘画运行按钮
    painter->setBrush(QColor(0, 255, 0));
    painter->drawPolygon(components.run_polygon.data(), (int)components.run_polygon.size());
    painter->setBrush(brush);
}
void NodeItem::paintSimpleTo(QPainter* painter, const NodeUISimpleComponents& simple_components, double scale, std::shared_ptr<NodeStyle>& style)
{
    //保存状态
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    //painter->drawRect(geometry_simple.bounding_rect);
    painter->drawRect(simple_components.icon_rect);
    painter->drawRect(simple_components.caption_rect);
    painter->drawRect(simple_components.in_port_rect);
    painter->drawRect(simple_components.out_port_rect);

    //画图标
    painter->fillRect(simple_components.icon_rect, 0xC0C0C0);

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(simple_components.caption_rect.topLeft() * scale, simple_components.node_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(simple_components.caption_rect.topLeft(), simple_components.node_name);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    painter->setBrush(QColor(255, 0, 0));
    painter->drawEllipse(simple_components.in_port_rect);
    painter->drawEllipse(simple_components.out_port_rect);
    painter->setBrush(brush);
}
void NodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}
} //namespace fe
