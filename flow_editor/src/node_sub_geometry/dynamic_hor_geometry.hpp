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
    void updateSimple(double scale);
    const NodeSubGeometry& geometry() const { return geometry_; }
    const NodeSubGeometry& simpleGeometry() const { return simple_geometry_; }

private:
    //输入
    const NodeData& data_;
    std::shared_ptr<NodeStyle> node_style_;
    //输出
    NodeSubGeometry geometry_;
    NodeSubGeometry simple_geometry_;

    //缓存
    bool is_inited_ = false;
    QRect bast_size_;
    std::unique_ptr<QFontMetrics> font_metrics_;
    std::unique_ptr<QFontMetrics> bold_font_metrics_;
};

} //namespace fe
