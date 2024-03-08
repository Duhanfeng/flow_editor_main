//
// Created by cxc on 2024/3/7.
//

#include "in_node_item.hpp"
#include "src/node_sub_geometry/hor_geometry_compute.hpp"
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>

namespace fe
{
InNodeItem::InNodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style) :
    data_(data), style_(style)
{
    updateCache();
}
void InNodeItem::updateCache()
{
    setPos(data_.position);

    HorGeometryCompute::compute(data_, style_, node_geometries_);
    //rect_cache_ = node_sub_geometry_.bounding_rect;
}
QRectF InNodeItem::boundingRect() const
{
    return node_geometries_.bounding_rect;
}
void InNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    //double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    //double scale = 1.0 / lod;
    //if (scale_ != scale)
    //{
    //    scale_ = scale;
    //    initializePixmap(scale_);
    //}
    //
    //if (scale_ == 1.0)
    //{
    //    painter->drawPixmap(node_sub_geometry_.bounding_rect.topLeft(), pixmap_cache_);
    //}
    //else
    //{
    //    painter->save();
    //    //启用抗锯齿和平滑像素图转换
    //    painter->setRenderHint(QPainter::Antialiasing, true);
    //    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    //
    //    painter->scale(scale_, scale_);
    //    painter->drawPixmap(geometry_scale_.bounding_rect.topLeft(), pixmap_cache_);
    //    painter->restore();
    //}

    paintTo(painter, data_, node_geometries_, style_);
}
//void InNodeItem::initializePixmap(double scale)
//{
//    if (scale == 1.0)
//    {
//        QPixmap pixmap(node_sub_geometry_.bounding_rect.width() + 1, node_sub_geometry_.bounding_rect.height() + 1);
//        pixmap.fill(Qt::transparent);
//
//        QPainter pixmap_painter(&pixmap);
//        paintTo(&pixmap_painter, data_, node_sub_geometry_);
//        pixmap_painter.end();
//
//        //保存QPixmap
//        pixmap_cache_ = pixmap;
//    }
//    else
//    {
//        scale = 1 / scale;
//        geometry_scale_.bounding_rect = { node_sub_geometry_.bounding_rect.topLeft() * scale, node_sub_geometry_.bounding_rect.size() * scale };
//        geometry_scale_.node_rect = { node_sub_geometry_.node_rect.topLeft() * scale, node_sub_geometry_.node_rect.size() * scale };
//        geometry_scale_.icon_rect = { node_sub_geometry_.icon_rect.topLeft() * scale, node_sub_geometry_.icon_rect.size() * scale };
//        geometry_scale_.caption_rect = { node_sub_geometry_.caption_rect.topLeft() * scale, node_sub_geometry_.caption_rect.size() * scale };
//        geometry_scale_.run_btn_rect = { node_sub_geometry_.run_btn_rect.topLeft() * scale, node_sub_geometry_.run_btn_rect.size() * scale };
//        geometry_scale_.message_box_rect = { node_sub_geometry_.message_box_rect.topLeft() * scale, node_sub_geometry_.message_box_rect.size() * scale };
//
//        geometry_scale_.in_port_rect.resize(node_sub_geometry_.in_port_rect.size());
//        geometry_scale_.in_port_text_rect.resize(node_sub_geometry_.in_port_rect.size());
//        for (size_t i = 0; i < node_sub_geometry_.in_port_rect.size(); ++i)
//        {
//            geometry_scale_.in_port_rect[i] = { node_sub_geometry_.in_port_rect[i].topLeft() * scale, node_sub_geometry_.in_port_rect[i].size() * scale };
//            geometry_scale_.in_port_text_rect[i] = { node_sub_geometry_.in_port_text_rect[i].topLeft() * scale, node_sub_geometry_.in_port_text_rect[i].size() * scale };
//        }
//
//        geometry_scale_.out_port_rect.resize(node_sub_geometry_.out_port_rect.size());
//        geometry_scale_.out_port_text_rect.resize(node_sub_geometry_.out_port_rect.size());
//        for (size_t i = 0; i < node_sub_geometry_.out_port_rect.size(); ++i)
//        {
//            geometry_scale_.out_port_rect[i] = { node_sub_geometry_.out_port_rect[i].topLeft() * scale, node_sub_geometry_.out_port_rect[i].size() * scale };
//            geometry_scale_.out_port_text_rect[i] = { node_sub_geometry_.out_port_text_rect[i].topLeft() * scale, node_sub_geometry_.out_port_text_rect[i].size() * scale };
//        }
//
//        QPixmap pixmap(geometry_scale_.bounding_rect.width() + 1, geometry_scale_.bounding_rect.height() + 1);
//        pixmap.fill(Qt::transparent);
//
//        QPainter pixmap_painter(&pixmap);
//        paintTo(&pixmap_painter, data_, geometry_scale_);
//        pixmap_painter.end();
//
//        //保存QPixmap
//        pixmap_cache_ = pixmap;
//    }
//}
void InNodeItem::paintTo(QPainter* painter, const NodeData& data, const NodeSubGeometry& node_sub_geometry, std::shared_ptr<NodeStyle>& style)
{
    QFont f = painter->font();
    QFont def_font(u8"新宋体");
    QPen pen = painter->pen();
    painter->setFont(def_font);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::VerticalSubpixelPositioning);

    //画各个区域(调试用)
    painter->setPen(pen);
    painter->drawRect(node_sub_geometry.bounding_rect);
    painter->drawRect(node_sub_geometry.node_rect);
    painter->drawRect(node_sub_geometry.icon_rect);
    painter->drawRect(node_sub_geometry.caption_rect);
    painter->drawRect(node_sub_geometry.run_btn_rect);
    for (size_t i = 0; i < node_sub_geometry.in_port_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.out_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.in_port_text_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.in_port_text_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_text_rect.size(); ++i)
    {
        painter->drawRect(node_sub_geometry.out_port_text_rect[i]);
    }

    //画主体
    const double radius = 3.0;
    //painter->fillRect(node_sub_geometry_.node_rect, 0xC0C0C0);
    painter->drawRoundedRect(node_sub_geometry.node_rect, radius, radius);

    //绘画标题
    //QFont font_song(u8"宋体");

    //f.setBold(true);
    //painter->setFont(font_song);
    painter->setPen(0xFFFFFF);
    painter->drawStaticText(node_sub_geometry.caption_rect.topLeft(), QStaticText(data.node_name));

    //绘画输入输出名
    for (size_t i = 0; i < node_sub_geometry.in_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(node_sub_geometry.in_port_text_rect[i].topLeft(), QStaticText(data.in_port[i].port_name));
    }
    f.setBold(false);
    painter->setFont(f);
    for (size_t i = 0; i < node_sub_geometry.out_port_text_rect.size(); ++i)
    {
        painter->drawStaticText(node_sub_geometry.out_port_text_rect[i].topLeft().toPoint(), QStaticText(data.out_port[i].port_name));
    }

    //绘画输入输出端口操作点
    painter->setPen(pen);
    for (size_t i = 0; i < node_sub_geometry.in_port_rect.size(); ++i)
    {
        painter->drawEllipse(node_sub_geometry.in_port_rect[i]);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_rect.size(); ++i)
    {
        painter->drawEllipse(node_sub_geometry.out_port_rect[i]);
    }

    //绘画运行按钮
    std::vector<QPointF> run_polygon;
    run_polygon.emplace_back(node_sub_geometry.run_btn_rect.topLeft());
    run_polygon.emplace_back(QPointF{ node_sub_geometry.run_btn_rect.right(), node_sub_geometry.run_btn_rect.center().y() });
    run_polygon.emplace_back(node_sub_geometry.run_btn_rect.bottomLeft());
    painter->drawPolygon(run_polygon.data(), (int)run_polygon.size());
}
} //namespace fe