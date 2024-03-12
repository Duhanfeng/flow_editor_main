//
// Created by cxc on 2024/3/12.
//

#include "dynamic_geometry.hpp"

namespace
{
//端口垂直方向的最大尺寸
inline double maxVerticalPortsExtent(const fe::NodeData& data, unsigned int port_size, unsigned int port_spasing)
{
    size_t n_in_ports = data.in_port.size();
    size_t n_out_ports = data.out_port.size();
    size_t max_num_of_entries = std::max(n_in_ports, n_out_ports);
    size_t step = port_size + port_spasing;
    return step * max_num_of_entries + port_spasing;
}

inline double textWidth(QFontMetrics* font_metrics, const QString& str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    return (double)font_metrics->horizontalAdvance(str);
#else
    return (double)font_metrics->width(str);
#endif
}
} //namespace

namespace fe
{
DynamicGeometry::DynamicGeometry(const fe::NodeData& data, std::shared_ptr<NodeStyle>& node_style) :
    data_(data), node_style_(node_style)
{
    QFont font = node_style_->font;
    QFont blod_font = node_style_->font;
    blod_font.setBold(true);
    font_metrics_ = std::make_unique<QFontMetrics>(font);
    bold_font_metrics_ = std::make_unique<QFontMetrics>(blod_font);

    //初始化对象
    simple_components_.caption_text = QStaticText(data.caption_text);
    components_.caption_text = QStaticText(data.caption_text);
    components_.in_ports.resize(data_.in_port.size());
    components_.out_ports.resize(data_.out_port.size());
    for (size_t i = 0; i < data_.in_port.size(); ++i)
    {
        components_.in_ports[i].port_text = QStaticText(data_.in_port[i].port_name);
        components_.in_ports[i].port_type = data_.in_port[i].port_type;
        auto itr = node_style_->type_color_map.find(data_.in_port[i].port_type);
        if (itr != node_style_->type_color_map.end())
        {
            components_.in_ports[i].port_color = itr->second;
        }
        else
        {
            components_.in_ports[i].port_color = { 125, 125, 125 };
        }
    }
    for (size_t i = 0; i < data_.out_port.size(); ++i)
    {
        components_.out_ports[i].port_text = QStaticText(data_.out_port[i].port_name);
        components_.out_ports[i].port_type = data_.out_port[i].port_type;
        auto itr = node_style_->type_color_map.find(data_.out_port[i].port_type);
        if (itr != node_style_->type_color_map.end())
        {
            components_.out_ports[i].port_color = itr->second;
        }
        else
        {
            components_.out_ports[i].port_color = { 125, 125, 125 };
        }
    }
    components_.run_btn_polygon.resize(3);

    //计算图形
    update();
}
void DynamicGeometry::update()
{
    //-----计算端口文本区域
    //计算连接点尺寸
    double point_diameter = node_style_->connection_point_diameter; //连接点尺寸
    double point_diameter_extend = 2.0 * point_diameter;            //扩展的连接点尺寸

    double in_width = 0;
    for (const auto& port : data_.in_port)
    {
        in_width = std::max(in_width, textWidth(font_metrics_.get(), port.port_name));
    }
    double out_width = 0;
    for (const auto& port : data_.out_port)
    {
        out_width = std::max(out_width, textWidth(font_metrics_.get(), port.port_name));
    }
    unsigned int port_spasing = 5;                                                      //端口字符之间的间隔
    unsigned int port_text_height = font_metrics_->height();                            //端口字符的高度
    double port_width = in_width + out_width + point_diameter + 2 * port_spasing;       //端口区域宽度
    double port_height = maxVerticalPortsExtent(data_, port_text_height, port_spasing); //端口字符区域高度

    //计算标题区域
    QSizeF caption_size = bold_font_metrics_->boundingRect(data_.caption_text).size();
    constexpr double btn_scale = 1.6;
    constexpr double icon_offset_rate = 0.2;
    double temp_btn_height = caption_size.height() * btn_scale;
    QSizeF icon_size = { node_style_->icon_size, node_style_->icon_size };
    QSizeF icon_size2 = icon_size * (1.0 - icon_offset_rate); //这是在node区域内部的
    double icon_offset = node_style_->icon_size * icon_offset_rate;
    double title_height = std::max(icon_size2.width(), temp_btn_height);
    QSizeF btn_size = { title_height, title_height };
    double title_width = icon_size2.width() + caption_size.width() + btn_size.width();

    //计算节点主区域的尺寸
    double node_width = std::max(title_width, port_width);
    double node_height = title_height + port_height;
    QSizeF node_size = { node_width, node_height };

    //计算边界保留区域
    constexpr double margin = 3.0; //3个像素的边界保留
    double left_margin = std::max(icon_offset, point_diameter_extend * 0.5) + margin;
    double right_margin = point_diameter_extend * 0.5 + margin;
    double top_margin = icon_offset + margin;
    double bottom_margin = margin;

    //计算边界区域
    components_.bounding_rect = { 0, 0, node_size.width() + left_margin + right_margin, node_size.height() + top_margin + bottom_margin };
    components_.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = components_.bounding_rect.topLeft() + QPointF{ left_margin, top_margin };
    double offset_x = translate_offset.x();
    double offset_y = translate_offset.y();

    //计算各区域位置
    components_.node_rect = { translate_offset, node_size };
    components_.icon_rect = { QPointF{ -icon_offset + offset_x, -icon_offset + offset_y }, icon_size };
    components_.run_btn_rect = { { node_width - btn_size.width() + offset_x, 0 + offset_y }, btn_size };
    double caption_offset = (icon_size2.height() - caption_size.height()) * 0.5;
    components_.caption_rect = { { icon_size2.width() + offset_x, caption_offset + offset_y }, caption_size };
    components_.title_rect = { icon_size2.width() + offset_x, offset_y, node_width - icon_size2.width() - btn_size.width(), title_height - 3.0 };
    components_.port_rect = { offset_x, title_height + offset_y, node_width, port_height };
    components_.caption_position = components_.caption_rect.topLeft();

    //计算运行按钮三角位置
    double btn_offset = btn_size.width() * 0.2;
    double btn_offset2 = btn_size.width() * 0.1;
    components_.run_btn_rect2 = { components_.run_btn_rect.topLeft() + QPointF{ btn_offset2, btn_offset2 }, QSizeF{ btn_size.width() - 2 * btn_offset2, btn_size.height() - 2 * btn_offset2 } };
    components_.run_btn_polygon[0] = { components_.run_btn_rect.topLeft() + QPointF{ btn_offset, btn_offset } };
    components_.run_btn_polygon[1] = QPointF{ components_.run_btn_rect.right() - btn_offset, components_.run_btn_rect.center().y() };
    components_.run_btn_polygon[2] = { components_.run_btn_rect.bottomLeft() + QPointF{ btn_offset, -btn_offset } };

    //开始绘画各port区域
    double half_point_size = point_diameter * 0.5;
    double half_point_diameter_extend = point_diameter_extend * 0.5;
    double point_offset = ((double)port_text_height - point_diameter) * 0.5;
    double point_offset2 = ((double)port_text_height - point_diameter_extend) * 0.5;
    double total_port_height = title_height + port_spasing * 0.5;
    for (size_t i = 0; i < components_.in_ports.size(); ++i)
    {
        components_.in_ports[i].port_rect = QRectF{ -half_point_size + offset_x, total_port_height + point_offset + offset_y, point_diameter, point_diameter };
        components_.in_ports[i].port_rect_extend = QRectF{ -half_point_diameter_extend + offset_x, total_port_height + point_offset2 + offset_y, point_diameter_extend, point_diameter_extend };
        components_.in_ports[i].port_text_rect = QRectF{ half_point_size + port_spasing + offset_x, total_port_height + offset_y, in_width, (float)port_text_height };
        components_.in_ports[i].port_center = components_.in_ports[i].port_rect.center();
        components_.in_ports[i].port_text_position = components_.in_ports[i].port_text_rect.topLeft();
        total_port_height += (double)port_text_height + port_spasing;
    }
    total_port_height = title_height + port_spasing * 0.5;
    double output_offset_x = node_width - half_point_size - out_width - port_spasing;
    double output_point_offset_x = node_width - half_point_size;
    double output_point_offset_x2 = node_width - half_point_diameter_extend;
    for (size_t i = 0; i < components_.out_ports.size(); ++i)
    {
        components_.out_ports[i].port_rect = QRectF{ output_point_offset_x + offset_x, total_port_height + point_offset + offset_y, point_diameter, point_diameter };
        components_.out_ports[i].port_rect_extend = QRectF{ output_point_offset_x2 + offset_x, total_port_height + point_offset2 + offset_y, point_diameter_extend, point_diameter_extend };
        components_.out_ports[i].port_text_rect = QRectF{ output_offset_x + offset_x, total_port_height + offset_y, out_width, (float)port_text_height };
        components_.out_ports[i].port_center = components_.out_ports[i].port_rect.center();
        components_.out_ports[i].port_text_position = components_.out_ports[i].port_text_rect.topLeft();
        total_port_height += (double)port_text_height + port_spasing;
    }
}
void DynamicGeometry::updateSimple(double scale)
{
    double port_point_size = node_style_->connection_point_diameter * 2; //连接点尺寸
    double half_point_size = port_point_size * 0.5;

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
    if (caption_size.width() >= (icon_size.width() + port_point_size))
    {
        //标题比图标区域大的情况
        simple_components_.icon_rect = { x_margin + x_offset, y_margin, icon_size.width(), icon_size.height() };
        simple_components_.caption_rect = { x_margin, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }
    else
    {
        //图标区域比标题大的情况
        x_margin += half_point_size;
        simple_components_.icon_rect = { x_margin, y_margin, icon_size.width(), icon_size.height() };
        simple_components_.caption_rect = { x_margin + x_offset, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }

    //计算port区域
    simple_components_.in_port_rect = { simple_components_.icon_rect.left() - half_point_size, simple_components_.icon_rect.center().y() - half_point_size, port_point_size, port_point_size };
    simple_components_.out_port_rect = { simple_components_.icon_rect.right() - half_point_size, simple_components_.icon_rect.center().y() - half_point_size, port_point_size, port_point_size };

    //计算对象的边界
    simple_components_.bounding_rect = { 0.0, 0.0, std::max(caption_size.width(), icon_size.width()) + 2 * x_margin, icon_size.height() + caption_size.height() + 2 * y_margin };

    //-----平移区域
    //获取bounding_rect的中心点坐标
    simple_components_.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = simple_components_.bounding_rect.topLeft();
    //调整每个矩形的坐标，使其相对于bounding_rect的中心
    simple_components_.icon_rect.translate(translate_offset);
    simple_components_.caption_rect.translate(translate_offset);
    simple_components_.in_port_rect.translate(translate_offset);
    simple_components_.out_port_rect.translate(translate_offset);
}
} //namespace fe
