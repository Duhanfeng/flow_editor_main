//
// Created by cxc on 2024/3/11.
//

#include "draft_connection_item.hpp"
#include <iostream>
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/abs_node_item.hpp>
#include <src/items/connection/connection_item.hpp>

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

fe::AbsNodeItem* locateNodeAt(QPointF scene_point,
    QGraphicsScene& scene,
    QTransform const& view_transform)
{
    //获取场景中位于指定点（scene_point）的所有图形项，使用 Qt::IntersectsItemShape 算法，
    //以 Qt::DescendingOrder（即后绘制的项先被检索）的顺序，并考虑视图变换（view_transform）。
    QList<QGraphicsItem*> items = scene.items(scene_point,
        Qt::IntersectsItemShape,
        Qt::DescendingOrder,
        view_transform);
    //创建一个空的 vector 来存储过滤后的图形项。
    std::vector<QGraphicsItem*> filtered_items;
    //使用 std::copy_if 和 lambda 表达式来过滤 items 列表，只保留可以转换为 AbsNodeItem 的项。
    std::copy_if(items.begin(),
        items.end(),
        std::back_inserter(filtered_items),
        [](QGraphicsItem* item)
        {
            //使用 qgraphicsitem_cast 尝试将项转换为 AbsNodeItem*。
            //如果转换成功（即不为 nullptr），则返回 true，该项将被添加到 filtered_items 中。
            return (dynamic_cast<fe::AbsNodeItem*>(item) != nullptr);
        });
    fe::AbsNodeItem* node = nullptr;
    //如果过滤后的列表不为空，
    if (!filtered_items.empty())
    {
        //获取列表中的第一个图形项。
        QGraphicsItem* graphics_item = filtered_items.front();
        //使用 dynamic_cast 将图形项转换为 AbsNodeItem*。
        //dynamic_cast 在运行时进行检查，如果转换失败，则返回 nullptr。
        node = dynamic_cast<fe::AbsNodeItem*>(graphics_item);
    }
    //返回转换后的 AbsNodeItem 指针，如果列表为空或转换失败，则返回 nullptr。
    return node;
}
} //namespace

