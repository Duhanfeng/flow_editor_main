//
// Created by cxc on 2024/3/13.
//

#pragma once
#include <flow_editor/flow_editor.hpp>
#include "flow.hpp"
#include "util/guid_creator.hpp"
#include "util/random_string.hpp"
#include "util/style_creator.hpp"

inline void createNodes(std::shared_ptr<Flow2> flow)
{
    QIcon icon(":/icon.bmp");

    //绘画对象
    fe::NodeData in_data1 = {
        u8"输入端口#1",
        icon,
        { -300.0, 100.0 },
        false,
        {},
        { { u8"输出端口1", u8"整型" } }
    };
    fe::NodeData in_data2 = {
        u8"命名稍长输入端口#2",
        icon,
        { -300.0, 300.0 },
        false,
        {},
        { { u8"输出端口1", u8"" } }
    };
    fe::NodeData out_data1 = {
        u8"输出端口#1",
        icon,
        { 300.0, 100.0 },
        false,
        { { u8"输入端口1", u8"整型" } },
        {}
    };
    fe::NodeData out_data2 = {
        u8"命名稍长输出端口#2",
        icon,
        { 300.0, 300.0 },
        false,
        { { u8"输入端口1", u8"" } },
        {}
    };

    fe::NodeData data5 = {
        u8"这是一段比较长的标题#1",
        icon,
        { 0.0, 0.0 },
        false,
        { { u8"输入端口1", u8"整型" },
            { u8"输入端口2", u8"" },
            { u8"输入端口3", u8"浮点数" } },
        { { u8"输出端口1", u8"浮点数" },
            { u8"输出端口2", u8"" },
            { u8"输出端口3", u8"浮点数" },
            { u8"输出端口4", u8"" } }
    };
    fe::NodeData data6 = {
        u8"节点2",
        icon,
        { 0.0, 400.0 },
        false,
        { { u8"输入端口1", u8"浮点数" },
            { u8"输入端口2", u8"整型" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"整型" },
            { u8"输入端口2", u8"整型" },
            { u8"this is long long port", u8"" } },
        { { u8"输出端口1", u8"" },
            { u8"输出端口2", u8"整型" },
            { u8"输出端口3", u8"浮点数" },
            { u8"输出端口4", u8"" } },
        fe::PortConfigOptions::Fixed,
        fe::PortConfigOptions::Fixed,
        fe::ErrorLevel::Error,
        u8"执行错误"
    };
    fe::NodeData data7 = {
        u8"执行节点",
        icon,
        { 0.0, 200.0 },
        true,
        { { u8"浮点数", u8"浮点数" },
            { u8"整型", u8"整型" },
            { u8"通用端口", u8"" } },
        { { u8"整型", u8"整型" },
            { u8"浮点数", u8"浮点数" } },
        fe::PortConfigOptions::Fixed,
        fe::PortConfigOptions::Fixed,
        fe::ErrorLevel::Warning,
        u8"这是一个需要自动换行的长文本示例，如果文本长度超过边界宽度，则需要自动换行。"
    };

    std::array<unsigned char, 16> guid1 = { 01 };
    std::array<unsigned char, 16> guid2 = { 02 };
    std::array<unsigned char, 16> guid3 = { 03 };
    std::array<unsigned char, 16> guid4 = { 04 };
    std::array<unsigned char, 16> guid5 = { 05 };
    std::array<unsigned char, 16> guid6 = { 06 };
    std::array<unsigned char, 16> guid7 = { 07 };
    flow->in_nodes.emplace(guid1, in_data1);
    flow->in_nodes.emplace(guid2, in_data2);
    flow->out_nodes.emplace(guid3, out_data1);
    flow->out_nodes.emplace(guid4, out_data2);
    flow->nodes.emplace(guid5, data5);
    flow->nodes.emplace(guid6, data6);
    flow->nodes.emplace(guid7, data7);
    std::array<unsigned char, 18> c_guid1 = { 01 };
    flow->connections.emplace(c_guid1, fe::Connection{ guid1, 0, guid3, 0 });
}

inline void createRandomNodes(std::shared_ptr<Flow2> flow)
{
    QIcon icon(":/icon.bmp");
    //随机生成100个节点
    const int count = 1000;
    for (int i = 0; i < count; ++i)
    {
        fe::NodeData random_data;
        //random_data.node_type = fe::NodeType::Node;
        random_data.caption_text = generateRandomString(2, 10);
        random_data.position.setX((double)QRandomGenerator::global()->bounded(-2000, 2000));
        random_data.position.setY((double)QRandomGenerator::global()->bounded(-2000, 2000));
        random_data.node_icon = icon;
        int in_count = QRandomGenerator::global()->bounded(1, 5);
        for (int j = 0; j < in_count; ++j)
        {
            fe::PortData port;
            port.port_name = generateRandomString(2, 5);
            random_data.inputs.emplace_back(port);
        }

        int out_count = QRandomGenerator::global()->bounded(1, 5);
        for (int j = 0; j < out_count; ++j)
        {
            fe::PortData port;
            port.port_name = generateRandomString(2, 5);
            random_data.outputs.emplace_back(port);
        }
        flow->nodes.emplace(createGuid16(), random_data);
    }
}
