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

namespace fe
{
class NodeItem : public QGraphicsItem
{
public:
    NodeItem() = delete;
    NodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style);
    QRectF boundingRect() const override { return *bounding_rect_; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget) override;

private:
    //绘画相关
    void updateCache();
    static void paintTo(QPainter* painter, const NodeSubGeometry& geometry, double scale, std::shared_ptr<NodeStyle>& style);
    static void paintSimpleTo(QPainter* painter, const NodeSubGeometrySimple& geometry_simple, double scale, std::shared_ptr<NodeStyle>& style);

private:
    NodeData data_;
    std::shared_ptr<NodeStyle> style_;
    DynamicHGeometry dynamic_hor_geometry_;
    double scale_ = 1.0;
    const QRectF* bounding_rect_ = nullptr;
    int crt_model_ = 0; //0:正常显示  1:缩略图模式
};

} //namespace fe