namespace fe
{
DraftConnectionItem::DraftConnectionItem(FlowScene& scene, PortType required_port, const guid16& id, unsigned int port_index, std::shared_ptr<DraftConnectionStyle> style) :
    scene_(scene),
    required_port_(required_port),
    id_(id),
    port_index_(port_index),
    style_(style)
{
    //setZValue(scene_.flowSceneData()->top_connection_z_value);
    setFlags(ItemIsSelectable | ItemIsFocusable);

    if (required_port_ == PortType::In)
    {
        QPointF out;
        scene_.flowSceneData()->getNodePortPosition(id_, PortType::Out, port_index_, out);
        out_ = out.toPoint();
        in_ = out_;
    }
    else if (required_port_ == PortType::Out)
    {
        QPointF in;
        scene_.flowSceneData()->getNodePortPosition(id_, PortType::In, port_index_, in);
        in_ = in.toPoint();
        out_ = in_;
    }
    updateCache();
    scene.addItem(this);
}
DraftConnectionItem::DraftConnectionItem(FlowScene& scene, PortType required_port, const guid16& id, unsigned int port_index, std::shared_ptr<DraftConnectionStyle> style, ConnectionItem* original_item) :
    DraftConnectionItem(scene, required_port, id, port_index, style)
{
    original_item_ = original_item;
    if (original_item_)
    {
        original_item_->setVisible(false);
    }
}
void DraftConnectionItem::setEndPoint(PortType port_type, const QPointF& point)
{
    if (port_type == PortType::In)
    {
        in_ = point.toPoint();
    }
    else
    {
        out_ = point.toPoint();
    }
    updateCache();
}
void DraftConnectionItem::updateCache()
{
    if (out_ == in_)
    {
        bounding_rect_ = {};
        shape_ = {};
    }
    else
    {
        //-----绘画曲线
        QPoint c1, c2;
        pointsC1C2Horizontal(out_, in_, c1, c2);
        cubic_ = QPainterPath(out_);
        cubic_.cubicTo(c1, c2, in_);

        //-----计算boundingRect
        QRectF basic_rect = QRectF(out_, in_).normalized();
        QRectF c1c2_rect = QRectF(c1, c2).normalized();
        QRectF common_rect = basic_rect.united(c1c2_rect);
        float const diam = style_->point_diameter;
        QPointF const corner_offset(diam, diam);
        common_rect.setTopLeft(common_rect.topLeft() - corner_offset);
        common_rect.setBottomRight(common_rect.bottomRight() + 2 * corner_offset);
        bounding_rect_ = common_rect;

        //-----计算shape
        QPainterPath shape(out_);
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

void DraftConnectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    painter->save();

    QPen pen;
    pen.setWidth(style_->construction_line_width);
    pen.setColor(style_->construction_color);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    //cubic spline
    painter->drawPath(cubic_);

    //draw end points
    const double point_diameter = style_->point_diameter;
    painter->setPen(style_->construction_color);
    painter->setBrush(style_->construction_color);
    double const point_radius = point_diameter / 2.0;
    painter->drawEllipse(out_.toPointF(), point_radius, point_radius);
    painter->drawEllipse(in_.toPointF(), point_radius, point_radius);

    painter->restore();
}
void DraftConnectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF end_point = event->pos();
    setEndPoint(required_port_, end_point);
    //更新图形
    event->accept();

    //if (required_port_ == PortType::In)
    //{
    //    QLineF line(out_, end_point);
    //    if (line.length() < 50)
    //    {
    //        return;
    //    }
    //}
    //else if (required_port_ == PortType::Out)
    //{
    //    QLineF line(in_, end_point);
    //    if (line.length() < 50)
    //    {
    //        return;
    //    }
    //}

    //std::cout << "event->pos" << event->pos().x() << "," << event->pos().y() << std::endl;
    //std::cout << "event->scenePos" << event->scenePos().x() << "," << event->scenePos().y() << std::endl;

    //将事件中的 widget 强制转换为 QGraphicsView 类型，以便获取视图信息。
    auto view = static_cast<QGraphicsView*>(event->widget());
    //在视图中的特定位置定位节点，这里的 locateNodeAt 是一个自定义函数。
    auto ngo = locateNodeAt(end_point, scene_, view->transform());
    //如果找到了节点（ngo 非空），则让节点响应连接(用于实现连接有效/无效的动画)
    if (ngo)
    {
        ngo->storeConnectionForReaction(this);
        ngo->update();
        last_hovered_node_ = ngo->id();
    }
    else
    {
        //如果没有找到节点，则重置最后一个悬停节点 ID。
        if (!isInvalid(last_hovered_node_))
        {
            auto item = scene_.flowSceneData()->absNodeItem(last_hovered_node_);
            if (item)
            {
                item->update();
            }
        }
        last_hovered_node_ = { 0 };
    }
}
void DraftConnectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    //查询当前位置的AbsNodeItem对象
    auto view = static_cast<QGraphicsView*>(event->widget());
    auto ngo = locateNodeAt(event->scenePos(), scene_, view->transform());
    if (ngo)
    {
        int port_index = ngo->getPortIndex(required_port_, mapToItem(ngo, event->scenePos()).toPoint());
        if (port_index >= 0)
        {
            Connection connection;
            if (required_port_ == PortType::In)
            {
                connection.out = id_;
                connection.out_port = port_index_;
                connection.in = ngo->id();
                connection.in_port = port_index;
            }
            else
            {
                connection.in = id_;
                connection.in_port = port_index_;
                connection.out = ngo->id();
                connection.out_port = port_index;
            }

            //如果环回,则直接退出
            //if (connection.in == connection.out)
            //{
            //    ungrabMouse();                                  //不再捕获鼠标事件。
            //    event->accept();                                //标记事件已被正确处理。
            //    scene_.flowSceneData()->resetDraftConnection(); //重置对象
            //    return;
            //}

            bool is_possible = scene_.flowSceneData()->checkConnectionPossible(connection.out, connection.out_port, connection.in, connection.in_port);

            //如果连接和原本的一致,则显示原有的对象
            if (original_item_)
            {
                if (connection.out == original_item_->connection()->out &&
                    connection.out_port == original_item_->connection()->out_port &&
                    connection.in == original_item_->connection()->in &&
                    connection.in_port == original_item_->connection()->in_port)
                {
                    original_item_->setVisible(true);
                }
                else
                {
                    if (scene_.flowModel()->tryDisconnect(original_item_->id()))
                    {
                        //尝试断开旧链接
                        scene_.flowSceneData()->removeConnection(original_item_->id());

                        if (is_possible)
                        {
                            //尝试连接
                            guid18 connection_id = scene_.flowModel()->tryConnect(connection);
                            if (!isInvalid(connection_id))
                            {
                                //内部添加新添加的连接对象
                                scene_.flowSceneData()->addConnection(connection_id, connection);
                            }
                        }
                    }
                    else
                    {
                        original_item_->setVisible(true);
                    }
                }
            }
            else
            {
                //尝试连接
                if (is_possible)
                {
                    guid18 connection_id = scene_.flowModel()->tryConnect(connection);
                    if (!isInvalid(connection_id))
                    {
                        //内部添加新添加的连接对象
                        scene_.flowSceneData()->addConnection(connection_id, connection);
                    }
                }
            }
        }
        else
        {
            if (original_item_)
            {
                if (scene_.flowModel()->tryDisconnect(original_item_->id()))
                {
                    scene_.flowSceneData()->removeConnection(original_item_->id());
                }
                else
                {
                    original_item_->setVisible(true);
                }
            }
        }
    }
    else
    {
        if (original_item_)
        {
            if (scene_.flowModel()->tryDisconnect(original_item_->id()))
            {
                //std::cout << "scene_.flowSceneData()->removeConnection" << std::endl;
                scene_.flowSceneData()->removeConnection(original_item_->id());
            }
            else
            {
                original_item_->setVisible(true);
            }
        }
    }

    //退出
    //如果没有找到节点，则重置最后一个悬停节点 ID。
    if (!isInvalid(last_hovered_node_))
    {
        auto item = scene_.flowSceneData()->absNodeItem(last_hovered_node_);
        if (item)
        {
            item->update();
        }
    }
    last_hovered_node_ = { 0 };

    ungrabMouse();                                  //不再捕获鼠标事件。
    event->accept();                                //标记事件已被正确处理。
    scene_.flowSceneData()->resetDraftConnection(); //重置对象
}

} //namespace fe
