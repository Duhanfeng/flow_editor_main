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
    In,
    Out,
    None
};
enum class NodeType
{
    Node,
    InNode,
    OutNode
};
enum class PortStatus
{
    Immutable, //端口是固定的,不能改动
    Removable  //端口是可以被移除的
};
//PortConfig和PortStatus的区别:  PortStatus只是决定当前端口是否可以被移除,而PortConfig为全局配置,仅为Modifiable时,每个端口的PortStatus才有效
enum class PortConfig
{
    Immutable, //所有端口都是固定的，不能改动
    Modifiable //可以自由添加和移除端口
};
class PortData
{
public:
    QString port_name;                             //端口名
    QString port_type;                             //端口类型,同名类型才可以连接,空表示不进行过滤
    PortStatus port_state = PortStatus::Immutable; //端口状态
};
enum class ErrorLevel
{
    Accept = 0,     //接受(正常运行,无异常)
    Warning = 1,    //警告(正常运行,但会记录进日志)
    Error = 2,      //错误(终止运行,并记录日志)
    Unexecuted = 4, //未执行/跳过执行
};
class NodeData
{
public:
    //基础属性
    QString caption_text;        //节点标题
    QIcon node_icon;             //节点图标
    QPointF position;            //节点位置
    bool is_highlighted = false; //是否高亮显示
    //端口配置
    std::vector<PortData> inputs;  //输入端口
    std::vector<PortData> outputs; //输出端口
    PortConfig inputs_config;      //输入端口配置
    PortConfig outputs_config;     //输出端口配置
    //消息提示
    ErrorLevel error = ErrorLevel::Accept;
    QString error_message;
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

//----Flow权限
enum FlowPermission
{
    FlowPermissionNone = 0,      //无权限
    NodeMovable = 0x0001,        //节点可移动
    InputNodeMovable = 0x0002,   //输入节点可移动
    OutputNodeMovable = 0x0004,  //输出节点可移动
    NodeDeletable = 0x0010,      //节点可删除
    ConnectionEditable = 0x0020, //连接可编辑（新增/修改/删除）
    FlowPermissionAll = 0xFFFF
};

enum class SceneShowMode
{
    Dynamic,    //动态计算
    FixNormal,  //强制正常模式
    FixPreview, //强制预览模式
};

//-----Style

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
    QColor normal_boundary_color;         //正常边界的颜色
    QColor selected_boundary_color;       //被选中边界的颜色
    QColor gradient_color0;               //渐变色0，用于背景或效果
    QColor gradient_color1;               //渐变色1
    QColor gradient_color2;               //渐变色2
    QColor gradient_color3;               //渐变色3
    QColor shadow_color;                  //阴影的颜色
    QColor font_color;                    //正常字体颜色
    QColor font_color_faded;              //淡化字体颜色，用于提示或者次级文字
    QColor connection_point_color;        //连接点的颜色
    QColor filled_connection_point_color; //填充的连接点的颜色
    QColor warning_color;                 //警告的颜色，用于显示潜在问题
    QColor error_color;                   //错误的颜色，用于显示发生错误的情况
    QColor btn_color;                     //按钮颜色
    QColor btn_hovered_color;             //按钮悬停时的背景颜色
    QColor btn_checked_color;             //按钮点击时颜色

    float icon_size = 36.0f;                //Icon图标默认尺寸
    float pen_width = 1.0f;                 //画笔宽度，用于绘制边界或线条
    float hovered_pen_width = 2.0f;         //鼠标悬停时的画笔宽度，用于突出显示
    float connection_point_diameter = 5.0f; //连接点的直径大小
    float opacity = 1.0f;                   //不透明度，用于控制颜色的透明度, 范围:[0,1]

    QString font_name; //默认字体名
    int font_size = 9; //默认字体大小
    QFont font;        //默认字体
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
    bool use_data_defined_colors; //是否使用数据定义的颜色
};
class DraftConnectionStyle
{
public:
    QColor construction_color;     //用于构造时的颜色
    float line_width;              //线条宽度
    float construction_line_width; //构造时线条宽度
    float point_diameter;          //点的直径
};

using TypeColorMap = std::map<QString, QColor>;

} //namespace fe