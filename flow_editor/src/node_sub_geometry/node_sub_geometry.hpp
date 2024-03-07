//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <vector>
#include <QSize>
#include <QPointF>
#include <QRectF>

namespace fe
{
//NodeItem里面每个组件的尺寸,用于渲染时绘画,图像中心为坐标(0,0)
class NodeSubGeometry
{
public:
    QRectF bounding_rect;                   //边界区域
    QRectF node_rect;                       //节点区域
    std::vector<QRectF> in_port_rect;       //输入端口位置
    std::vector<QRectF> out_port_rect;      //输出端口位置
    std::vector<QRectF> in_port_text_rect;  //输入端口文本位置
    std::vector<QRectF> out_port_text_rect; //输出端口文本位置
    QRectF icon_rect;                       //图标位置区域
    QRectF caption_rect;                    //标题区域
    QRectF run_btn_rect;                    //运行标志区域
    QRectF message_box_rect;                //消息区域位置(若有)
};
} //namespace fe