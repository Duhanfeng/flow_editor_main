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
#include <src/flow_view/flow_scene_data.hpp>
//#include "connection_item.hpp"

namespace fe
{
OutNodeItem::OutNodeItem(FlowScene& scene, const guid16& id) :
    AbsNodeItem(scene, id)
{
    //查询配置属性
    FlowSceneData* flow_data = scene.flowSceneData();
    auto itr = flow_data->out_node_items.find(id);
    if (itr == flow_data->out_node_items.end())
    {
        setEnabled(false);
        return;
    }
    data_ = &itr->second->data;
    geometry_ = itr->second->geometry.get();
    style_ = itr->second->node_style;
    z_value_ = itr->second->z_value;

    //初始化
    setPos(data_->position);
    setZValue(z_value_);
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    updateCache(1.0);

    //添加显示对象
    itr->second->draw_item = this;
    scene.addItem(this);
}
void OutNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    updateCache(lod);
    painter->save();
    paintTo(painter);
    painter->restore();
}
void OutNodeItem::updateCache(double scale)
{
    if (scale_ != scale)
    {
        scale_ = scale;
        geometry_->update(scale);
        shape_.clear();
        shape_.addPolygon(geometry_->components().node_polygon);
        shape_.addRect(geometry_->components().port_rect);
        prepareGeometryChange();
        moveConnections();
    }
}
void OutNodeItem::paintTo(QPainter* painter)
{
    double scale = scale_;
    const auto& components = geometry_->components();
    auto& style = style_;

    QPen pen = painter->pen();
    QBrush brush = painter->brush();
    QFont font = style->font;
    painter->setFont(font);

    //画各个区域(调试用)
    painter->setPen(pen);
    //painter->drawRect(components.bounding_rect);
    //painter->drawRect(geometry.icon_rect);
    painter->drawRect(components.caption_rect);
    painter->drawRect(components.port_rect);

    //画主区域
    painter->setBrush(QColor(0x123456));
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
    //painter->setPen(pen);
    //painter->setBrush(QColor(255, 0, 0));
    //painter->drawEllipse(components.port_rect);
    //painter->setBrush(brush);

    //画交互端口
    if (draft_connection_item_)
    {
        //std::cout << "draft_connection_item_" << std::endl;
        PortType port_type = draft_connection_item_->requiredPort();
        if (port_type == PortType::Out)
        {
            return;
        }

        QPointF p = components.port_rect.center();

        auto cp = draft_connection_item_->sceneTransform().map(draft_connection_item_->endPoint(port_type));
        cp = draft_connection_item_->sceneTransform().inverted().map(cp);

        auto diff = cp - p;
        double dist = std::sqrt(QPointF::dotProduct(diff, diff));

        double r = 1.0;
        bool possible = false;
        if (possible)
        {
            double const thres = 20;
            r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
        }
        else
        {
            double const thres = 20;
            r = (dist < thres) ? (dist / thres) : 1.0;
        }

        painter->setBrush(style->connection_point_color);
        painter->drawEllipse(p, style->connection_point_diameter * r, style->connection_point_diameter * r);
        resetConnectionForReaction();
    }
}
void OutNodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (geometry_->components().port_rect.contains(event->pos()))
    {
        scene_.flowSceneData()->makeDraftConnection(PortType::Out, id_, 0);
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}
void OutNodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseMoveEvent(event);
}
void OutNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
void OutNodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    //改变鼠标样式
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}
int OutNodeItem::getPortIndex(PortType required_port, const QPoint& pos) const
{
    if (required_port != PortType::In)
    {
        return -1;
    }

    QPointF p1 = geometry_->components().port_rect.center();
    QLineF line(p1, pos);
    if (line.length() < style_->connection_point_diameter)
    {
        return 0;
    }

    return -1;
}
} //namespace fe
