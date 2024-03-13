//
// Created by cxc on 2024/3/8.
//

#include "dynamic_h_port_geometry.hpp"

namespace fe
{
DynamicHPortGeometry::DynamicHPortGeometry(NodeType node_type, const fe::NodeData& data, std::shared_ptr<NodeStyle> node_style, std::shared_ptr<TypeColorMap> type_color_map) :
    node_type_(node_type), data_(data), node_style_(node_style), type_color_map_(type_color_map)
{
    QFont blod_font = node_style_->font;
    blod_font.setBold(true);
    bold_font_metrics_ = std::make_unique<QFontMetrics>(blod_font);

    components_.node_polygon.resize(3);
    components_.caption_text = QStaticText(data.caption_text);

    if (node_type_ == NodeType::InNode)
    {
        if (!data_.out_port.empty())
        {
            components_.port_type = data_.out_port[0].port_type;
        }
        else
        {
            components_.port_type = "";
        }
    }
    else
    {
        if (!data_.in_port.empty())
        {
            components_.port_type = data_.in_port[0].port_type;
        }
        else
        {
            components_.port_type = "";
        }
    }
    auto itr = type_color_map->find(components_.port_type);
    if (itr != type_color_map->end())
    {
        components_.port_color = itr->second;
    }
    else
    {
        components_.port_color = { 125, 125, 125 };
    }
}
void DynamicHPortGeometry::update(double scale)
{
    if (last_scale_ >= 1.0 && scale > 1.0)
    {
        return;
    }
    last_scale_ = scale;

    double point_diameter = node_style_->connection_point_diameter * 2; //连接点尺寸(比普通的稍大一些)
    double point_diameter_extend = 2.0 * point_diameter;                //扩展的连接点尺寸
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

    //计算边界保留区域
    constexpr double margin = 3.0; //3个像素的边界保留
    double left_margin = margin;
    double right_margin = margin;
    double top_margin = margin;
    double bottom_margin = margin;

    //计算对象的边界
    double node_width = std::max(caption_size.width(), icon_size.width() + point_diameter_extend);
    double node_height = icon_size.height() + caption_size.height();
    QSizeF node_size = { node_width, node_height };
    components_.bounding_rect = { 0, 0, node_size.width() + left_margin + right_margin, node_size.height() + top_margin + bottom_margin };
    components_.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = components_.bounding_rect.topLeft() + QPointF{ left_margin, top_margin };
    double offset_x = translate_offset.x();
    double offset_y = translate_offset.y();

    //计算标题区域
    double icon_extend_width = icon_size.width() + point_diameter_extend;
    if (caption_size.width() >= icon_extend_width)
    {
        //标题比icon扩展区域大
        double icon_title_offset_x = (caption_size.width() - icon_size.width()) * 0.5;
        components_.caption_rect = { offset_x, offset_y + icon_size.height(), caption_size.width(), caption_size.height() };
        components_.icon_rect = { offset_x + icon_title_offset_x, offset_y, icon_size.width(), icon_size.height() };
    }
    else
    {
        //标题比icon区域小
        double icon_title_offset_x = (icon_size.width() - caption_size.width()) * 0.5;
        components_.icon_rect = { offset_x + half_point_diameter_extend, offset_y, icon_size.width(), icon_size.height() };
        components_.caption_rect = { offset_x + half_point_diameter_extend + icon_title_offset_x, offset_y + icon_size.height(), caption_size.width(), caption_size.height() };
    }

    //计算port区域
    if (node_type_ == NodeType::InNode)
    {
        components_.port_rect = { components_.icon_rect.right() - half_point_size, components_.icon_rect.center().y() - half_point_size, point_diameter, point_diameter };
        components_.port_rect_extend = { components_.icon_rect.right() - half_point_diameter_extend, components_.icon_rect.center().y() - half_point_diameter_extend, point_diameter_extend, point_diameter_extend };
        components_.port_center = components_.port_rect.center();
    }
    else
    {
        components_.port_rect = { components_.icon_rect.left() - half_point_size, components_.icon_rect.center().y() - half_point_size, point_diameter, point_diameter };
        components_.port_rect_extend = { components_.icon_rect.left() - half_point_diameter_extend, components_.icon_rect.center().y() - half_point_diameter_extend, point_diameter_extend, point_diameter_extend };
    }

    //计算其节点形状
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
