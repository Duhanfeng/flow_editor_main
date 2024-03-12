﻿//
// Created by cxc on 2024/3/8.
//

#include "dynamic_h_port_geometry.hpp"

namespace fe
{
DynamicHPortGeometry::DynamicHPortGeometry(NodeType node_type, const fe::NodeData& data, std::shared_ptr<NodeStyle>& node_style) :
    node_type_(node_type), data_(data), node_style_(node_style)
{
    QFont blod_font = node_style_->font;
    blod_font.setBold(true);
    bold_font_metrics_ = std::make_unique<QFontMetrics>(blod_font);

    components_.port_name = QStaticText(data.caption_text);
}
void DynamicHPortGeometry::update(double scale)
{
    if (last_scale_ >= 1.0 && scale > 1.0)
    {
        return;
    }
    last_scale_ = scale;

    double point_diameter = node_style_->connection_point_diameter; //连接点尺寸
    double point_diameter_extend = 2.0 * point_diameter;            //扩展的连接点尺寸
    double half_point_size = point_diameter * 0.5;
    double half_point_diameter_extend = point_diameter_extend * 0.5;

    //计算标题区域的尺寸
    QSizeF caption_size = bold_font_metrics_->boundingRect(data_.caption_text).size();
    if (scale < 1.0)
    {
        //如果是缩小,则需要进行动态调整,以保证尺寸保持
        caption_size /= scale;
    }

    //计算icon区域
    QSizeF icon_size = { 100, 100 };

    //计算标题区域
    constexpr double margin = 3.0; //3个像素的边界保留
    double x_margin = margin;
    double y_margin = margin;
    double x_offset = std::abs(caption_size.width() - icon_size.width()) * 0.5;
    if (caption_size.width() >= (icon_size.width() + point_diameter))
    {
        //标题比图标区域大的情况
        components_.icon_rect = { x_margin + x_offset, y_margin, icon_size.width(), icon_size.height() };
        components_.caption_rect = { x_margin, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }
    else
    {
        //图标区域比标题大的情况
        x_margin += half_point_size;
        components_.icon_rect = { x_margin, y_margin, icon_size.width(), icon_size.height() };
        components_.caption_rect = { x_margin + x_offset, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }

    //计算port区域
    if (node_type_ == NodeType::InNode)
    {
        components_.port_rect_extend = { components_.icon_rect.right() - half_point_diameter_extend, components_.icon_rect.center().y() - half_point_size, point_diameter_extend, point_diameter_extend };
    }
    else
    {
        components_.port_rect_extend = { components_.icon_rect.left() - half_point_diameter_extend, components_.icon_rect.center().y() - half_point_size, point_diameter_extend, point_diameter_extend };
    }

    //计算对象的边界
    components_.bounding_rect = { 0.0, 0.0, std::max(caption_size.width(), icon_size.width()) + 2 * x_margin, icon_size.height() + caption_size.height() + 2 * y_margin };

    //-----平移区域
    //获取bounding_rect的中心点坐标
    components_.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = components_.bounding_rect.topLeft();
    //调整每个矩形的坐标，使其相对于bounding_rect的中心
    components_.icon_rect.translate(translate_offset);
    components_.caption_rect.translate(translate_offset);
    components_.port_rect_extend.translate(translate_offset);
    components_.port_rect = { 0, 0, point_diameter, point_diameter };
    components_.port_rect.moveCenter(components_.port_rect_extend.center());

    components_.node_polygon.resize(3);
    if (node_type_ == NodeType::InNode)
    {
        components_.node_polygon[0] = (components_.icon_rect.topLeft());
        components_.node_polygon[1] = QPointF{ components_.icon_rect.right(), components_.icon_rect.center().y() };
        components_.node_polygon[2] = (components_.icon_rect.bottomLeft());
    }
    else
    {
        components_.node_polygon[0] = (components_.icon_rect.topRight());
        components_.node_polygon[1] = QPointF{ components_.icon_rect.left(), components_.icon_rect.center().y() };
        components_.node_polygon[2] = (components_.icon_rect.bottomRight());
    }
}

} //namespace fe
