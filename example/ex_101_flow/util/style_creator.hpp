//
// Created by cxc on 2024/3/13.
//

#pragma once
#include <memory>
#include <flow_editor/flow_editor.hpp>

inline std::shared_ptr<fe::NodeStyle> createStyle()
{
    std::shared_ptr<fe::NodeStyle> node_style = std::make_shared<fe::NodeStyle>();
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
    node_style->btn_checked_color = QColor(0, 125, 0);

    node_style->icon_size = 56.0f;
    node_style->pen_width = 1.0f;
    node_style->hovered_pen_width = 1.0f;
    node_style->connection_point_diameter = 10.0f;
    node_style->opacity = 1.0f;
    node_style->font_name = u8"NSimSun";
    node_style->font_size = 12;
    node_style->font = QFont(node_style->font_name, node_style->font_size);

    return node_style;
};


inline std::shared_ptr<fe::TypeColorMap> createTypeColorMap()
{
    auto type_color_map = std::make_shared<fe::TypeColorMap>();
    type_color_map->emplace(u8"整型", QColor(0x55, 0xAA, 0xFF));
    type_color_map->emplace(u8"浮点数", QColor(0xAA, 0x55, 0x00));
    type_color_map->emplace(u8"", QColor(0x80, 0x80, 0x80));

    return type_color_map;
}