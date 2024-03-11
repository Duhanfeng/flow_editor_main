//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QColor>
#include <QIcon>
#include <QFont>
#include <map>

namespace fe
{

enum class NodeLayoutStyle
{
    Horizontal,
    Vertical
};
enum class PortType
{
    In = 0,  ///<Input node port (from the left).
    Out = 1, ///<Output node port (from the right).
    None = 2
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
    QString node_name;
    QIcon node_icon;
    QPointF position;
    std::vector<PortData> in_port;
    std::vector<PortData> out_port;
};

using guid16 = std::array<unsigned char, 16>;
using guid18 = std::array<unsigned char, 18>;
template <size_t _Size>
inline bool isInvalid(const std::array<unsigned char, _Size>& id)
{
    for (size_t i = 0; i < _Size; ++i)
    {
        if (id[i] != 0)
        {
            return false;
        }
    }
    return true;
}

class Connection
{
public:
    guid16 out;
    unsigned int out_port = 0;
    guid16 in;
    unsigned int in_port = 0;
};
class Flow
{
public:
    virtual ~Flow() = default;

public:
    //原始数据
    std::map<guid16, NodeData> nodes;
    std::map<guid16, NodeData> in_nodes;
    std::map<guid16, NodeData> out_nodes;
    std::map<guid18, Connection> connections;

public:
    //界面交互响应
    virtual bool tryDeleteNode(const guid16& id)
    {
        auto iter = nodes.find(id);
        if (iter != nodes.end())
        {
            nodes.erase(iter);
            return true;
        }
        return false;
    }
    virtual bool tryDisconnect(const guid18& id)
    {
        auto iter = connections.find(id);
        if (iter != connections.end())
        {
            connections.erase(iter);
            return true;
        }
        return false;
    }
    virtual guid18 tryConnect(const Connection& connection)
    {
        return { 0 };
    }
};

class GraphicsViewStyle
{
public:
    QColor background_color;
    QColor fine_grid_color;
    QColor coarse_grid_color;
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
class ConnectionStyle
{
public:
    //基础颜色
    QColor construction_color;  //用于构造时的颜色
    QColor normal_color;        //正常状态下的颜色
    QColor selected_color;      //被选中时的颜色
    QColor selected_halo_color; //被选中时光晕的颜色
    QColor hovered_color;       //鼠标悬停时的颜色

    //定义线条和点的尺寸
    float line_width;              //线条宽度
    float construction_line_width; //构造时线条宽度
    float point_diameter;          //点的直径

    //自定义颜色
    bool use_data_defined_colors;             //是否使用数据定义的颜色
    std::map<QString, QColor> type_color_map; //类型与颜色映射字典
};

class DraftConnectionStyle
{
public:
    QColor construction_color;     //用于构造时的颜色
    float line_width;              //线条宽度
    float construction_line_width; //构造时线条宽度
    float point_diameter;          //点的直径
};

} //namespace fe