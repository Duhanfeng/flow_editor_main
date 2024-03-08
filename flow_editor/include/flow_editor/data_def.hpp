//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QColor>
#include <QIcon>
#include <QFont>

namespace fe
{
class GraphicsViewStyle
{
public:
    QColor background_color;
    QColor fine_grid_color;
    QColor coarse_grid_color;
};

enum class NodeLayoutStyle
{
    Horizontal,
    Vertical
};
enum class NodeType
{
    Node,
    InNode,
    OutNode
};
class PortData
{
public:
    QString port_name;
    QColor port_color;
};
class NodeData
{
public:
    NodeType node_type = NodeType::Node;
    QString node_name;
    QIcon node_icon;
    QPointF position;
    std::vector<PortData> in_port;
    std::vector<PortData> out_port;
};

class NodeStyle
{
public:
    QColor normal_boundary_color;           //正常边界的颜色
    QColor selected_boundary_color;         //被选中边界的颜色
    QColor gradient_color0;                 //渐变色0，用于背景或效果
    QColor gradient_color1;                 //渐变色1
    QColor gradient_color2;                 //渐变色2
    QColor gradient_color3;                 //渐变色3
    QColor shadow_color;                    //阴影的颜色
    QColor font_color;                      //正常字体颜色
    QColor font_color_faded;                //淡化字体颜色，用于提示或者次级文字
    QColor connection_point_color;          //连接点的颜色
    QColor filled_connection_point_color;   //填充的连接点的颜色
    QColor warning_color;                   //警告的颜色，用于显示潜在问题
    QColor error_color;                     //错误的颜色，用于显示发生错误的情况
    float pen_width = 1.0f;                 //画笔宽度，用于绘制边界或线条
    float hovered_pen_width = 2.0f;         //鼠标悬停时的画笔宽度，用于突出显示
    float connection_point_diameter = 5.0f; //连接点的直径大小
    float opacity = 1.0f;                   //不透明度，用于控制颜色的透明度, 范围:[0,1]
    QString font_name;                      //默认字体名
    int font_size = 9;                      //默认字体大小
    QFont font;                             //默认字体
};

} //namespace fe