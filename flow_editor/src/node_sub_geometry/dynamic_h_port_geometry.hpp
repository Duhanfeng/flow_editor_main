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
class DynamicHPortGeometry
{
public:
    DynamicHPortGeometry(const NodeData& data, std::shared_ptr<NodeStyle>& node_style);
    void update(double scale);
    const PortSubGeometry& geometry() const { return geometry_; }

private:
    //输入
    const NodeData& data_;
    std::shared_ptr<NodeStyle> node_style_;
    //输出
    PortSubGeometry geometry_;
    //缓存
    double last_scale_ = 0;
    std::unique_ptr<QFontMetrics> bold_font_metrics_;
};

} //namespace fe
