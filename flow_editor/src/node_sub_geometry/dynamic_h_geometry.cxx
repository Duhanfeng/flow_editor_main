//
// Created by cxc on 2024/3/8.
//

#include "dynamic_h_geometry.hpp"

namespace fe
{
//端口垂直方向的最大尺寸
inline double maxVerticalPortsExtent(const NodeData& data, unsigned int port_size, unsigned int port_spasing)
{
    size_t n_in_ports = data.in_port.size();
    size_t n_out_ports = data.out_port.size();
    size_t max_num_of_entries = std::max(n_in_ports, n_out_ports);
    size_t step = port_size + port_spasing;
    return step * max_num_of_entries;
}

inline double textWidth(QFontMetrics* font_metrics, const QString& str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    return (double)font_metrics->horizontalAdvance(str);
#else
    return (double)font_metrics->width(str);
#endif
}

DynamicHGeometry::DynamicHGeometry(const NodeData& data, std::shared_ptr<NodeStyle>& node_style) :
    data_(data), node_style_(node_style)
{
    QFont font = node_style_->font;
    QFont blod_font = node_style_->font;
    blod_font.setBold(true);
    font_metrics_ = std::make_unique<QFontMetrics>(font);
    bold_font_metrics_ = std::make_unique<QFontMetrics>(blod_font);

    //初始化对象
    geometry_.in_port_rect.resize(data_.in_port.size());
    geometry_.in_port_text_rect.resize(data_.in_port.size());
    geometry_.out_port_rect.resize(data_.out_port.size());
    geometry_.out_port_text_rect.resize(data_.out_port.size());
    //初始化字体缓存
    geometry_.node_name = QStaticText(data.node_name);
    geometry_.in_port_text.resize(data_.in_port.size());
    geometry_.out_port_text.resize(data_.out_port.size());
    for (size_t i = 0; i < data_.in_port.size(); ++i)
    {
        geometry_.in_port_text[i] = QStaticText(data_.in_port[i].port_name);
    }
    for (size_t i = 0; i < data_.out_port.size(); ++i)
    {
        geometry_.out_port_text[i] = QStaticText(data_.out_port[i].port_name);
    }

    geometry_simple_.node_name = QStaticText(data.node_name);
}
void DynamicHGeometry::update(double scale)
{
    //计算端口区域
    unsigned int port_spasing = 5;                    //端口字符之间的间隔
    unsigned int port_size = font_metrics_->height(); //端口字符的高度

    //计算端口区域尺寸
    double port_height = maxVerticalPortsExtent(data_, port_size, port_spasing);
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
    double port_width = in_width + out_width;                        //端口区域尺寸
    double port_point_size = node_style_->connection_point_diameter; //连接点尺寸
    double half_point_size = port_point_size * 0.5;

    //计算标题区域的尺寸
    QSizeF caption_size = bold_font_metrics_->boundingRect(data_.node_name).size();
    if (scale < 1.0)
    {
        //如果是缩小,则需要进行动态调整,以保证title尺寸保持不变
        caption_size /= scale;
    }
    constexpr double icon_scale = 1.6;
    double icon_height = caption_size.height() * icon_scale;
    QSizeF icon_size = { icon_height, icon_height };
    QSizeF btn_size = icon_size;

    //计算全局尺寸
    double title_height = icon_size.height();
    double node_width = std::max(port_width, icon_size.width() + caption_size.width() + btn_size.width()) + port_point_size;
    double node_height = title_height + port_height;
    QSizeF node_size = { node_width, node_height };

    //计算主区域
    constexpr double margin = 3.0; //3个像素的边界保留
    double x_margin = port_point_size * 0.5 + margin;
    double y_margin = margin;
    geometry_.bounding_rect = { 0, 0, node_size.width() + port_point_size + margin * 2, node_size.height() + margin * 2 };
    geometry_.node_rect = { { x_margin, y_margin }, node_size };
    geometry_.title_rect = { x_margin, y_margin, node_width, title_height };
    geometry_.port_rect = { x_margin, y_margin + title_height, node_width, port_height };

    //开始反算各个组件的Rect(以原点在左上角为原点)
    double caption_offset = (icon_size.height() - caption_size.height()) * 0.5;
    geometry_.icon_rect = QRectF{ { x_margin, y_margin }, icon_size };
    geometry_.caption_rect = QRectF{ { x_margin + icon_size.width(), y_margin + caption_offset }, caption_size };
    geometry_.run_btn_rect = QRectF{ { x_margin + node_width - btn_size.width(), y_margin }, btn_size };
    //计算运行按钮的区域
    geometry_.run_polygon[0] = (geometry_.run_btn_rect.topLeft());
    geometry_.run_polygon[1] = QPointF{ geometry_.run_btn_rect.right(), geometry_.run_btn_rect.center().y() };
    geometry_.run_polygon[2] = (geometry_.run_btn_rect.bottomLeft());

    //计算端口交互点区域
    double point_offset = ((double)port_size - port_point_size) * 0.5;
    double total_port_height = title_height + port_spasing * 0.5;
    for (size_t i = 0; i < data_.in_port.size(); ++i)
    {
        geometry_.in_port_rect[i] = QRectF{ x_margin + -half_point_size, y_margin + total_port_height + point_offset, port_point_size, port_point_size };
        geometry_.in_port_text_rect[i] = QRectF{ x_margin + half_point_size, y_margin + total_port_height, in_width, (float)port_size };
        total_port_height += (float)port_size + port_spasing;
    }

    total_port_height = title_height + port_spasing * 0.5;
    double output_offset = node_width - half_point_size - out_width;
    double output_point_offset = node_width - half_point_size;
    for (size_t i = 0; i < data_.out_port.size(); ++i)
    {
        geometry_.out_port_rect[i] = QRectF{ x_margin + output_point_offset, y_margin + total_port_height + point_offset, port_point_size, port_point_size };
        geometry_.out_port_text_rect[i] = QRectF{ x_margin + output_offset, y_margin + total_port_height, out_width, (float)port_size };
        total_port_height += (float)port_size + port_spasing;
    }

    //-----平移区域
    //获取bounding_rect的中心点坐标
    geometry_.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = geometry_.bounding_rect.topLeft();
    //调整每个矩形的坐标，使其相对于bounding_rect的中心
    geometry_.node_rect.translate(translate_offset);
    geometry_.title_rect.translate(translate_offset);
    geometry_.icon_rect.translate(translate_offset);
    geometry_.caption_rect.translate(translate_offset);
    geometry_.run_btn_rect.translate(translate_offset);
    geometry_.port_rect.translate(translate_offset);
    geometry_.message_box_rect.translate(translate_offset);
    for (auto& rect : geometry_.in_port_rect)
    {
        rect.translate(translate_offset);
    }
    for (auto& rect : geometry_.in_port_text_rect)
    {
        rect.translate(translate_offset);
    }
    for (auto& rect : geometry_.out_port_rect)
    {
        rect.translate(translate_offset);
    }
    for (auto& rect : geometry_.out_port_text_rect)
    {
        rect.translate(translate_offset);
    }
    for (auto& rect : geometry_.run_polygon)
    {
        rect += translate_offset;
    }
}

void DynamicHGeometry::updateSimple(double scale)
{
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
        geometry_simple_.icon_rect = { x_margin + x_offset, y_margin, icon_size.width(), icon_size.height() };
        geometry_simple_.caption_rect = { x_margin, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }
    else
    {
        //图标区域比标题大的情况
        x_margin += half_point_size;
        geometry_simple_.icon_rect = { x_margin, y_margin, icon_size.width(), icon_size.height() };
        geometry_simple_.caption_rect = { x_margin + x_offset, y_margin + icon_size.height(), caption_size.width(), caption_size.height() };
    }

    //计算port区域
    geometry_simple_.in_port_rect = { geometry_simple_.icon_rect.left() - half_point_size, geometry_simple_.icon_rect.center().y() - half_point_size, port_point_size, port_point_size };
    geometry_simple_.out_port_rect = { geometry_simple_.icon_rect.right() - half_point_size, geometry_simple_.icon_rect.center().y() - half_point_size, port_point_size, port_point_size };

    //计算对象的边界
    geometry_simple_.bounding_rect = { 0.0, 0.0, std::max(caption_size.width(), icon_size.width()) + 2 * x_margin, icon_size.height() + caption_size.height() + 2 * y_margin };

    //-----平移区域
    //获取bounding_rect的中心点坐标
    geometry_simple_.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = geometry_simple_.bounding_rect.topLeft();
    //调整每个矩形的坐标，使其相对于bounding_rect的中心
    geometry_simple_.icon_rect.translate(translate_offset);
    geometry_simple_.caption_rect.translate(translate_offset);
    geometry_simple_.in_port_rect.translate(translate_offset);
    geometry_simple_.out_port_rect.translate(translate_offset);
}
} //namespace fe