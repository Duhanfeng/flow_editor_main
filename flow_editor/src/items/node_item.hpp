//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <memory>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <src/node_sub_geometry/dynamic_h_geometry.hpp>
#include <src/items/abs_node_item.hpp>

namespace fe
{
class NodeItem : public AbsNodeItem
{
public:
    NodeItem() = delete;
    NodeItem(FlowScene& scene, const guid16& id);

    int getPortIndex(PortType required_port, const QPoint& pos) const override;

    //绘画接口
    QRectF boundingRect() const override { return *bounding_rect_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    //绘画相关
    void updateCache();
    static void paintTo(QPainter* painter, const NodeUIComponents& components, double scale, std::shared_ptr<NodeStyle>& style);
    static void paintSimpleTo(QPainter* painter, const NodeUISimpleComponents& simple_components, double scale, std::shared_ptr<NodeStyle>& style);

private:
    NodeData* data_ = nullptr;
    DynamicHGeometry* geometry_ = nullptr;
    std::shared_ptr<NodeStyle> style_ = nullptr;
    double z_value_ = 0.0;
    double scale_ = 1.0;
    const QRectF* bounding_rect_ = nullptr;
    int crt_model_ = 0; //0:正常显示  1:缩略图模式
};

} //namespace fe