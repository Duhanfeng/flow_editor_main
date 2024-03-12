//
// Created by cxc on 2024/3/12.
//

#include "node_painter.hpp"
#include <QPainter>
#include <iostream>

namespace fe
{
void NodePainter::paint(QPainter* painter, NodeItem* item)
{
    if (item->crt_model_ == 0)
    {
        paintNormal(painter, item);
    }
    else
    {
        paintSimple(painter, item);
    }
}
void NodePainter::paintNormal(QPainter* painter, NodeItem* item)
{
    const auto& components = item->geometry_->components();
    auto& style = item->style_;

    QFont font = style->font;
    painter->setFont(font);

    //画框架(测试用)
    //paintFrame(painter, item);

    //画主体
    paintNodeRect(painter, item);

    //画图标
    if (!item->icon_.isNull())
    {
        painter->drawPixmap(components.icon_rect.toRect(), item->icon_);
    }
    else
    {
        painter->fillRect(components.icon_rect, 0xC0C0C0);
    }

    //绘画标题
    painter->setPen(style->font_color);
    painter->drawStaticText(components.caption_position, components.caption_text);

    //绘画输入输出名
    for (const auto& port : components.in_ports)
    {
        painter->drawStaticText(port.port_text_position, port.port_text);
    }
    for (const auto& port : components.out_ports)
    {
        painter->drawStaticText(port.port_text_position, port.port_text);
    }

    //画交互点
    paintRunBtn(painter, item);

    //画连接点
    paintConnectionPoints(painter, item);
}

void NodePainter::paintFrame(QPainter* painter, NodeItem* item)
{
    const auto& components = item->geometry_->components();

    //画各个区域(调试用)
    painter->drawRect(components.bounding_rect);
    //painter->drawRect(components.node_rect);
    painter->drawRect(components.title_rect);
    painter->drawRect(components.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.run_btn_rect);
    painter->drawRect(components.port_rect);
    painter->drawPolygon(components.run_btn_polygon);

    for (size_t i = 0; i < components.in_ports.size(); ++i)
    {
        painter->drawRect(components.in_ports[i].port_rect);
        painter->drawRect(components.in_ports[i].port_rect_extend);
        painter->drawRect(components.in_ports[i].port_text_rect);
    }
    for (size_t i = 0; i < components.out_ports.size(); ++i)
    {
        painter->drawRect(components.out_ports[i].port_rect);
        painter->drawRect(components.out_ports[i].port_rect_extend);
        painter->drawRect(components.out_ports[i].port_text_rect);
    }
}
void NodePainter::paintNodeRect(QPainter* painter, NodeItem* item)
{
    const auto& components = item->geometry_->components();
    auto color = item->isSelected() ? item->style_->selected_boundary_color : item->style_->normal_boundary_color;
    if (item->is_hovered_)
    {
        QPen p(color, item->style_->hovered_pen_width);
        painter->setPen(p);
    }
    else
    {
        QPen p(color, item->style_->pen_width);
        painter->setPen(p);
    }
    QColor color2 = item->style_->gradient_color2;
    painter->setBrush(color2);

    double const radius = 3.0;
    painter->drawRoundedRect(components.node_rect, radius, radius);

    if (item->isSelected())
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);
        painter->drawRect(components.title_rect);
    }
}
void NodePainter::paintRunBtn(QPainter* painter, NodeItem* item)
{
    const auto& components = item->geometry_->components();

    painter->setPen(Qt::NoPen);
    double const radius = 3.0;

    if (item->is_checked_btn_)
    {
        painter->setBrush(item->style_->btn_hovered_color);
        painter->drawRoundedRect(components.run_btn_rect2, radius, radius);

        //painter->setBrush(item->style_->btn_checked_color);
        //painter->drawRoundedRect(components.run_btn_rect, radius, radius);

        painter->setBrush(item->style_->btn_checked_color);
        painter->drawPolygon(components.run_btn_polygon);
        return;
    }

    if (item->is_hovered_btn_)
    {
        painter->setBrush(item->style_->btn_hovered_color);
        painter->drawRoundedRect(components.run_btn_rect2, radius, radius);
    }
    painter->setBrush(item->style_->btn_color);
    painter->drawPolygon(components.run_btn_polygon);
}
void NodePainter::paintConnectionPoints(QPainter* painter, NodeItem* item)
{
    const auto& components = item->geometry_->components();
    //float diameter = item->style_->connection_point_diameter; //连接点直径

    QPen pen;
    auto color = item->isSelected() ? item->style_->selected_boundary_color : item->style_->normal_boundary_color;
    auto width = item->isSelected() ? item->style_->pen_width : item->style_->hovered_pen_width;
    pen.setColor(color);
    pen.setWidth(width);
    painter->setPen(pen);

    if (item->draft_connection_item_)
    {
        auto draft_item = item->draft_connection_item_;
        item->draft_connection_item_ = nullptr;

        PortType required_port = draft_item->requiredPort();
        if (required_port == PortType::In)
        {
            for (size_t i = 0; i < components.out_ports.size(); ++i)
            {
                painter->setBrush(components.out_ports[i].port_color);
                painter->drawEllipse(components.out_ports[i].port_rect);
            }

            guid16 out_id = draft_item->id();
            unsigned int out_port_index = draft_item->portIndex();
            QPointF end_point = item->mapFromScene(draft_item->endPoint(required_port)); //相对坐标
            for (size_t i = 0; i < components.in_ports.size(); ++i)
            {
                const PortComponent& port_component = components.in_ports[i];
                painter->setBrush(port_component.port_color);

                double half_rect_width = port_component.port_rect.width() * 0.5;

                //d2: 鼠标到操作中心的距离
                QPointF p = port_component.port_center;
                double d2 = (p.x() - end_point.x()) * (p.x() - end_point.x()) + (p.y() - end_point.y()) * (p.y() - end_point.y());
                double trigger_distance = 2 * port_component.port_rect_extend.width() * port_component.port_rect_extend.width();

                //如果满足距离,则进行尺寸运算
                if (d2 < trigger_distance)
                {
                    bool possible = item->scene_.flowSceneData()->checkConnectionPossible(out_id, out_port_index, item->id(), (unsigned int)i);

                    double dr = d2 / trigger_distance;
                    double r = possible ? std::max(2.0 - dr, 1.0) : std::min(dr, 1.0);
                    double rx = r * half_rect_width;

                    painter->drawEllipse(p, rx, rx);
                }
                else
                {
                    painter->drawEllipse(port_component.port_rect);
                }
            }
        }
        else if (required_port == PortType::Out)
        {
            for (size_t i = 0; i < components.in_ports.size(); ++i)
            {
                painter->setBrush(components.in_ports[i].port_color);
                painter->drawEllipse(components.in_ports[i].port_rect);
            }

            for (size_t i = 0; i < components.out_ports.size(); ++i)
            {
            }
        }
    }
    else
    {
        for (size_t i = 0; i < components.in_ports.size(); ++i)
        {
            painter->setBrush(components.in_ports[i].port_color);
            painter->drawEllipse(components.in_ports[i].port_rect);
        }
        for (size_t i = 0; i < components.out_ports.size(); ++i)
        {
            painter->setBrush(components.out_ports[i].port_color);
            painter->drawEllipse(components.out_ports[i].port_rect);
        }
    }
}
void NodePainter::paintSimple(QPainter* painter, NodeItem* item)
{
    double scale = item->scale_;
    const auto& components = item->geometry_->simpleComponents();
    auto& style = item->style_;

    //保存状态
    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    //painter->drawRect(geometry_simple.bounding_rect);
    painter->drawRect(components.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.in_port_rect);
    painter->drawRect(components.out_port_rect);

    //画图标
    if (!item->icon_.isNull())
    {
        painter->drawPixmap(components.icon_rect.toRect(), item->icon_);
    }
    else
    {
        painter->fillRect(components.icon_rect, 0xC0C0C0);
    }

    //绘画标题
    QFont f = painter->font();
    //f.setBold(true);
    painter->setFont(f);
    painter->setPen(style->font_color);
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

    //绘画输入输出端口操作点
    painter->setPen(pen);
    painter->setBrush(QColor(255, 0, 0));
    painter->drawEllipse(components.in_port_rect);
    painter->drawEllipse(components.out_port_rect);
    painter->setBrush(brush);
}
} //namespace fe