//
// Created by cxc on 2024/3/12.
//

#include "dynamic_geometry.hpp"

namespace
{
//端口垂直方向的最大尺寸
inline double maxVerticalPortsExtent(const fe::NodeData& data, unsigned int port_size, unsigned int port_spasing)
{
    size_t n_in_ports = data.inputs.size();
    size_t n_out_ports = data.outputs.size();
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

inline std::vector<QString> getMultiLineMessage(QFontMetrics* font_metrics, const QString& text, int max_width, int max_lines)
{
    //计算单行文本的最大宽度
    int single_line_width = font_metrics->horizontalAdvance(text);
    //如果单行文本宽度小于最大宽度，则不需要换行
    if (single_line_width <= max_width)
    {
        //直接返回原始文本
        return { text };
    }
    //如果单行文本宽度超过最大宽度，则尝试换行
    QStringList lines;
    int current_line_width = 0;
    QString current_line;
    QString remaining_text = text;
    for (int i = 0; i < text.length(); ++i)
    {
        QChar current_char = text[i];
        int char_width = font_metrics->horizontalAdvance(current_char);
        //检查当前字符是否会超出最大宽度
        if (current_line_width + char_width > max_width)
        {
            //如果当前行不为空，则保存当前行并重置
            if (!current_line.isEmpty())
            {
                lines.append(current_line);
                current_line.clear();
                current_line_width = 0;
                //如果已经达到最大行数，则截断剩余文本并退出循环
                if (lines.count() >= max_lines)
                {
                    remaining_text = remaining_text.mid(i);
                    break;
                }
            }
        }
        //添加当前字符到当前行
        current_line.append(current_char);
        current_line_width += char_width;
    }

    //添加最后一行（如果有的话）
    if (!current_line.isEmpty())
    {
        lines.append(current_line);
    }
    //如果还有剩余文本，则在最后一行添加省略号
    if (!remaining_text.isEmpty())
    {
        QString last_line = lines.takeLast();
        int ellipsis_width = font_metrics->horizontalAdvance("...");
        while (last_line.length() > 0 && font_metrics->horizontalAdvance(last_line) + ellipsis_width > max_width)
        {
            last_line.chop(1); //移除最后一个字符，以便添加省略号
        }
        last_line += "...";
        lines.append(last_line);
    }

    //返回结果
    std::vector<QString> result_lines;
    for (const auto& line : lines)
    {
        result_lines.emplace_back(line);
    }
    return result_lines;
}

} //namespace

namespace fe
{
DynamicGeometry::DynamicGeometry(const fe::NodeData& data, std::shared_ptr<NodeStyle> node_style, std::shared_ptr<TypeColorMap> type_color_map) :
    data_(data), node_style_(node_style), type_color_map_(type_color_map)
{
    QFont font = node_style_->font;
    QFont blod_font = node_style_->font;
    blod_font.setBold(true);
    font_metrics_ = std::make_unique<QFontMetrics>(font);
    bold_font_metrics_ = std::make_unique<QFontMetrics>(blod_font);

    //初始化对象
    simple_components_.caption_text = QStaticText(data.caption_text);
    components_.caption_text = QStaticText(data.caption_text);
    components_.in_ports.resize(data_.inputs.size());
    components_.out_ports.resize(data_.outputs.size());
    for (size_t i = 0; i < data_.inputs.size(); ++i)
    {
        components_.in_ports[i].port_text = QStaticText(data_.inputs[i].port_name);
        components_.in_ports[i].port_type = data_.inputs[i].port_type;
        auto itr = type_color_map->find(data_.inputs[i].port_type);
        if (itr != type_color_map->end())
        {
            components_.in_ports[i].port_color = itr->second;
        }
        else
        {
            components_.in_ports[i].port_color = { 125, 125, 125 };
        }
    }
    for (size_t i = 0; i < data_.outputs.size(); ++i)
    {
        components_.out_ports[i].port_text = QStaticText(data_.outputs[i].port_name);
        components_.out_ports[i].port_type = data_.outputs[i].port_type;
        auto itr = type_color_map->find(data_.outputs[i].port_type);
        if (itr != type_color_map->end())
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
    auto& components = components_;

    //-----计算端口区域
    double point_diameter = node_style_->connection_point_diameter; //连接点尺寸
    double point_diameter_extend = 2.0 * point_diameter;            //扩展的连接点尺寸
    constexpr double port_spasing = 5;                              //端口字符之间的间隔
    double port_text_height = font_metrics_->height();              //端口字符的高度
    double port_btn_width = port_text_height;                       //端口按钮的尺寸

    //计算端口添加/修改按钮
    bool enable_in_port_add_btn = false;
    bool enable_in_port_del_btn = false;
    bool enable_out_port_add_btn = false;
    bool enable_out_port_del_btn = false;
    if (data_.inputs_config == PortConfig::Modifiable)
    {
        enable_in_port_add_btn = true;
        for (const auto& input : data_.inputs)
        {
            if (input.port_state == PortStatus::Removable)
            {
                enable_in_port_del_btn = true;
                break;
            }
        }
    }
    if (data_.outputs_config == PortConfig::Modifiable)
    {
        enable_out_port_add_btn = true;
        for (const auto& output : data_.outputs)
        {
            if (output.port_state == PortStatus::Removable)
            {
                enable_out_port_del_btn = true;
                break;
            }
        }
    }
    double in_port_btn_width = 0;
    if (enable_in_port_add_btn)
    {
        in_port_btn_width = port_btn_width + port_spasing * 2;
        if (enable_in_port_del_btn)
        {
            in_port_btn_width += port_btn_width;
        }
    }
    double out_port_btn_width = 0;
    if (enable_out_port_add_btn)
    {
        out_port_btn_width = port_btn_width + port_spasing * 2;
        if (enable_out_port_del_btn)
        {
            out_port_btn_width += port_btn_width;
        }
    }
    double port_btn_redundancy = port_btn_width;
    //计算输入/输出文本区域
    double in_width = 0;
    for (const auto& port : data_.inputs)
    {
        in_width = std::max(in_width, textWidth(font_metrics_.get(), port.port_name));
    }
    double out_width = 0;
    for (const auto& port : data_.outputs)
    {
        out_width = std::max(out_width, textWidth(font_metrics_.get(), port.port_name));
    }
    double port_margin_top = port_text_height * 0.5;
    double port_margin_bottom = port_text_height * 0.5;
    double port_width = in_width + out_width + in_port_btn_width + out_port_btn_width + port_btn_redundancy + point_diameter + 2 * port_spasing; //端口区域宽度
    double port_height = maxVerticalPortsExtent(data_, port_text_height, port_spasing) + port_margin_top + port_margin_bottom;                   //端口字符区域高度
    double message_margin_x = port_spasing;
    double message_margin_y = port_spasing;

    //-----计算标题区域
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

    //-----计算错误提示框区域
    double node_width = std::max(title_width, port_width);
    QSizeF message_size = { 0, 0 };
    if (data_.error != ErrorLevel::Accept && !data_.error_message.isEmpty())
    {
        double max_message_width = node_width - message_margin_x;
        components.enable_message = true;
        components.message_box_color = (data_.error == ErrorLevel::Error) ? QColor(213, 7, 7) : QColor(111, 111, 7);

        //消息切分并添加对象
        std::vector<QString> message_multi_line = getMultiLineMessage(font_metrics_.get(), data_.error_message, max_message_width, 3);
        components.message_texts.resize(message_multi_line.size());
        components.message_positions.resize(message_multi_line.size());
        for (size_t i = 0; i < message_multi_line.size(); ++i)
        {
            components.message_texts[i] = QStaticText(message_multi_line[i]);
        }

        message_size = { node_width, port_text_height * message_multi_line.size() + message_margin_y * 2 };
    }

    //计算节点主区域的尺寸
    double node_height = title_height + port_height + message_size.height();
    QSizeF node_size = { node_width, node_height };

    //计算边界保留区域
    constexpr double margin = 3.0; //3个像素的边界保留
    double left_margin = std::max(icon_offset, point_diameter_extend * 0.5) + margin;
    double right_margin = point_diameter_extend * 0.5 + margin;
    double top_margin = icon_offset + margin;
    double bottom_margin = margin;

    //计算边界区域
    components.bounding_rect = { 0, 0, node_size.width() + left_margin + right_margin, node_size.height() + top_margin + bottom_margin };
    components.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = components.bounding_rect.topLeft() + QPointF{ left_margin, top_margin };
    double offset_x = translate_offset.x();
    double offset_y = translate_offset.y();

    //计算各区域位置
    components.node_rect = { translate_offset, node_size };
    components.icon_rect = { QPointF{ -icon_offset + offset_x, -icon_offset + offset_y }, icon_size };
    components.run_btn_rect = { { node_width - btn_size.width() + offset_x, 0 + offset_y }, btn_size };
    double caption_offset = (icon_size2.height() - caption_size.height()) * 0.5;
    components.caption_rect = { { icon_size2.width() + offset_x, caption_offset + offset_y }, caption_size };
    components.title_rect = { icon_size2.width() + offset_x, offset_y, node_width - icon_size2.width() - btn_size.width(), title_height - 3.0 };
    components.port_rect = { offset_x, title_height + offset_y + port_margin_top, node_width, port_height };
    components.caption_position = components.caption_rect.topLeft();
    if (components.enable_message)
    {
        components.message_box_rect = { offset_x, title_height + port_height + offset_y, node_width, message_size.height() };
        double message_text_left = components.message_box_rect.left() + message_margin_x;
        double message_text_top = components.message_box_rect.top() + message_margin_y;

        if (components.message_texts.size() == 1)
        {
            //如果只有一行,则实现居中显示
            QString msg = components.message_texts[0].text();
            QSizeF msg_size = font_metrics_->boundingRect(msg).size();
            components.message_positions[0] = { message_text_left + (node_width - msg_size.width()) * 0.5, message_text_top };
        }
        else
        {
            for (size_t i = 0; i < components.message_texts.size(); ++i)
            {
                components.message_positions[i] = { message_text_left, message_text_top + (i * port_text_height) };
            }
        }
    }

    //计算运行按钮三角位置
    double btn_offset = btn_size.width() * 0.2;
    double btn_offset2 = btn_size.width() * 0.1;
    components.run_btn_rect2 = { components.run_btn_rect.topLeft() + QPointF{ btn_offset2, btn_offset2 }, QSizeF{ btn_size.width() - 2 * btn_offset2, btn_size.height() - 2 * btn_offset2 } };
    components.run_btn_polygon[0] = { components.run_btn_rect.topLeft() + QPointF{ btn_offset, btn_offset } };
    components.run_btn_polygon[1] = QPointF{ components.run_btn_rect.right() - btn_offset, components.run_btn_rect.center().y() };
    components.run_btn_polygon[2] = { components.run_btn_rect.bottomLeft() + QPointF{ btn_offset, -btn_offset } };

    //开始绘画各port区域
    double half_point_size = point_diameter * 0.5;
    double half_point_diameter_extend = point_diameter_extend * 0.5;
    double point_offset = (port_text_height - point_diameter) * 0.5;
    double point_offset2 = (port_text_height - point_diameter_extend) * 0.5;
    double total_port_height = title_height + port_margin_top + port_spasing * 0.5;
    for (size_t i = 0; i < components.in_ports.size(); ++i)
    {
        components.in_ports[i].port_rect = QRectF{ -half_point_size + offset_x, total_port_height + point_offset + offset_y, point_diameter, point_diameter };
        components.in_ports[i].port_rect_extend = QRectF{ -half_point_diameter_extend + offset_x, total_port_height + point_offset2 + offset_y, point_diameter_extend, point_diameter_extend };
        components.in_ports[i].port_text_rect = QRectF{ half_point_size + port_spasing + offset_x, total_port_height + offset_y, in_width, (float)port_text_height };
        components.in_ports[i].port_center = components.in_ports[i].port_rect.center();
        components.in_ports[i].port_text_position = components.in_ports[i].port_text_rect.topLeft();
        total_port_height += port_text_height + port_spasing;
    }
    //计算添加/删除按钮
    if (enable_in_port_add_btn)
    {
        double crt_top = title_height + port_margin_top + port_spasing * 0.5 + offset_y;
        double left = half_point_size + port_spasing + offset_x + in_width + port_spasing * 2;
        if (enable_in_port_del_btn)
        {
            double left2 = left + port_btn_width;
            for (size_t i = 0; i < components.in_ports.size(); ++i)
            {
                components.in_ports[i].port_add_btn_rect = { left, crt_top, port_btn_width, port_btn_width };
                components.in_ports[i].port_del_btn_rect = { left2, crt_top, port_btn_width, port_btn_width };
                crt_top += port_text_height + port_spasing;
            }
        }
        else
        {
            for (size_t i = 0; i < components.in_ports.size(); ++i)
            {
                components.in_ports[i].port_add_btn_rect = { left, crt_top, port_btn_width, port_btn_width };
                crt_top += port_text_height + port_spasing;
            }
        }
    }

    total_port_height = title_height + port_margin_top + port_spasing * 0.5;
    double output_offset_x = node_width - half_point_size - out_width - port_spasing;
    double output_point_offset_x = node_width - half_point_size;
    double output_point_offset_x2 = node_width - half_point_diameter_extend;
    for (size_t i = 0; i < components.out_ports.size(); ++i)
    {
        components.out_ports[i].port_rect = QRectF{ output_point_offset_x + offset_x, total_port_height + point_offset + offset_y, point_diameter, point_diameter };
        components.out_ports[i].port_rect_extend = QRectF{ output_point_offset_x2 + offset_x, total_port_height + point_offset2 + offset_y, point_diameter_extend, point_diameter_extend };
        components.out_ports[i].port_text_rect = QRectF{ output_offset_x + offset_x, total_port_height + offset_y, out_width, (float)port_text_height };
        components.out_ports[i].port_center = components.out_ports[i].port_rect.center();
        components.out_ports[i].port_text_position = components.out_ports[i].port_text_rect.topLeft();
        total_port_height += port_text_height + port_spasing;
    }
    //计算添加/删除按钮
    if (enable_out_port_add_btn)
    {
        double crt_top = title_height + port_margin_top + port_spasing * 0.5 + offset_y;
        double left = output_offset_x + offset_x - out_port_btn_width;
        if (enable_in_port_del_btn)
        {
            double left2 = left + port_btn_width;
            for (size_t i = 0; i < components.out_ports.size(); ++i)
            {
                components.out_ports[i].port_add_btn_rect = { left, crt_top, port_btn_width, port_btn_width };
                components.out_ports[i].port_del_btn_rect = { left2, crt_top, port_btn_width, port_btn_width };
                crt_top += port_text_height + port_spasing;
            }
        }
        else
        {
            for (size_t i = 0; i < components.out_ports.size(); ++i)
            {
                components.out_ports[i].port_add_btn_rect = { left, crt_top, port_btn_width, port_btn_width };
                crt_top += port_text_height + port_spasing;
            }
        }
    }

    components.enable_in_port_add_btn = enable_in_port_add_btn;
    components.enable_in_port_del_btn = enable_in_port_del_btn;
    components.enable_out_port_add_btn = enable_out_port_add_btn;
    components.enable_out_port_del_btn = enable_out_port_del_btn;
}
void DynamicGeometry::updateSimple(double scale)
{
    auto& components = simple_components_;

    double point_diameter_extend = node_style_->connection_point_diameter * 2; //连接点尺寸
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
    components.bounding_rect = { 0, 0, node_size.width() + left_margin + right_margin, node_size.height() + top_margin + bottom_margin };
    components.bounding_rect.moveCenter({ 0, 0 });
    QPointF translate_offset = components.bounding_rect.topLeft() + QPointF{ left_margin, top_margin };
    double offset_x = translate_offset.x();
    double offset_y = translate_offset.y();

    //计算标题区域
    double icon_extend_width = icon_size.width() + point_diameter_extend;
    if (caption_size.width() >= icon_extend_width)
    {
        //标题比icon扩展区域大
        double icon_title_offset_x = (caption_size.width() - icon_size.width()) * 0.5;
        components.caption_rect = { offset_x, offset_y + icon_size.height(), caption_size.width(), caption_size.height() };
        components.icon_rect = { offset_x + icon_title_offset_x, offset_y, icon_size.width(), icon_size.height() };
    }
    else
    {
        //标题比icon区域小
        double icon_title_offset_x = (icon_size.width() - caption_size.width()) * 0.5;
        components.icon_rect = { offset_x + half_point_diameter_extend, offset_y, icon_size.width(), icon_size.height() };
        components.caption_rect = { offset_x + half_point_diameter_extend + icon_title_offset_x, offset_y + icon_size.height(), caption_size.width(), caption_size.height() };
    }

    //计算port区域
    components.in_port_rect = { components.icon_rect.left() - half_point_diameter_extend, components.icon_rect.center().y() - half_point_diameter_extend, point_diameter_extend, point_diameter_extend };
    components.out_port_rect = { components.icon_rect.right() - half_point_diameter_extend, components.icon_rect.center().y() - half_point_diameter_extend, point_diameter_extend, point_diameter_extend };
    components.in_port_center = components.in_port_rect.center();
    components.out_port_center = components.out_port_rect.center();

    //计算消息框
    if (data_.error != ErrorLevel::Accept && !data_.error_message.isEmpty())
    {
        components.message_box_color = (data_.error == ErrorLevel::Error) ? QColor(213, 7, 7) : QColor(255, 255, 7);
        components.message_box_rect = { 0, 0, 20, 20 };
        components.message_box_rect.moveBottomRight(components.icon_rect.bottomRight());
    }
    else
    {
        components.message_box_rect = {};
    }
}
} //namespace fe
