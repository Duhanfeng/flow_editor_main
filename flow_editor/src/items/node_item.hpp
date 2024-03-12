//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <memory>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_geometry.hpp>
#include <src/items/abs_node_item.hpp>

namespace fe
{
class NodeItem final : public AbsNodeItem
{
public:
    NodeItem() = delete;
    NodeItem(FlowScene& scene, const guid16& id);

    int getPortIndex(PortType required_port, const QPoint& pos) const override;

    //绘画接口
    QRectF boundingRect() const override { return *bounding_rect_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    //事件重载
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    //更新缓存
    void updateCache(double scale);

private:
    friend class NodePainter;
    NodeData* data_ = nullptr;
    DynamicGeometry* geometry_ = nullptr;
    std::shared_ptr<NodeStyle> style_ = nullptr;
    QPixmap icon_;
    double z_value_ = 0.0;
    double scale_ = 0.0;
    const QRectF* bounding_rect_ = nullptr;
    int crt_model_ = 0; //0:正常显示  1:缩略图模式

    //当前状态
    bool is_hovered_ = false;
    bool is_hovered_btn_ = false;
    bool is_checked_btn_ = false;
};

} //namespace fe