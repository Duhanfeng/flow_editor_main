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
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/painter/node_painter.hpp>

namespace fe
{
NodeItem::NodeItem(FlowScene& scene, const guid16& id) :
    AbsNodeItem(scene, id)
{
    //查询配置属性
    FlowSceneData* flow_data = scene.flowSceneData();
    auto itr = flow_data->node_items.find(id);
    if (itr == flow_data->node_items.end())
    {
        setEnabled(false);
        return;
    }
    data_ = &itr->second->data;
    geometry_ = itr->second->geometry.get();
    style_ = itr->second->node_style;
    z_value_ = itr->second->z_value;
    preview_scale_ = flow_data->scene_config.preview_scale;

    icon_ = data_->node_icon.pixmap(QSize(32, 32));

    //初始化
    setPos(data_->position);
    setZValue(z_value_);
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable | ItemIsFocusable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    //setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    flow_permission_ = flow_data->scene_config.flow_permission;
    if (flow_permission_ & FlowPermission::NodeMovable)
    {
        setFlag(QGraphicsItem::ItemIsMovable);
    }

    updateCache(1.0);

    //添加显示对象
    itr->second->draw_item = this;
    scene.addItem(this);
}
void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    updateCache(lod);

    NodePainter::paint(painter, this);
}
void NodeItem::updateCache(double scale)
{
    if (scale_ == scale)
    {
        return;
    }
    scale_ = scale;
    if (scale_ <= preview_scale_)
    {
        geometry_->updateSimple(scale_);
        bounding_rect_ = &geometry_->simpleComponents().bounding_rect;
        crt_model_ = 1;
    }
    else
    {
        bounding_rect_ = &geometry_->components().bounding_rect;
        crt_model_ = 0;
    }
    moveConnections();
    prepareGeometryChange();
}

int NodeItem::getPortIndex(PortType required_port, const QPoint& pos) const
{
    if (required_port == PortType::Out)
    {
        for (size_t i = 0; i < geometry_->components().out_ports.size(); ++i)
        {
            if (geometry_->components().out_ports[i].port_rect.contains(pos))
            {
                return (int)i;
            }
        }
    }
    else if (required_port == PortType::In)
    {
        for (size_t i = 0; i < geometry_->components().in_ports.size(); ++i)
        {
            if (geometry_->components().in_ports[i].port_rect.contains(pos))
            {
                return (int)i;
            }
        }
    }

    return -1;
}
void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();

    if (!isSelected() && event->modifiers() != Qt::CTRL)
    {
        scene_.clearSelection();
        setSelected(true);
    }
    if (event->button() == Qt::RightButton)
    {
        //如果有多个节点,则
        QList<QGraphicsItem*> items = scene_.selectedItems();
        if (items.size() > 1)
        {
            std::vector<guid16> node_ids;
            for (const auto& item : items)
            {
                if (AbsNodeItem* node_item = dynamic_cast<AbsNodeItem*>(item))
                {
                    node_ids.emplace_back(node_item->id());
                }
            }
            if (node_ids.size() > 1)
            {
                emit scene_.nodesContextMenu(node_ids, mapToScene(event->pos()));
            }
            else if (node_ids.size() == 1)
            {
                emit scene_.nodeContextMenu(node_ids[0], mapToScene(event->pos()));
            }
        }
        else
        {
            emit scene_.nodeContextMenu(id_, mapToScene(event->pos()));
        }
        return;
    }

    //判断是否为按键点击事件
    auto pos = event->pos();
    if (geometry_->components().run_btn_rect2.contains(pos))
    {
        is_checked_btn_ = true;
        emit scene_.runBtnClicked(id_);
        //std::cout << "is_checked_btn_ = true" << std::endl;
        update();
        //触发相应信号
        return;
    }

    //判断点击了对应的端口位置
    if (flow_permission_ & FlowPermission::ConnectionEditable)
    {
        for (size_t i = 0; i < geometry_->components().in_ports.size(); ++i)
        {
            if (geometry_->components().in_ports[i].port_rect.contains(pos))
            {
                scene_.flowSceneData()->makeDraftConnection(PortType::Out, id_, (unsigned int)i, mapToScene(pos), id_);
                return;
            }
        }
        for (size_t i = 0; i < geometry_->components().out_ports.size(); ++i)
        {
            if (geometry_->components().out_ports[i].port_rect.contains(pos))
            {
                scene_.flowSceneData()->makeDraftConnection(PortType::In, id_, (unsigned int)i, mapToScene(pos), id_);
                return;
            }
        }
    }

    //QGraphicsItem::mousePressEvent(event);
}
void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (is_checked_btn_)
    {
        event->accept();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}
void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (is_checked_btn_)
    {
        is_checked_btn_ = false;
        //std::cout << "is_checked_btn_ = false" << std::endl;
        update();
        event->accept();
        return;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    //判断是否为按键点击事件
    auto pos = event->pos();
    if (geometry_->components().run_btn_rect2.contains(pos))
    {
        is_checked_btn_ = true;
        emit scene_.runBtnClicked(id_);
        update();
    }
    else
    {
        emit scene_.nodeDoubleClicked(id_);
    }
}
void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    setCursor(Qt::ArrowCursor);
    setZValue(scene_.flowSceneData()->top_node_z_value);
    is_hovered_ = true;
    QGraphicsItem::hoverEnterEvent(event);
}
void NodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    if (crt_model_ == 0 && geometry_->components().run_btn_rect2.contains(event->pos()))
    {
        if (!is_hovered_btn_)
        {
            is_hovered_btn_ = true;
            update();
            event->accept();
            return;
        }
    }
    else
    {
        if (is_hovered_btn_)
        {
            is_hovered_btn_ = false;
            update();
            event->accept();
            return;
        }
    }
    //QGraphicsItem::hoverMoveEvent(event);
}
void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    setZValue(z_value_);
    is_hovered_ = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

} //namespace fe
