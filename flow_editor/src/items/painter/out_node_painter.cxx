//
// Created by cxc on 2024/3/12.
//

#include "out_node_painter.hpp"
#include <QPainter>

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
    painter->drawRect(components.port_rect2);
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
        PortType port_type = draft_item->requiredPort();
        if (port_type != PortType::In)
        {
            return;
        }

        QPointF p = components.port_rect.center();                        //相对坐标
        QPointF p2 = item->mapFromScene(draft_item->endPoint(port_type)); //相对坐标
        if (item->getPortIndex(port_type, p2.toPoint()) < 0)
        {
            return;
        }

        //计算其比率
        double d2 = (p.x() - p2.x()) * (p.x() - p2.x()) + (p.y() - p2.y()) * (p.y() - p2.y());
        double w2 = components.port_rect.width() * components.port_rect.width() * 0.25;
        double r = std::max(2.0 - d2 / w2, 1.0);
        r = (r > 1.8) ? 2.0 : r;
        double rx = r * (components.port_rect.width() - style->pen_width) / 2.0;

        painter->setBrush(style->connection_point_color);
        painter->drawEllipse(p, rx, rx);
    }
}
} //namespace fe