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
NodeItem::NodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style) :
    data_(data),
    style_(style),
    dynamic_hor_geometry_(data_, style_)
{
    setPos(data_.position);
    dynamic_hor_geometry_.update(scale_);
    updateCache();
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
        paintTo(painter, dynamic_hor_geometry_.geometry(), 1.0, style_);
    }
    else
    {
        paintSimpleTo(painter, dynamic_hor_geometry_.simpleGeometry(), scale_, style_);
    }
}
void NodeItem::updateCache()
{
    if (scale_ < 0.5)
    {
        dynamic_hor_geometry_.updateSimple(scale_);
        bounding_rect_ = &dynamic_hor_geometry_.simpleGeometry().bounding_rect;
        crt_model_ = 1;
    }
    else
    {
        bounding_rect_ = &dynamic_hor_geometry_.geometry().bounding_rect;
        crt_model_ = 0;
    }
}
void NodeItem::paintTo(QPainter* painter, const NodeSubGeometry& geometry, double scale, std::shared_ptr<NodeStyle>& style)
{
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    painter->drawRect(geometry.bounding_rect);
    painter->drawRect(geometry.node_rect);
    painter->drawRect(geometry.title_rect);
    painter->drawRect(geometry.icon_rect);
    painter->drawRect(geometry.caption_rect);
    painter->drawRect(geometry.run_btn_rect);
    painter->drawRect(geometry.port_rect);
    for (size_t i = 0; i < geometry.in_port_rect.size(); ++i)
    {
        painter->drawRect(geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < geometry.out_port_rect.size(); ++i)
    {
        painter->drawRect(geometry.out_port_rect[i]);
    }
    for (size_t i = 0; i < geometry.in_port_text_rect.size(); ++i)
    {
        painter->drawRect(geometry.in_port_text_rect[i]);
    }
    for (size_t i = 0; i < geometry.out_port_text_rect.size(); ++i)
    {
        painter->drawRect(geometry.out_port_text_rect[i]);
    }

    //画主体
    //QBrush b = painter->brush();
    //painter->setBrush(QColor(0x606060));
    const double radius = 3.0;
    painter->drawRoundedRect(geometry.node_rect, radius, radius);
    //painter->setBrush(b);

    //画图标
    painter->fillRect(geometry.icon_rect, 0xC0C0C0);

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(geometry.caption_rect.topLeft() * scale, geometry.node_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(geometry.caption_rect.topLeft(), geometry.node_name);
    }

    //绘画输入输出名
    f.setBold(false);
    painter->setFont(f);
    for (size_t i = 0; i < geometry.in_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(geometry.in_port_text_rect[i].topLeft(), geometry.in_port_text[i]);
    }
    painter->setFont(font);
    for (size_t i = 0; i < geometry.out_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(geometry.out_port_text_rect[i].topLeft(), geometry.out_port_text[i]);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    painter->setBrush(QColor(255, 0, 0));
    for (size_t i = 0; i < geometry.in_port_rect.size(); ++i)
    {
        painter->drawEllipse(geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < geometry.out_port_rect.size(); ++i)
    {
        painter->drawEllipse(geometry.out_port_rect[i]);
    }
    painter->setBrush(brush);

    //绘画运行按钮
    painter->drawPolygon(geometry.run_polygon.data(), (int)geometry.run_polygon.size());
}
void NodeItem::paintSimpleTo(QPainter* painter, const NodeSubGeometrySimple& geometry_simple, double scale, std::shared_ptr<NodeStyle>& style)
{
    //保存状态
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    painter->drawRect(geometry_simple.bounding_rect);
    painter->drawRect(geometry_simple.icon_rect);
    painter->drawRect(geometry_simple.caption_rect);
    painter->drawRect(geometry_simple.in_port_rect);
    painter->drawRect(geometry_simple.out_port_rect);

    //画图标
    painter->fillRect(geometry_simple.icon_rect, 0xC0C0C0);

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(geometry_simple.caption_rect.topLeft() * scale, geometry_simple.node_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(geometry_simple.caption_rect.topLeft(), geometry_simple.node_name);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    painter->setBrush(QColor(255, 0, 0));
    painter->drawEllipse(geometry_simple.in_port_rect);
    painter->drawEllipse(geometry_simple.out_port_rect);
    painter->setBrush(brush);
}
} //namespace fe
