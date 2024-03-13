//
// Created by cxc on 2024/3/12.
//

#include "in_node_painter.hpp"
#include <QPainter>
#include <iostream>

namespace fe
{
void InNodePainter::paint(QPainter* painter, InNodeItem* item)
{
    auto& style = item->style_;

    QFont font = style->font;
    painter->setFont(font);

    painter->setPen(QPen());
    painter->setBrush(Qt::NoBrush);

    //画框架(测试用)
    //paintFrame(painter, item);

    //画主体
    paintNodeRect(painter, item);

    //画标题
    paintCaption(painter, item);

    //画交互端口
    paintConnectionPoints(painter, item);
}
void InNodePainter::paintFrame(QPainter* painter, InNodeItem* item)
{
    //画各个区域(调试用)
    const auto& components = item->geometry_->components();
    painter->drawRect(components.bounding_rect);
    painter->drawRect(components.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.port_rect);
    painter->drawRect(components.port_rect_extend);
    painter->drawPath(item->shape_);
}
void InNodePainter::paintNodeRect(QPainter* painter, InNodeItem* item)
{
    QPen pen;
    bool is_selected = item->isSelected();
    auto color = is_selected ? item->style_->selected_boundary_color : item->style_->normal_boundary_color;
    auto width = is_selected ? item->style_->pen_width : item->style_->hovered_pen_width;
    pen.setColor(color);
    pen.setWidth(width);
    painter->setPen(pen);

    //画主区域
    const auto& components = item->geometry_->components();
    painter->setBrush(item->draft_connection_item_ ? QColor(0x546281) : QColor(0x123456));
    painter->drawPolygon(components.node_polygon);
}
void InNodePainter::paintCaption(QPainter* painter, InNodeItem* item)
{
    const auto& components = item->geometry_->components();
    double scale = item->scale_;
    bool is_selected = item->isSelected();

    QPen pen;
    auto color = is_selected ? item->style_->selected_boundary_color : item->style_->normal_boundary_color;
    auto width = is_selected ? item->style_->pen_width : item->style_->hovered_pen_width;
    pen.setColor(color);
    pen.setWidth(width);
    painter->setPen(pen);

    if (is_selected)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);
        painter->drawRect(components.caption_rect);
    }

    //绘画标题
    painter->setPen(item->style_->font_color);
    if (scale < 1.0)
    {
        painter->save();
        painter->scale(1.0 / scale, 1.0 / scale);
        painter->drawStaticText(components.caption_rect.topLeft() * scale, components.caption_text);
        painter->restore();
    }
    else
    {
        painter->drawStaticText(components.caption_rect.topLeft(), components.caption_text);
    }
}
void InNodePainter::paintConnectionPoints(QPainter* painter, InNodeItem* item)
{
    const auto& components = item->geometry_->components();

    QPen pen;
    auto color = item->isSelected() ? item->style_->selected_boundary_color : item->style_->normal_boundary_color;
    auto width = item->isSelected() ? item->style_->pen_width : item->style_->hovered_pen_width;
    pen.setColor(color);
    pen.setWidth(width);
    painter->setPen(pen);
    painter->setBrush(components.port_color);

    //画交互端口
    if (item->draft_connection_item_)
    {
        auto draft_item = item->draft_connection_item_;
        item->draft_connection_item_ = nullptr;

        PortType required_port = draft_item->requiredPort();
        if (required_port != PortType::Out)
        {
            return;
        }

        guid16 in_id = draft_item->id();
        unsigned int in_port_index = draft_item->portIndex();
        QPointF end_point = item->mapFromScene(draft_item->endPoint(required_port)); //相对坐标

        double half_rect_width = components.port_rect.width() * 0.5;
        double half_rect_extend_width = components.port_rect_extend.width() * 0.5;

        //d2: 鼠标到操作中心的距离
        QPointF p = components.port_rect.center();
        double d2 = (p.x() - end_point.x()) * (p.x() - end_point.x()) + (p.y() - end_point.y()) * (p.y() - end_point.y());
        double trigger_distance = half_rect_extend_width * half_rect_extend_width;

        //如果满足距离,则进行尺寸运算
        if (d2 < trigger_distance)
        {
            bool possible = item->scene_.flowSceneData()->checkConnectionPossible(item->id(), 0, in_id, in_port_index);
            double dr = d2 / trigger_distance;
            double r = possible ? std::max(2.0 - dr, 1.0) : std::min(dr, 1.0);
            double rx = r * half_rect_width;
            painter->drawEllipse(p, rx, rx);
        }
        else
        {
            painter->drawEllipse(components.port_rect);
        }
    }
    else
    {
        painter->drawEllipse(components.port_rect);
    }
}
} //namespace fe