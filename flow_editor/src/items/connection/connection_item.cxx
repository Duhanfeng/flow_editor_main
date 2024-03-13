//
// Created by cxc on 2024/3/9.
//

#include "connection_item.hpp"
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <src/flow_view/flow_scene_data.hpp>

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
ConnectionItem::ConnectionItem(FlowScene& scene, const guid18& id) :
    scene_(scene),
    id_(id)
{
    //查询配置属性
    FlowSceneData* flow_data = scene.flowSceneData();
    auto itr = flow_data->connection_items.find(id);
    if (itr == flow_data->connection_items.end())
    {
        setEnabled(false);
        return;
    }

    connection_ = &itr->second->connection;
    style_ = itr->second->style;
    z_value_ = itr->second->z_value;
    preview_scale_ = flow_data->scene_config.preview_scale;

    setZValue(z_value_);
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable | ItemIsFocusable);
    //setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    move();

    //添加显示对象
    itr->second->draw_item = this;
    scene.addItem(this);
}

void ConnectionItem::updateCache()
{
    if (start_ == end_)
    {
        bounding_rect_ = {};
    }
    else
    {
        //-----绘画曲线
        QPoint c1, c2;
        pointsC1C2Horizontal(start_, end_, c1, c2);
        cubic_ = QPainterPath(start_);
        cubic_.cubicTo(c1, c2, end_);

        //-----计算boundingRect
        QRectF basic_rect = QRectF(start_, end_).normalized();
        QRectF c1c2_rect = QRectF(c1, c2).normalized();
        //获取基本矩形和控制点矩形的并集，得到一个包含连接所有相关点的矩形。
        QRectF common_rect = basic_rect.united(c1c2_rect);
        float const diam = style_->point_diameter;
        //创建一个偏移量，用于扩展矩形。
        QPointF const corner_offset(diam, diam);
        //通过端口圆直径扩展矩形。
        //将矩形左上角向左上移动直径的距离，以确保连接的起点和终点都被包含在内。
        common_rect.setTopLeft(common_rect.topLeft() - corner_offset);
        //将矩形右下角向右下移动两倍直径的距离，以确保连接的曲线部分也被包含在内。
        common_rect.setBottomRight(common_rect.bottomRight() + 2 * corner_offset);
        bounding_rect_ = common_rect;

        //-----计算shape
        QPainterPath shape(start_);
        unsigned segments = 20;
        for (auto i = 0ul; i < segments; ++i)
        {
            double ratio = double(i + 1) / segments;
            shape.lineTo(cubic_.pointAtPercent(ratio));
        }
        QPainterPathStroker stroker;
        stroker.setWidth(10.0);
        shape_ = stroker.createStroke(shape);
    }
    prepareGeometryChange();
    update();
}
void ConnectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    const bool hovered = hovered_;
    const bool selected = isSelected();

    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    if (lod != scale_)
    {
        scale_ = lod;
        move();
    }

    painter->save();
    if (hovered || selected)
    {
        double const line_width = style_->line_width;
        QPen pen;
        pen.setWidth(2 * line_width);
        pen.setColor(selected ? style_->selected_color : style_->hovered_color);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(cubic_);
    }

    //drawNormalLine
    {
        double const line_width = style_->line_width;
        QPen p;
        p.setWidth(line_width);
        p.setColor(style_->normal_color);
        if (selected)
        {
            p.setColor(style_->selected_halo_color);
        }
        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(cubic_);
    }

    //draw end points
    //const double point_diameter = style_->point_diameter;
    //painter->setPen(style_->construction_color);
    //painter->setBrush(style_->construction_color);
    //double const point_radius = point_diameter / 2.0;
    //painter->drawEllipse(start_.toPointF(), point_radius, point_radius);
    //painter->drawEllipse(end_.toPointF(), point_radius, point_radius);

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
void ConnectionItem::move()
{
    auto flow_scene_data = scene_.flowSceneData();
    if (scale_ > preview_scale_)
    {
        QPointF start;
        flow_scene_data->getNodePortPosition(connection_->out, PortType::Out, connection_->out_port, start);
        QPointF end;
        flow_scene_data->getNodePortPosition(connection_->in, PortType::In, connection_->in_port, end);
        updateCache(start.toPoint(), end.toPoint());
    }
    else
    {
        QPointF start;
        flow_scene_data->getNodePreviewPortPosition(connection_->out, PortType::Out, connection_->out_port, start);
        QPointF end;
        flow_scene_data->getNodePreviewPortPosition(connection_->in, PortType::In, connection_->in_port, end);
        updateCache(start.toPoint(), end.toPoint());
    }
}

void ConnectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);
}
void ConnectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
void ConnectionItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    //改变鼠标样式
    setCursor(Qt::ArrowCursor);
}
void ConnectionItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    hovered_ = true;
    update();
    event->accept();
}
void ConnectionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    hovered_ = false;
    update();
    event->accept();
}

} //namespace fe