//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <vector>
#include <QSize>
#include <QPointF>
#include <QRectF>
#include <QStaticText>

namespace fe
{
//NodeItem里面每个组件的尺寸,用于渲染时绘画,坐标原点(0,0)为边界区域的中心
class NodeUIComponents
{
public:
    //子组件区域
    QRectF bounding_rect;                   //边界区域
    QRectF node_rect;                       //节点区域
    QRectF title_rect;                      //title区域,即包括了 图标+标题+运行标志 的区域
    QRectF icon_rect;                       //图标位置区域
    QRectF caption_rect;                    //标题区域
    QRectF run_btn_rect;                    //运行标志区域
    QRectF port_rect;                       //端口区域
    QRectF message_box_rect;                //消息区域位置(若有)
    std::vector<QRectF> in_port_rect;       //输入端口位置
    std::vector<QRectF> in_port_text_rect;  //输入端口文本位置
    std::vector<QRectF> out_port_rect;      //输出端口位置
    std::vector<QRectF> out_port_text_rect; //输出端口文本位置
    std::array<QPointF, 3> run_polygon;     //运行按钮多边形

    //字符缓存
    QStaticText node_name;
    std::vector<QStaticText> in_port_text;
    std::vector<QStaticText> out_port_text;
};
//简略图实现
class NodeUISimpleComponents
{
public:
    //子组件区域
    QRectF bounding_rect; //边界区域
    QRectF icon_rect;     //图标位置区域
    QRectF caption_rect;  //标题区域
    QRectF in_port_rect;  //输入端口位置
    QRectF out_port_rect; //输出端口位置
    //字符缓存
    QStaticText node_name;
};
//端口
class PortUIComponents
{
public:
    //子组件区域
    QRectF bounding_rect;   //边界区域
    QRectF icon_rect;       //图标位置区域
    QRectF caption_rect;    //标题区域
    QRectF port_rect;       //端口位置
    QRectF port_rect2;      //端口位置(扩大区域)
    QPolygonF node_polygon; //node多边形
    //字符缓存
    QStaticText port_name;
};

} //namespace fe