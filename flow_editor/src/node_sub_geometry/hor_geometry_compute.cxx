//
// Created by cxc on 2024/3/7.
//

#include "hor_geometry_compute.hpp"
#include <QFont>
#include <QFontMetrics>

namespace fe
{
namespace hor_global
{
    bool is_init = false;
    unsigned int port_size = 20;
    unsigned int port_spasing = 5;
    unsigned int spasing = 5;
    QFontMetrics* font_metrics = nullptr;
    QFontMetrics* bold_font_metrics = nullptr;

    void init()
    {
        //全局数据没必要释放
        QFont f;
        font_metrics = new QFontMetrics(f);
        f.setBold(true);
        bold_font_metrics = new QFontMetrics(f);
        port_size = font_metrics->height();
        is_init = true;
    }

    inline double textWidth(const QString& str)
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        return (double)font_metrics->horizontalAdvance(str);
#else
        return (double)font_metrics->width(str);
#endif
    }

    inline double boldTextWidth(const QString& str)
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        return (double)bold_font_metrics->horizontalAdvance(str);
#else
        return (double)bold_font_metrics->width(str);
#endif
    }

} //namespace hor_global

//端口垂直方向的最大尺寸
inline double maxVerticalPortsExtent(const NodeData& data)
{
    size_t n_in_ports = data.in_port.size();
    size_t n_out_ports = data.out_port.size();
    size_t max_num_of_entries = std::max(n_in_ports, n_out_ports);
    size_t step = hor_global::port_size + hor_global::port_spasing;
    return step * max_num_of_entries;
}
//输入输出端口的文本最大尺寸
inline void maxPortsTextExtent(const NodeData& data, double& in_width, double& out_width)
{
    in_width = 0;
    for (const auto& port : data.in_port)
    {
        in_width = std::max(in_width, hor_global::textWidth(port.port_name));
    }

    out_width = 0;
    for (const auto& port : data.out_port)
    {
        out_width = std::max(out_width, hor_global::textWidth(port.port_name));
    }
}
//标题栏尺寸
inline QSizeF captionSize(const NodeData& data)
{
    return hor_global::bold_font_metrics->boundingRect(data.node_name).size();
}
//图标区域尺寸
inline QSizeF iconSize(double caption_height)
{
    return { caption_height * 1.6, caption_height * 1.6 };
}
//运行按钮尺寸
inline QSizeF runBtnSize(double caption_height)
{
    return { caption_height * 1.6, caption_height * 1.6 };
}

void HorGeometryCompute::compute(const NodeData& data, NodeSubGeometry& node_sub_geometry)
{
    if (!hor_global::is_init)
    {
        hor_global::init();
    }

    node_sub_geometry.in_port_rect.clear();
    node_sub_geometry.out_port_rect.clear();
    node_sub_geometry.in_port_text_rect.clear();
    node_sub_geometry.out_port_text_rect.clear();

    //计算图标标题栏区域
    QSizeF caption_size = captionSize(data);
    QSizeF icon_size = iconSize(caption_size.height());
    QSizeF btn_size = runBtnSize(caption_size.height());

    //计算端口区域尺寸
    double port_height = maxVerticalPortsExtent(data);
    double in_width;
    double out_width;
    maxPortsTextExtent(data, in_width, out_width);
    double port_width = in_width + out_width;

    //计算消息提示区域(暂略)

    QSizeF port_point_size = { 10, 10 };

    //计算全局尺寸
    double node_width = std::max(port_width, icon_size.width() + caption_size.width() + btn_size.width()) + port_point_size.width();
    double title_height = std::max(caption_size.height(), icon_size.height());
    double node_height = title_height + port_height;

    //开始反算各个组件的Rect(以原点在左上角为原点)
    double caption_offset = (icon_size.height() - caption_size.height()) * 0.5;
    node_sub_geometry.icon_rect = QRectF{ { 0, 0 }, icon_size };
    node_sub_geometry.caption_rect = QRectF{ { icon_size.width(), caption_offset }, caption_size };
    node_sub_geometry.run_btn_rect = QRectF{ { node_width - btn_size.width(), 0 }, btn_size };

    //计算端口交互点区域
    double half_point = port_point_size.width() * 0.5;
    double point_offset = ((double)hor_global::port_size - port_point_size.width()) * 0.5;
    double total_port_height = title_height + hor_global::port_spasing * 0.5;
    for (size_t i = 0; i < data.in_port.size(); ++i)
    {
        node_sub_geometry.in_port_rect.emplace_back(QRectF{ { -half_point, total_port_height + point_offset }, port_point_size });
        node_sub_geometry.in_port_text_rect.emplace_back(QRectF{ half_point, total_port_height, in_width, (float)hor_global::port_size });
        total_port_height += (float)hor_global::port_size + hor_global::port_spasing;
    }

    total_port_height = title_height + hor_global::port_spasing * 0.5;
    double output_offset = node_width - half_point - out_width;
    double output_point_offset = node_width - half_point;
    for (size_t i = 0; i < data.out_port.size(); ++i)
    {
        node_sub_geometry.out_port_rect.emplace_back(QRectF{ { output_point_offset, total_port_height + point_offset }, port_point_size });
        node_sub_geometry.out_port_text_rect.emplace_back(QRectF{ output_offset, total_port_height, out_width, (float)hor_global::port_size });
        total_port_height += (float)hor_global::port_size + hor_global::port_spasing;
    }

    //计算边界区域
    double margin = 3.0; //3个像素的边界保留
    double x_margin = port_point_size.width() * 0.5 + margin;
    double y_margin = margin;

    node_sub_geometry.bounding_rect = { 0, 0, node_width + port_point_size.width() + margin * 2, node_height + margin * 2 };
    //node_sub_geometry.bounding_rect.moveCenter({ 0, 0 });

    node_sub_geometry.node_rect = { x_margin, y_margin, node_width, node_height };
    //node_sub_geometry.node_rect.moveCenter({ 0, 0 });

    //原本区域以node边框左上角为(0,0)计算的,进行偏移
    double offset_x = x_margin;
    double offset_y = y_margin;
    node_sub_geometry.icon_rect.translate(offset_x, offset_y);
    node_sub_geometry.caption_rect.translate(offset_x, offset_y);
    node_sub_geometry.run_btn_rect.translate(offset_x, offset_y);
    node_sub_geometry.message_box_rect.translate(offset_x, offset_y);
    for (size_t i = 0; i < node_sub_geometry.in_port_rect.size(); ++i)
    {
        node_sub_geometry.in_port_rect[i].translate(offset_x, offset_y);
        node_sub_geometry.in_port_text_rect[i].translate(offset_x, offset_y);
    }
    for (size_t i = 0; i < node_sub_geometry.out_port_rect.size(); ++i)
    {
        node_sub_geometry.out_port_rect[i].translate(offset_x, offset_y);
        node_sub_geometry.out_port_text_rect[i].translate(offset_x, offset_y);
    }
}
} //namespace fe