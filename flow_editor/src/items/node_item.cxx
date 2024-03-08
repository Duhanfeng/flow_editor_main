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
#include <src/node_sub_geometry/hor_geometry_compute.hpp>
#include <iostream>

namespace fe
{
NodeItem::NodeItem(const NodeData& data, std::shared_ptr<NodeStyle> style) :
    data_(data), style_(style)
{
    updateCache();
}
void NodeItem::updateCache()
{
    setPos(data_.position);
    HorGeometryCompute::compute(data_, style_, node_geometries_);
}
QRectF NodeItem::boundingRect() const
{
    return node_geometries_.bounding_rect;
}
void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget)
{
    //double lod = item->levelOfDetailFromTransform(painter->worldTransform());
    ////double scale = 1.0 / lod;
    //if (scale_ != lod)
    //{
    //    scale_ = lod;
    //    //HorGeometryCompute::compute(data_, scale_, node_geometries_);
    //    initializePixmap(scale_);
    //}
    //
    ////if (scale_ == 1.0)
    ////{
    //painter->save();
    ////painter->drawPixmap(0, 0, pixmap_cache_);
    //painter->scale(1.0 / scale_, 1.0 / scale_);
    //painter->drawPixmap(0, 0, pixmap_cache_);
    //painter->restore();
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
void NodeItem::paintTo(QPainter* painter, const NodeData& data, const NodeSubGeometry& node_sub_geometry, std::shared_ptr<NodeStyle>& style)
{
    QPen pen = painter->pen();

    QFont font = style->font;
    painter->setFont(font);

    //QFontMetrics font_metrics(font);
    //QSizeF caption_size = font_metrics.boundingRect(data.node_name).size();
    //std::cout << caption_size.width() << "," << caption_size.height() << std::endl;
    //
    //std::cout << "Font 1 properties:" << std::endl;
    //std::cout << "Family: " << font.family().toStdString() << std::endl;
    //std::cout << "PointSize: " << font.pointSize() << std::endl;
    //
    //QSizeF caption_size2 = painter->fontMetrics().boundingRect(data.node_name).size();
    //std::cout << caption_size2.width() << "," << caption_size2.height() << std::endl;
    //
    //std::cout << "Painter's Font properties:" << std::endl;
    //std::cout << "Family: " << painter->font().family().toStdString() << std::endl;
    //std::cout << "PointSize: " << painter->font().pointSize() << std::endl;

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
    painter->drawRoundedRect(node_sub_geometry.node_rect, radius, radius);

    //绘画标题
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(style->font_color);
    painter->drawStaticText(node_sub_geometry.caption_rect.topLeft(), QStaticText(data.node_name));
    //painter->drawText(node_sub_geometry.caption_rect, data.node_name);

    //绘画输入输出名
    font.setBold(false);
    painter->setFont(font);
    for (size_t i = 0; i < node_sub_geometry.in_port_text_rect.size(); ++i)
    {
        //painter->drawText(node_sub_geometry.in_port_text_rect[i], data.in_port[i].port_name);
        painter->drawStaticText(node_sub_geometry.in_port_text_rect[i].topLeft(), QStaticText(data.in_port[i].port_name));
    }
    painter->setFont(font);
    for (size_t i = 0; i < node_sub_geometry.out_port_text_rect.size(); ++i)
    {
        //painter->drawText(node_sub_geometry.out_port_text_rect[i], data.out_port[i].port_name);
        painter->drawStaticText(node_sub_geometry.out_port_text_rect[i].topLeft(), QStaticText(data.out_port[i].port_name));
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