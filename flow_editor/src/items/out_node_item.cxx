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
#include <src/node_sub_geometry/hor_geometry_compute.hpp>
#include <iostream>
#include <QElapsedTimer>

namespace fe
{
OutNodeItem::OutNodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style) :
    data_(data),
    style_(style),
    dynamic_hor_geometry_(data_, style_)
{
    updateCache();
}
QRectF OutNodeItem::boundingRect() const
{
    return dynamic_hor_geometry_.geometry().bounding_rect;
}
void OutNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    if (scale_ != lod)
    {
        scale_ = lod;
        if (scale_ < 0.5)
        {
            dynamic_hor_geometry_.updateSimple(scale_);
            bounding_rect_ = &dynamic_hor_geometry_.simpleGeometry().bounding_rect;
            crt_model_ = 1;
        }
        else
        {
            //dynamic_hor_geometry_.update(1.0);
            bounding_rect_ = &dynamic_hor_geometry_.geometry().bounding_rect;
            crt_model_ = 0;
        }
    }

    if (crt_model_ == 0)
    {
        paintTo(painter, 1.0, data_, dynamic_hor_geometry_.geometry(), style_);
    }
    else
    {
        paintSimpleTo(painter, scale_, data_, dynamic_hor_geometry_.simpleGeometry(), style_);
    }
}
void OutNodeItem::updateCache()
{
    setPos(data_.position);
    dynamic_hor_geometry_.update(scale_);
    if (scale_ < 0.5)
    {
        bounding_rect_ = &dynamic_hor_geometry_.geometry().bounding_rect;
    }
    else
    {
        bounding_rect_ = &dynamic_hor_geometry_.simpleGeometry().bounding_rect;
    }
}
void OutNodeItem::paintTo(QPainter* painter, double scale, const NodeData& data, const NodeSubGeometry& node_sub_geometry, std::shared_ptr<NodeStyle>& style)
{
    QPen pen = painter->pen();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    painter->drawRect(node_sub_geometry.bounding_rect);
    painter->drawRect(node_sub_geometry.node_rect);
    painter->drawRect(node_sub_geometry.title_rect);
    painter->drawRect(node_sub_geometry.icon_rect);
    painter->drawRect(node_sub_geometry.caption_rect);
    painter->drawRect(node_sub_geometry.run_btn_rect);
    painter->drawRect(node_sub_geometry.port_rect);
    for (size_t i = 0; i < node_sub_geometry.in_port_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.out_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.in_port_text_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.in_port_text_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_text_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.out_port_text_rect[i]);
    }

    //画主体
    //QBrush b = painter->brush();
    //painter->setBrush(QColor(0x606060));
    const double radius = 3.0;
    painter->drawRoundedRect(node_sub_geometry.node_rect, radius, radius);
    //painter->setBrush(b);

    //画图标
    painter->fillRect(node_sub_geometry.icon_rect, 0xC0C0C0);

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(node_sub_geometry.caption_rect.topLeft() * scale, node_sub_geometry.node_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(node_sub_geometry.caption_rect.topLeft(), node_sub_geometry.node_name);
    }

    //绘画输入输出名
    f.setBold(false);
    painter->setFont(f);
    for (size_t i = 0; i < node_sub_geometry.in_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(node_sub_geometry.in_port_text_rect[i].topLeft(), node_sub_geometry.in_port_text[i]);
    }
    painter->setFont(font);
    for (size_t i = 0; i < node_sub_geometry.out_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(node_sub_geometry.out_port_text_rect[i].topLeft(), node_sub_geometry.out_port_text[i]);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    for (size_t i = 0; i < node_sub_geometry.in_port_rect.size(); ++i)
    {
        painter->drawEllipse(node_sub_geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_rect.size(); ++i)
    {
        painter->drawEllipse(node_sub_geometry.out_port_rect[i]);
    }

    //绘画运行按钮
    painter->drawPolygon(node_sub_geometry.run_polygon.data(), (int)node_sub_geometry.run_polygon.size());
}
void OutNodeItem::paintSimpleTo(QPainter* painter, double scale, const NodeData& data, const NodeSubGeometry& node_sub_geometry, std::shared_ptr<NodeStyle>& style)
{
    QPen pen = painter->pen();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    painter->drawRect(node_sub_geometry.bounding_rect);
    painter->drawRect(node_sub_geometry.node_rect);
    painter->drawRect(node_sub_geometry.icon_rect);
    painter->drawRect(node_sub_geometry.caption_rect);
    for (size_t i = 0; i < node_sub_geometry.in_port_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.out_port_rect[i]);
    }

    //画主体
    const double radius = 3.0;
    painter->drawRoundedRect(node_sub_geometry.node_rect, radius, radius);

    //画图标
    painter->fillRect(node_sub_geometry.icon_rect, 0xC0C0C0);

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(node_sub_geometry.caption_rect.topLeft() * scale, node_sub_geometry.node_name);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(node_sub_geometry.caption_rect.topLeft(), node_sub_geometry.node_name);
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    for (size_t i = 0; i < node_sub_geometry.in_port_rect.size(); ++i)
    {
        painter->drawEllipse(node_sub_geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_rect.size(); ++i)
    {
        painter->drawEllipse(node_sub_geometry.out_port_rect[i]);
    }
}
} //namespace fe
