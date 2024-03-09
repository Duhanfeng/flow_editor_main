//
// Created by cxc on 2024/3/9.
//

#include "connection_item.hpp"
#include <QPen>
#include <QPainter>

namespace
{
void pointsC1C2Horizontal(QPoint start, QPoint end, QPoint& c1, QPoint& c2)
{
    double const default_offset = 200;
    double x_distance = end.x() - start.x();
    double horizontal_offset = qMin(default_offset, std::abs(x_distance));
    double vertical_offset = 0;
    double ratio_x = 0.5;
    if (x_distance <= 0)
    {
        double y_distance = end.y() - start.y() + 20;
        double vector = y_distance < 0 ? -1.0 : 1.0;
        vertical_offset = qMin(default_offset, std::abs(y_distance)) * vector;
        ratio_x = 1.0;
    }

    horizontal_offset *= ratio_x;
    c1 = QPoint(start.x() + horizontal_offset, start.y() + vertical_offset);
    c2 = QPoint(end.x() - horizontal_offset, end.y() - vertical_offset);
}
} //namespace

namespace fe
{
ConnectionItem::ConnectionItem(double z_value) :
    z_value_(z_value)
{
    updateCache();
}
void ConnectionItem::updateCache()
{
    if (start_ == end_)
    {
        bounding_rect_ = {};
    }
    else
    {
        //绘画曲线
        QPoint c1, c2;
        pointsC1C2Horizontal(start_, end_, c1, c2);
        cubic_ = QPainterPath(start_);
        cubic_.cubicTo(c1, c2, end_);
        bounding_rect_ = cubic_.boundingRect();
    }
    prepareGeometryChange();
}
QRectF ConnectionItem::boundingRect() const
{
    return bounding_rect_;
}
void ConnectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    painter->save();
    QPen pen;
    pen.setWidth(2.0);
    pen.setColor(0x125513);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->drawPath(cubic_);
    painter->restore();
}
void ConnectionItem::updateStart(const QPoint& value)
{
    if (start_ == value)
    {
        return;
    }
    start_ = value;
    updateCache();
}
void ConnectionItem::updateEnd(const QPoint& value)
{
    if (end_ == value)
    {
        return;
    }
    end_ = value;
    updateCache();
}
void ConnectionItem::updateCache(const QPoint& start, const QPoint& end)
{
    if ((start_ == start) && (end_ == end))
    {
        return;
    }
    start_ = start;
    end_ = end;
    updateCache();
}
} //namespace fe