//
// Created by cxc on 2024/3/8.
//

#include "dynamic_h_port_geometry.hpp"

namespace fe
{
DynamicHPortGeometry::DynamicHPortGeometry(const fe::NodeData& data, std::shared_ptr<NodeStyle>& node_style) :
    data_(data), node_style_(node_style)
{
    QFont blod_font = node_style_->font;
    blod_font.setBold(true);
    bold_font_metrics_ = std::make_unique<QFontMetrics>(blod_font);

    geometry_.port_name = QStaticText(data.node_name);
}
void DynamicHPortGeometry::update(double scale)
{
    if (last_scale_ >= 1.0 && scale > 1.0)
    {
        return;
    }
    last_scale_ = scale;

    double port_point_size = node_style_->connection_point_diameter * 2; //连接点尺寸
    double half_point_size = port_point_size * 0.5;

    //计算标题区域的尺寸
    QSizeF caption_size = bold_font_metrics_->boundingRect(data_.node_name).size();
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
    if (caption_size.width() >= (icon_size.width() + port_point_size))
    {
        //标题比图标区域大的情况
        geometry_.icon_rect = { x_margin + x_offset, y_margin, icon_size.width(), icon_size.height() };
        geometry_.caption_rect = { x_margin, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }
    else
    {
        //图标区域比标题大的情况
        x_margin += half_point_size;
        geometry_.icon_rect = { x_margin, y_margin, icon_size.width(), icon_size.height() };
        geometry_.caption_rect = { x_margin + x_offset, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }

    //计算port区域
    if (data_.node_type == NodeType::InNode)
    {
        geometry_.port_rect = { geometry_.icon_rect.right() - half_point_size, geometry_.icon_rect.center().y() - half_point_size, port_point_size, port_point_size };
    }
    else
    {
        geometry_.port_rect = { geometry_.icon_rect.left() - half_point_size, geometry_.icon_rect.center().y() - half_point_size, port_point_size, port_point_size };
    }

    //计算对象的边界
    geometry_.bounding_rect = { 0.0, 0.0, std::max(caption_size.width(), icon_size.width()) + 2 * x_margin, icon_size.height() + caption_size.height() + 2 * y_margin };

    //-----平移区域
    //获取bounding_rect的中心点坐标
    geometry_.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = geometry_.bounding_rect.topLeft();
    //调整每个矩形的坐标，使其相对于bounding_rect的中心
    geometry_.icon_rect.translate(translate_offset);
    geometry_.caption_rect.translate(translate_offset);
    geometry_.port_rect.translate(translate_offset);

    if (data_.node_type == NodeType::InNode)
    {
        geometry_.node_polygon[0] = (geometry_.icon_rect.topLeft());
        geometry_.node_polygon[1] = QPointF{ geometry_.icon_rect.right(), geometry_.icon_rect.center().y() };
        geometry_.node_polygon[2] = (geometry_.icon_rect.bottomLeft());
    }
    else
    {
        geometry_.node_polygon[0] = (geometry_.icon_rect.topRight());
        geometry_.node_polygon[1] = QPointF{ geometry_.icon_rect.left(), geometry_.icon_rect.center().y() };
        geometry_.node_polygon[2] = (geometry_.icon_rect.bottomRight());
    }
}

} //namespace fe
