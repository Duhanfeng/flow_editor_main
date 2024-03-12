﻿//
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
class PortComponent
{
public:
    QRectF port_rect;           //端口连接点交互区域
    QRectF port_rect_extend;    //端口连接点交互区域(扩展,用于动态显示时)
    QRectF port_text_rect;      //端口文本区域
    QPointF port_center;        //端口中心
    QPointF port_text_position; //端口文本绘画位置
    QStaticText port_text;      //端口文本内容
    QString port_type;          //端口类型
    QColor port_color;          //端口颜色
};

//NodeItem里面每个组件的尺寸,用于渲染时绘画,坐标原点(0,0)为边界区域的中心
class NodeUIComponents
{
public:
    //主框架
    QRectF bounding_rect;                 //边界区域
    QRectF node_rect;                     //节点区域
    QRectF title_rect;                    //title区域
    QRectF icon_rect;                     //图标位置区域
    QRectF caption_rect;                  //标题区域
    QRectF run_btn_rect;                  //运行按钮区域
    QRectF run_btn_rect2;                 //运行按钮区域(裁剪后的)
    QRectF port_rect;                     //端口区域
    std::vector<PortComponent> in_ports;  //输入端口
    std::vector<PortComponent> out_ports; //输出端口
    QPolygonF run_btn_polygon;            //运行按钮

    //标题
    QPointF caption_position; //标题绘画位置
    QStaticText caption_text; //标题内容

    //消息区域
    bool enable_message = false; //使能消息
    QRectF message_box_rect;     //消息区域位置(若有)
    QPointF message_position;    //消息绘画位置
    QStaticText message_text;    //消息内容
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
    QStaticText caption_text;
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