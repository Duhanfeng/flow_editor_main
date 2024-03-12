//
// Created by cxc on 2024/3/12.
//

#include "out_node_painter.hpp"
#include <QPainter>
#include <iostream>

namespace fe
{
void OutNodePainter::paint(QPainter* painter, OutNodeItem* item)
{
    double scale = item->scale_;
    const auto& components = item->geometry_->components();
    auto& style = item->style_;

    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    //painter->drawRect(components.bounding_rect);
    //painter->drawRect(geometry.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.port_rect_extend);
    //painter->fillPath(shape_, draft_connection_item_ ? QColor(0x546281) : QColor(0x123456));

    //画主区域
    painter->setBrush(item->draft_connection_item_ ? QColor(0x546281) : QColor(0x123456));
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

    //画交互端口
    if (item->draft_connection_item_)
    {
        auto draft_item = item->draft_connection_item_;
        item->draft_connection_item_ = nullptr;

        //std::cout << "draft_connection_item_" << std::endl;
        PortType required_port = draft_item->requiredPort();
        if (required_port != PortType::In)
        {
            return;
        }

        guid16 out_id = draft_item->id();
        unsigned int out_port_index = draft_item->portIndex();
        QPointF end_point = item->mapFromScene(draft_item->endPoint(required_port)); //相对坐标

        double half_rect_width = components.port_rect.width() * 0.5;

        //d2: 鼠标到操作中心的距离
        QPointF p = components.port_rect.center();
        double d2 = (p.x() - end_point.x()) * (p.x() - end_point.x()) + (p.y() - end_point.y()) * (p.y() - end_point.y());
        double trigger_distance = 2 * components.port_rect_extend.width() * components.port_rect_extend.width();

        //如果满足距离,则进行尺寸运算
        painter->setBrush(style->connection_point_color);
        if (d2 < trigger_distance)
        {
            bool possible = item->scene_.flowSceneData()->checkConnectionPossible(out_id, out_port_index, item->id(), 0);

            double dr = d2 / trigger_distance;
            double r = possible ? std::max(2.0 - dr, 1.0) : std::min(dr, 1.0);
            double rx = r * half_rect_width;
            std::cout << "r: " << r << std::endl;

            painter->drawEllipse(p, rx, rx);
        }
        else
        {
            painter->drawEllipse(components.port_rect);
        }
    }
}
} //namespace fe