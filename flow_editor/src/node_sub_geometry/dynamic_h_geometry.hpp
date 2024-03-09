//
// Created by cxc on 2024/3/8.
//

#pragma once
#include <memory>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>
#include <QFontMetrics>

namespace fe
{
class DynamicHGeometry
{
public:
    DynamicHGeometry(const NodeData& data, std::shared_ptr<NodeStyle>& node_style);
    void update(double scale);
    void updateSimple(double scale);
    const NodeUIComponents& components() const { return components_; }
    const NodeUISimpleComponents& simpleComponents() const { return simple_components_; }

private:
    //输入
    const NodeData& data_;
    std::shared_ptr<NodeStyle> node_style_;
    //输出
    NodeUIComponents components_;
    NodeUISimpleComponents simple_components_;
    //缓存
    std::unique_ptr<QFontMetrics> font_metrics_;
    std::unique_ptr<QFontMetrics> bold_font_metrics_;
};

} //namespace fe
