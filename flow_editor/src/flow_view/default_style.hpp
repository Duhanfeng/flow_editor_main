//
// Created by cxc on 2024/3/11.
//

#pragma once
#include <flow_editor/data_def.hpp>
#include <QColor>
#include <QFont>

namespace fe
{
void initDefaultStyle(std::shared_ptr<fe::NodeStyle>& node_style)
{
    if (node_style == nullptr)
    {
        node_style = std::make_shared<fe::NodeStyle>();
    }
    node_style->normal_boundary_color = QColor(255, 255, 255);
    node_style->selected_boundary_color = QColor(255, 165, 0);
    node_style->gradient_color0 = QColor(125, 125, 125);
    node_style->gradient_color1 = QColor(80, 80, 80);
    node_style->gradient_color2 = QColor(64, 64, 64);
    node_style->gradient_color3 = QColor(58, 58, 58);
    node_style->shadow_color = QColor(20, 20, 20);
    node_style->font_color = QColor(255, 255, 255);
    node_style->font_color_faded = QColor(125, 125, 125);
    node_style->connection_point_color = QColor(169, 169, 169);
    node_style->filled_connection_point_color = QColor(0, 255, 255);
    node_style->warning_color = QColor(128, 128, 0);
    node_style->error_color = QColor(255, 0, 0);
    node_style->btn_color = QColor(0, 255, 0);
    node_style->btn_hovered_color = QColor(125, 125, 125);
    node_style->btn_checked_color = QColor(255, 0, 0);
    node_style->pen_width = 1.0f;
    node_style->hovered_pen_width = 2.0f;
    node_style->connection_point_diameter = 10.0f;
    node_style->opacity = 1.0f;
    node_style->font_name = u8"NSimSun";
    node_style->font_size = 12;
    node_style->font = QFont(node_style->font_name, node_style->font_size);
}
void initDefaultStyle(std::shared_ptr<fe::ConnectionStyle>& connection_style)
{
    if (connection_style == nullptr)
    {
        connection_style = std::make_shared<fe::ConnectionStyle>();
    }
    connection_style->construction_color = QColor("gray");
    connection_style->normal_color = QColor("darkcyan");
    connection_style->selected_color = QColor(100, 100, 100);
    connection_style->selected_halo_color = QColor("orange");
    connection_style->hovered_color = QColor("lightcyan");
    connection_style->line_width = 3.0f;
    connection_style->construction_line_width = 2.0f;
    connection_style->point_diameter = 10.0f;
    connection_style->use_data_defined_colors = false;
}
void initDefaultStyle(std::shared_ptr<fe::DraftConnectionStyle>& draft_connection_style)
{
    if (draft_connection_style == nullptr)
    {
        draft_connection_style = std::make_shared<fe::DraftConnectionStyle>();
    }
    draft_connection_style->construction_color = QColor("gray");
    draft_connection_style->line_width = 3.0f;
    draft_connection_style->construction_line_width = 2.0f;
    draft_connection_style->point_diameter = 10.0f;
}
}