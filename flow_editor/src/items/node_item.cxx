//
// Created by cxc on 2024/3/7.
//

#include "node_item.hpp"
#include <QPainter>
#include <QPen>

namespace ss
{
NodeItem::NodeItem(const NodeData& data, const NodeSubGeometry& node_sub_geometry) :
    data_(data),
    node_sub_geometry_(node_sub_geometry)
{
    updateCache();
}
void NodeItem::updateCache()
{
    //计算当前节点所占用的尺寸

    setPos(data_.position);
    rect_cache_.setRect(0, 0, node_sub_geometry_.size.width(), node_sub_geometry_.size.height());
}
QRectF NodeItem::boundingRect() const
{
    return rect_cache_;
}
void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    QPen pen = painter->pen();
    pen.setColor(QColor(50, 200, 10));
    painter->setBrush(QColor(0xC0C0C0));
    painter->drawRect(0, 0, 100, 100);
}
} //namespace ss