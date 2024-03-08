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
class DynamicHorGeometry
{
public:
    DynamicHorGeometry(const NodeData& data, std::shared_ptr<NodeStyle>& node_style);
    void update(double scale);
    const NodeSubGeometry& geometry() const { return node_sub_geometry_; }

private:
    //输入
    const NodeData& data_;
    std::shared_ptr<NodeStyle> node_style_;
    //输出
    NodeSubGeometry node_sub_geometry_;

    //缓存
    bool is_inited_ = false;
    std::unique_ptr<QFontMetrics> font_metrics_;
    std::unique_ptr<QFontMetrics> bold_font_metrics_;
};

} //namespace fe
