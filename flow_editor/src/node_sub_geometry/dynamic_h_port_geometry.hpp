//
// Created by cxc on 2024/3/8.
//

#pragma once
#include <memory>
#include <QFontMetrics>
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>

namespace fe
{
class DynamicHPortGeometry
{
public:
    DynamicHPortGeometry(NodeType node_type, const NodeData& data, std::shared_ptr<NodeStyle> node_style, std::shared_ptr<TypeColorMap> type_color_map);
    void update(double scale);
    const PortUIComponents& components() const { return components_; }

private:
    //输入
    NodeType node_type_;
    const NodeData& data_;
    std::shared_ptr<NodeStyle> node_style_;
    std::shared_ptr<TypeColorMap> type_color_map_;
    //输出
    PortUIComponents components_;
    //缓存
    double last_scale_ = 0;
    std::unique_ptr<QFontMetrics> bold_font_metrics_;
};

} //namespace fe
